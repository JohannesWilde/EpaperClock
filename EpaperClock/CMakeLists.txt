# avoid compile check for cross-compile targets
# https://stackoverflow.com/questions/53633705/cmake-the-c-compiler-is-not-able-to-compile-a-simple-test-program
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

cmake_minimum_required(VERSION 3.19)

# This is a dummy project, as to have convenient code highlighting in the QtCreator IDE.
# This won't actually compile with it as of now.
# For compilation use the Arduino IDE [necessary adaptations are referenced in README.md].
project(EpaperClockDummy)


# For correct highlighting in QtCreator check Preferences->Environment->MIME Types->text/x-c++src to include "*.ino" in Patterns.
add_executable(${PROJECT_NAME}
    main.cpp
    EpaperClock.ino
    )
   
target_include_directories(${PROJECT_NAME}
    PRIVATE "c:\\Program Files (x86)\\Arduino\\hardware\\arduino\\avr\\cores\\arduino\\"
	PRIVATE "c:\\Program Files (x86)\\Arduino\\hardware\\arduino\\avr\\variants\\standard\\"
    )
	
target_compile_definitions(${PROJECT_NAME}
    PRIVATE __AVR_ATmega328P__
	PRIVATE F_CPU 1000000
    )