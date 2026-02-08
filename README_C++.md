I required at least C++ 14 for constexpr functions consisting not just of a return statement.

The current Arduino IDE 2.0.1 ships with an avr-g++ of version 7.5, which should support C++ 17 [see [https://gcc.gnu.org/projects/cxx-status.html#cxx17](https://gcc.gnu.org/projects/cxx-status.html#cxx17)].

I the following line in the following file:

c:\Users\User\AppData\Local\Arduino15\packages\arduino\hardware\avr\1.8.6\platform.txt

````{verbatim}
compiler.cpp.flags=-c -g -Os {compiler.warning_flags} -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto
````

to

````{verbatim}
compiler.cpp.flags=-c -g -Os {compiler.warning_flags} -std=gnu++17 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto
````

i.e. simply replaced gnu++11 with gnu++17.

Please note that I needed to restart the Arduino IDE for this to take effect.
