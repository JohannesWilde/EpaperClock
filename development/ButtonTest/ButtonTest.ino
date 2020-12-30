/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/


#include "ButtonTimed.hpp"

Button<5, LOW> key1;
Button<6, LOW> key2;
Button<4, LOW> key3;
Button<3, LOW> key4;


// the setup function runs once when you press reset or power the board
void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() 
{
    uint8_t constexpr totalShowtime = 20;
    uint8_t currentShowtime = 0;
    uint16_t constexpr halfPeriodTimeMs = 250;

    {
        bool const isDown = key1.isDown();
        if (isDown)
        {
            currentShowtime |= (0x01 << 0);
        }
    }

    {
        bool const isDown = key2.isDown();
        if (isDown)
        {
            currentShowtime |= (0x01 << 1);
        }
    }

    {
        bool const isDown = key3.isDown();
        if (isDown)
        {
            currentShowtime |= (0x01 << 2);
        }
    }

    {
        bool const isDown = key4.isDown();
        if (isDown)
        {
            currentShowtime |= (0x01 << 3);
        }
    }

    uint8_t currentNotShowTime = totalShowtime - currentShowtime;


    for (uint8_t index = 0; index < currentShowtime; ++index)
    {
        digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(halfPeriodTimeMs);                       // wait for a second
        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        delay(halfPeriodTimeMs);
    }

    // wait rest
    delay((halfPeriodTimeMs * 2) * currentNotShowTime);
    
}
