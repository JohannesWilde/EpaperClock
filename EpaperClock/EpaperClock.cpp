// ----------------------------------------------------------------------------------------------------

#include <avr/sleep.h>
#include <avr/interrupt.h>

#include <Arduino.h>

#include <DS3231.h>

#include "src/WaveshareEpaper/epd2in7.h"
#include "src/WaveshareEpaper/epdpaint.h"
#include "src/WaveshareEpaper/imagedata.h"

#include "ArduinoDrivers/arduinoUno.hpp"
#include "ArduinoDrivers/buttonTimed.hpp"
#include "ArduinoDrivers/buttonTimedCache.hpp"
#include "ArduinoDrivers/driverHelper.hpp"
#include "ArduinoDrivers/powerbankKeepAlive.hpp"


// ----------------------------------------------------------------------------------------------------

// One will have to connect D1 to the Keep-Alive Test LED for this to have any visuable effect.
typedef ArduinoUno::D1 DEBUG_LED;

// for debugging purposes
void showOnInternalLed(uint8_t const currentShowtime, uint8_t const totalShowtime)
{
    uint16_t constexpr halfPeriodTimeMs = 250;

    uint8_t currentNotShowTime = totalShowtime - currentShowtime;

    for (uint8_t index = 0; index < currentShowtime; ++index)
    {
        DEBUG_LED::setType(AvrInputOutput::PinType::OutputLow);
        delay(halfPeriodTimeMs);
        DEBUG_LED::setType(AvrInputOutput::PinType::Input);
        delay(halfPeriodTimeMs);
    }

    // wait rest
    delay((halfPeriodTimeMs * 2) * currentNotShowTime);
}

// ----------------------------------------------------------------------------------------------------

namespace ePaperInterface
{
    int constexpr Success = 0;
    int constexpr Colored = 0;
    int constexpr Uncolored = 1;
}


enum DisplayMode
{
    ModeClock,
    _ModesCount // number of modes - not a mode itself
};

enum DisplayUpdateMode
{
    IsUpdated,
    RequiresFullUpdate,
    RequiresPartialUpdate,
};

// ----------------------------------------------------------------------------------------------------

ButtonTimedProperties::Duration_t constexpr keyPressDurationShort = 1; // currently in 50ms steps [-> Timer2 interrupts]
ButtonTimedProperties::Duration_t constexpr keyPressDurationLong = 20;

typedef PowerbankKeepAlive</*AvrPin*/ ArduinoUno::D2, /*DurationActive*/ 1, /*DurationInactive*/ 19> PowerbankKeepAlive0;

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

static Epd ePaperDisplay;
/**
    Due to not enough RAM in Arduino UNO, a full frame buffer is not
    possible.
    Therefore a smaller image buffer is allocated and you have to
    update the display partially.
    1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
*/
static unsigned char frameBuffer[1024];
Paint paintFrame(/*image*/ frameBuffer, /*width*/ 176, /*height*/ 24); // width should be a multiple of 8

static DisplayMode displayMode;
static DisplayUpdateMode displayUpdateMode;

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

/**
 * @brief powerOff puts the microcontroller in SLEEP_MODE_PWR_DOWN.
 * This is only supposed to be used in error cases, as in this
 * mode nothing can wake up the microcontroller apart from a
 * reset [Timer2 is stopped as well -> no interrupts anymore].
 */
void powerOff()
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode (); // here the device is actually put to sleep!!
}

void returnFailure()
{
    cli(); // disable interrupts
    // long-pressing powerbank button turns it off for me
    PowerbankKeepAlive0::Pin::setType(AvrInputOutput::PinType::OutputLow);
    powerOff();
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

//    paintFrame.SetRotate(ROTATE_270);
    if (ePaperInterface::Success != ePaperDisplay.Init())
    {
        returnFailure();
    }
    ePaperDisplay.ClearFrame();

//    realTimeClock.setClockMode(/*h12*/ false); // Set to 24-hour clock mode. @Todo: store this in EEPROM or assume DS3231 saves this?

    displayMode = DisplayMode::ModeClock;
    displayUpdateMode = DisplayUpdateMode::RequiresFullUpdate;


    // ----------------------------------------------------------------------------------------------------

    while (true)
    {
        cli(); // disable interrupts
        // update locally cashed keys - please note, that the cashing
        // currently does not remember all things happened between two
        // update() calls. Instead it corrects the long/short/tooShort
        // info with the most recent info, as well as the isDown/isUp.
        // This however means that whole button presses will be lost, in
        // case this while-iteration takes too long.
        CachedKey1::update();
        CachedKey2::update();
        CachedKey3::update();
        CachedKey4::update();
        sei(); // enable interrupts


        switch (displayMode)
        {
        case DisplayMode::ModeClock:
        {

            if (DisplayUpdateMode::RequiresFullUpdate == displayUpdateMode)
            {
                paintFrame.Clear(ePaperInterface::Colored);
                paintFrame.DrawStringAt(20, 5, "Hello world!", &Font16, ePaperInterface::Uncolored);
                ePaperDisplay.TransmitPartialData(paintFrame.GetImage(),
                                                  0, 64,
                                                  paintFrame.GetWidth(), paintFrame.GetHeight());
                ePaperDisplay.DisplayFrame();

                displayUpdateMode = DisplayUpdateMode::IsUpdated;
            }

            break;
        }
        case DisplayMode::_ModesCount:
        {
            returnFailure();
            break;
        }
        }


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

