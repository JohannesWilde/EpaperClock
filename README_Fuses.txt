Under Windows use WinAVR [install WinAVR and Git SCM [with MinTTY] and in MinTTY execute

PATH=/c/WinAVR/utils/bin:/c/WinAVR/bin:${PATH}

].

Under Linux use avrdude directly.

Setup the fuses for the Atmega328P as follows [0 - programmed, 1 - unprogrammed, big-endian Bit-order]:

[https://www.engbedded.com/fusecalc/]


avrdude.exe -P COM5 -b 19200 -c avrisp -p atmega328p -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h

# 1MHz, no bootloader, no EEPROM-erase, no brown-out-detection
avrdude.exe -P COM5 -b 19200 -c avrisp -p atmega328p -U lfuse:w:0x62:m -U hfuse:w:0xd7:m -U efuse:w:0x07:m

l: 62
h: d7
e: 07



Arduino Uno defaults [16MHz, bootloader, EEPROM-erase, brown-out-detection 2.7V]:

avrdude.exe -P COM5 -b 19200 -c avrisp -p ATtiny85 -U lfuse:w:0xff:m -U hfuse:w:0xde:m -U efuse:w:0x05:m

l: ff
h: de
e: 05








I added the following to this file:

c:\Users\User\AppData\Local\Arduino15\packages\arduino\hardware\avr\1.8.5\boards.txt

This lets one compile code for an atmega328p at 1MHz. Please note however, that the programming of the fuses has to be done manually.


##############################################################
################ custom addition by J.W. #####################

unoOneMhz.name=Arduino Uno (1MHz)

unoOneMhz.vid.0=0x2341
unoOneMhz.pid.0=0x0043
unoOneMhz.vid.1=0x2341
unoOneMhz.pid.1=0x0001
unoOneMhz.vid.2=0x2A03
unoOneMhz.pid.2=0x0043
unoOneMhz.vid.3=0x2341
unoOneMhz.pid.3=0x0243

unoOneMhz.upload.tool=avrdude
unoOneMhz.upload.tool.default=avrdude
unoOneMhz.upload.tool.network=arduino_ota
unoOneMhz.upload.protocol=arduino
unoOneMhz.upload.maximum_size=32256
unoOneMhz.upload.maximum_data_size=2048
unoOneMhz.upload.speed=115200

# keep the original fuses [16MHz], as to not unintendetly render the programming arduino unprogrammable [the bootloader here expects 16MHz!].
# unoOneMhz.bootloader.tool=avrdude
# unoOneMhz.bootloader.low_fuses=0x62
# unoOneMhz.bootloader.high_fuses=0xD7
# unoOneMhz.bootloader.extended_fuses=0x07
# unoOneMhz.bootloader.unlock_bits=0x3F
# unoOneMhz.bootloader.lock_bits=0x0F
# unoOneMhz.bootloader.file="no bootloader on this configuration"

unoOneMhz.build.mcu=atmega328p
unoOneMhz.build.f_cpu=1000000L
unoOneMhz.build.board=AVR_UNO
unoOneMhz.build.core=arduino
unoOneMhz.build.variant=standard

################ custom addition by J.W. #####################
##############################################################
