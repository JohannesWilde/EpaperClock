from io import StringIO
import csv

csvValues = \
    """SEL_F;00;01;10;11
000000;156.25;78.13;39.06;-
000001;159.01;79.5;39.75;-
000010;161.76;80.88;40.44;20.22
000011;164.52;82.26;41.13;20.57
000100;167.28;83.64;41.82;20.91
000101;170.04;85.02;42.51;21.25
000110;172.79;86.4;43.2;21.6
000111;175.55;87.78;43.89;21.94
001000;178.31;89.15;44.58;22.29
001001;181.07;90.53;45.27;22.63
001010;183.82;91.91;45.96;22.98
001011;186.58;93.29;46.65;23.32
001100;189.34;94.67;47.33;23.67
001101;192.1;96.05;48.02;24.01
001110;194.85;97.43;48.71;24.36
001111;197.61;98.81;49.4;24.7
010000;-;100.18;50.09;25.05
010001;-;101.56;50.78;25.39
010010;-;102.94;51.47;25.74
010011;-;104.32;52.16;26.08
010100;-;105.7;52.85;26.42
010101;-;107.08;53.54;26.77
010110;-;108.46;54.23;27.11
010111;-;109.83;54.92;27.46
011000;-;111.21;55.61;27.8
011001;-;112.59;56.3;28.15
011010;-;113.97;56.99;28.49
011011;-;115.35;57.67;28.84
011100;-;116.73;58.36;29.18
011101;-;118.11;59.05;29.53
011110;-;119.49;59.74;29.87
011111;-;120.86;60.43;30.22
100000;153.49;76.75;38.37;-
100001;150.74;75.37;37.68;-
100010;147.98;73.99;36.99;-
100011;145.22;72.61;36.31;-
100100;142.46;71.23;35.62;-
100101;139.71;69.85;34.93;-
100110;136.95;68.47;34.24;-
100111;134.19;67.1;33.55;-
101000;131.43;65.72;32.86;-
101001;128.68;64.34;32.17;-
101010;125.92;62.96;31.48;-
101011;123.16;61.58;30.79;-
101100;120.4;60.2;30.1;-
101101;117.65;58.82;29.41;-
101110;114.89;57.44;28.72;-
101111;112.13;56.07;28.03;-
110000;109.38;54.69;27.34;-
110001;106.62;53.31;26.65;-
110010;103.86;51.93;25.97;-
110011;101.1;50.55;25.28;-
110100;98.35;49.17;24.59;-
110101;95.59;47.79;23.9;-
110110;92.83;46.42;23.21;-
110111;90.07;45.04;22.52;-
111000;87.32;43.66;21.83;-
111001;84.56;42.28;21.14;-
111010;81.8;40.9;20.45;-
111011;79.04;39.52;-;-
111100;76.29;38.14;-;-
111101;73.53;36.76;-;-
111110;70.77;35.39;-;-
111111;68.01;34.01;-;-
"""

def frequencyStringToEnum(valueString):
    before, after = valueString.split('.')
    modifiedString = 'frequency{paddingBefore}{before}Hz{after}{paddingAfter}'.format(paddingBefore='0' * (3 - len(before)),
                                                                             before=before,
                                                                             after=after,
                                                                             paddingAfter='0' * (2 - len(after)))
    return modifiedString

if __name__ == '__main__':
    f = StringIO(csvValues)
    reader = csv.reader(f, delimiter=';')

    headerRow = next(reader, None)
    assert headerRow == ['SEL_F', '00', '01', '10', '11'], "Currently this is the only format supported"

    valueDictionary = {}
    for row in reader:
        SEL_F = row[0]
        for index in range(1, 5):
            frequency = row[index]
            if '-' != frequency:
                binaryValueString = '0b{SEL_DIV}{SEL_F}'.format(SEL_DIV=headerRow[index], SEL_F=SEL_F)
                if frequency in valueDictionary:
                    raise ValueError('Duplicate frequency [{}].'.format(frequency))
                valueDictionary[frequency] = binaryValueString
    del SEL_F, frequency, binaryValueString

    enumValueList = []
    for key, value in sorted(valueDictionary.items(), key=lambda item: float(item[0])):
        enumString = frequencyStringToEnum(key)
        enumValueList.append((enumString, value))

    enumClassName = 'Frequency'

    print('enum class {enumClassName}\n{{'.format(enumClassName=enumClassName))
    for enumString, value in enumValueList:
        print('    {enumString},'.format(enumString=enumString))
    print('};')

    print('')

    print('constexpr uint8_t frequencyToInternal({enumClassName} const frequency)'.format(enumClassName=enumClassName))
    print('{')
    print('    switch (frequency)\n    {')
    for enumString, value in enumValueList:
        print('        case {enumClassName}::{enumString}: return {value};'.format(
            enumClassName=enumClassName, enumString=enumString, value=value))
    print('    }')
    print('}')

    exit(0)
