# EpaperClock

Simple Clock using a DS3231 Real Time Clock [RTC] and a Waveshare 2.7" ePaper HAT.

In addition it shall be powered via a Powerbank - as the current required by this project however is so low, the powerbank keeps turning off after approximately 15 seconds. Therefore the power button on the powerbank is periodically pressed via software [by pulling the contact on the actual button low with an OUTPUT, LOW pin and releasing it afterwards via INPUT, LOW]. Please not that this requires a harware modification in the powerbank in order to be able to access the pin.
The released pin is mimiced via INPUT,LOW instead of OUTPUT,HIGH intentionally, as the powerbank chip runs at 3.3V - this project however currently at 5.0V.

Please note that for this to compile correctly one will have to adapt some files in one's Arduino installation. This is necessary as I wanted to run the Chip at 1MHz [please refer to [README_Fuses.md](README_Fuses.md) how to change F_CPU to 1000000 conveniently in the Arduino IDE] and employ a Meta Template Programming styled approach for changing the Pins [please refer to EpaperClock\ArduinoDrivers\avrSrc\avrHeaderConverter.py].

Also I needed to bump up the supported C++ version to C++ 17 [see [README_C++.md](README_C++.md) for more details].

Please note, that this is not finished and the development is still in its starting phase.
