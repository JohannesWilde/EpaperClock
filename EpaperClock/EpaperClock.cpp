/* -*- C++ -*- Mark this as a c++ file for QtCreator [Preferences->Environment->MIME Types->text/x-c++src]. */

#include <Arduino.h>

#include <DS3231.h>

#include "ArduinoDrivers/arduinoUno.hpp"
#include "ArduinoDrivers/powerbankKeepAlive.hpp"

static PowerbankKeepAlive</*AvrPin*/ ArduinoUno::D2, /*DurationActive*/ 1, /*DurationInactive*/ 199> powerbankKeepAlive;


// the setup function runs once when you press reset or power the board
void setup()
{
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);
	
    while (true)
    {
        powerbankKeepAlive.update();
        delay(50);
    }
}





















// ---------------------------------------------------------------------------

void loop() 
{
	// this shall not be reached - if however, then reset here.
    void(* resetFunc) (void) = 0; // declare reset function at address 0
	resetFunc(); //call reset  
}

