// ----------------------------------------------------------------------------------------------------

#include <avr/sleep.h>
#include <avr/interrupt.h>

#include <Arduino.h>
#include <Wire.h>

#include <DS3231.h>

#include "ArduinoDrivers/arduinoUno.hpp"
#include "ArduinoDrivers/buttonTimed.hpp"
#include "ArduinoDrivers/buttonTimedCache.hpp"
#include "ArduinoDrivers/driverHelper.hpp"
#include "ArduinoDrivers/powerbankKeepAlive.hpp"

// ----------------------------------------------------------------------------------------------------

ButtonTimedProperties::Duration_t constexpr keyPressDurationShort = 1; // currently in 50ms steps [-> Timer2 interrupts]
ButtonTimedProperties::Duration_t constexpr keyPressDurationLong = 20;

typedef PowerbankKeepAlive</*AvrPin*/ ArduinoUno::D2, /*DurationActive*/ 1, /*DurationInactive*/ 199> PowerbankKeepAlive0;

typedef ButtonTimed</*AvrPin*/ ArduinoUno::D5, /*PinDownState*/ AvrInputOutput::PinState::Low, /*PullupEnabled*/ true,
                    /*DurationShort*/ keyPressDurationShort, /*DurationLong*/ keyPressDurationLong> Key1;
typedef ButtonTimed</*AvrPin*/ ArduinoUno::D6, /*PinDownState*/ AvrInputOutput::PinState::Low, /*PullupEnabled*/ true,
                    /*DurationShort*/ keyPressDurationShort, /*DurationLong*/ keyPressDurationLong> Key2;
typedef ButtonTimed</*AvrPin*/ ArduinoUno::D4, /*PinDownState*/ AvrInputOutput::PinState::Low, /*PullupEnabled*/ true,
                    /*DurationShort*/ keyPressDurationShort, /*DurationLong*/ keyPressDurationLong> Key3;
typedef ButtonTimed</*AvrPin*/ ArduinoUno::D3, /*PinDownState*/ AvrInputOutput::PinState::Low, /*PullupEnabled*/ true,
                    /*DurationShort*/ keyPressDurationShort, /*DurationLong*/ keyPressDurationLong> Key4;

typedef ButtonTimedCache</*ButtonTimed*/ Key1> CachedKey1;
typedef ButtonTimedCache</*ButtonTimed*/ Key2> CachedKey2;
typedef ButtonTimedCache</*ButtonTimed*/ Key3> CachedKey3;
typedef ButtonTimedCache</*ButtonTimed*/ Key4> CachedKey4;

static DS3231 realTimeClock;

// ----------------------------------------------------------------------------------------------------

// Interrupt Service Routine for when Timer2 matches OCR2A.
ISR (TIMER2_COMPA_vect)
{
    PowerbankKeepAlive0::update();
    Key1::update();
    Key2::update();
    Key3::update();
    Key4::update();
}

/**
 * @brief powerDown puts the microcontroller in SLEEP_MODE_PWR_SAVE.
 * Please note that Timer0 won't run while sleeping, thus millis()
 * will be effectively stopped. Therefore do not rely on differences
 * of millis() before and after a call to powerDown()!
 */
void powerDown()
{
    sleep_mode (); // here the device is actually put to sleep!!
}

// ----------------------------------------------------------------------------------------------------

// the setup function runs once when you press reset or power the board
void setup()
{
    DriverDeInitializer<PowerbankKeepAlive0> driverDeInitializerPowerbankKeepAlive0;

    DriverDeInitializer<Key1> driverDeInitializerKey1;
    DriverDeInitializer<Key2> driverDeInitializerKey2;
    DriverDeInitializer<Key3> driverDeInitializerKey3;
    DriverDeInitializer<Key4> driverDeInitializerKey4;

    DriverDeInitializer<CachedKey1> driverDeInitializerCachedKey1;
    DriverDeInitializer<CachedKey2> driverDeInitializerCachedKey2;
    DriverDeInitializer<CachedKey3> driverDeInitializerCachedKey3;
    DriverDeInitializer<CachedKey4> driverDeInitializerCachedKey4;

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

    set_sleep_mode(SLEEP_MODE_PWR_SAVE);


    realTimeClock.setClockMode(/*h12*/ false); // @Todo: store this in EEPROM or assume DS3231 saves this?

    // ----------------------------------------------------------------------------------------------------

    while (true)
    {
        cli(); // disable interrupts
        // update locally cashed keys
        CachedKey1::update();
        CachedKey2::update();
        CachedKey3::update();
        CachedKey4::update();
        sei(); // enable interrupts



        powerDown();
    }
}





















// ---------------------------------------------------------------------------

void loop() 
{
	// this shall not be reached - if however, then reset here.
    void(* resetFunc) (void) = 0; // declare reset function at address 0
	resetFunc(); //call reset  
}

