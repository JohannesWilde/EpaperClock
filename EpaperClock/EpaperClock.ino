/* -*- C++ -*- Mark this as a c++ file for QtCreator [Preferences->Environment->MIME Types->text/x-c++src]. */

#include <Arduino.h>

#include "ArduinoDrivers/arduinoUno.hpp"

typedef ArduinoUno arduinoUno;

uint8_t constexpr keepAlivePin = 2;


// the setup function runs once when you press reset or power the board
void setup()
{
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);

    digitalWrite(keepAlivePin, LOW);   // turn the LED on (HIGH is the voltage level)
    pinMode(keepAlivePin, INPUT);
	
    while (true)
    {
        digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        pinMode(keepAlivePin, OUTPUT);
        delay(50);                       // wait for a second
        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        pinMode(keepAlivePin, INPUT);
        delay(5000-50);                       // wait for a second
    }
}













// ---------------------------------------------------------------------------

void loop() 
{
	// this shall not be reached - if however, then reset here.
    void(* resetFunc) (void) = 0; // declare reset function at address 0
	resetFunc(); //call reset  
}

