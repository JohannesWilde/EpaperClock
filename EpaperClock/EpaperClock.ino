// this is only for the QtCreator to find the correct highlighting - this is already defined somewhere before
#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include "ArduinoDrivers/arduinoUno.hpp"

typedef ArduinoUno arduinoUno;


// the setup function runs once when you press reset or power the board
void setup()
{
	
    while (true)
    {
        
    }
}


void loop() 
{
	// this shall not be reached - if however, then reset here.
    void(* resetFunc) (void) = 0;//declare reset function at address 0
	resetFunc(); //call reset  
}

