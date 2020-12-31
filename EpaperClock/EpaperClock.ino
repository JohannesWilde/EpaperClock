/* -*- C++ -*- Mark this as a c++ file for QtCreator [Preferences->Environment->MIME Types->text/x-c++src]. */

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
    void(* resetFunc) (void) = 0; // declare reset function at address 0
	resetFunc(); //call reset  
}

