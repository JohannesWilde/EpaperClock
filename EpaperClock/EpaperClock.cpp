
#include <avr/sleep.h>
#include <avr/interrupt.h>

#include <Arduino.h>

#include <DS3231.h>

#include "ArduinoDrivers/arduinoUno.hpp"
#include "ArduinoDrivers/powerbankKeepAlive.hpp"

typedef PowerbankKeepAlive</*AvrPin*/ ArduinoUno::D2, /*DurationActive*/ 1, /*DurationInactive*/ 199> PowerbankKeepAlive0;

ISR (TIMER2_COMPA_vect)
{
    PowerbankKeepAlive0::update();
}

void powerDown()
{
    sleep_mode (); // here the device is actually put to sleep!!
}

// the setup function runs once when you press reset or power the board
void setup()
{
    PowerbankKeepAlive0::initialize();

    cli(); // disable interrupts

    // this reconfigures TIMER2 - thus PWM [analogWrite] will not work as expected on Pins 3 and 11
    TCCR2A = 0b00000010; // COM2A1, COM2A0, COM2B1, COM2B0, 0, 0, WGM21, WGM20: no output, CTC-mode
    TCCR2B = 0b00000111; // FOC2A, FOC2B, 0, 0, WGM22, CS22, CS21, CS20: 1024 prescaler
    OCR2A = 49; // Output compare register [49 * 1024 @ 1MHz = 50176us]
    TIMSK2 = 0b00000010; // 0, 0, 0, 0, 0, OCIE2B, OCIE2A, TOIE2: Output Compare Match Interrupt Enable Timer2 A

    // disable ADC conversions -> analogRead non-usable
    ADCSRA = 0b00010000; // ADC Control and Status Register A: ADEN, ADSC, ADATE, ADIF, ADIE, ADPS2, ADPS1, ADPS0: ADC disable, ADIF cleared, ADC interrupts disabled

    // disable Analog Comparator -> everything regarding analog measurements non-usable
    ACSR = 0b10010000; // Analog Comparator Control and Statur Register: ACD, ACBG, ACO, ACI, ACIE, ACIC, ACIS1, ACIS0: AC disabled, ACI cleared, AC interrupts disabled

    sei(); // enable interrupts

    set_sleep_mode (SLEEP_MODE_PWR_SAVE);

    while (true)
    {
        powerDown();
    }


    PowerbankKeepAlive0::deinitialize();
}





















// ---------------------------------------------------------------------------

void loop() 
{
	// this shall not be reached - if however, then reset here.
    void(* resetFunc) (void) = 0; // declare reset function at address 0
	resetFunc(); //call reset  
}

