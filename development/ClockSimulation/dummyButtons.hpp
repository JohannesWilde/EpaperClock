#ifndef DUMMYBUTTONS_HPP
#define DUMMYBUTTONS_HPP

#include <ArduinoDrivers/buttonDynamic.hpp>
#include <ArduinoDrivers/buttonTimedMultiple.hpp>
#include <ArduinoDrivers/simplePinDynamic.hpp>


void simplePinInitialize(void const * const instance);
SimplePin::State getSimplePinIsDown(void const * const instance);
void setSimplePinIsDown(void const * const instance, SimplePin::State const state);
void simplePinDeinitialize(void const * const instance);

typedef SimplePinDynamic<&simplePinInitialize,
                         &getSimplePinIsDown,
                         &setSimplePinIsDown,
                         &simplePinDeinitialize> CustomSimplePin;

extern CustomSimplePin pin0;
extern CustomSimplePin pin1;
extern CustomSimplePin pin2;
extern CustomSimplePin pin3;


constexpr ButtonTimedProperties::Duration_t durationShort = 1; // 50ms per cycle
constexpr ButtonTimedProperties::Duration_t durationLong = 6;
constexpr ButtonTimedProperties::Duration_t durationCombineMax = 4;

void buttonInitialize(void const * const instance);
void buttonDeinitialize(void const * const instance);
bool buttonIsDown(void const * const instance);

typedef ButtonDynamic<&buttonInitialize, &buttonIsDown, &buttonDeinitialize> CustomButton;
typedef ButtonTimedMultiple<CustomButton, durationShort, durationLong, durationCombineMax, 5> CustomButtomTimedMultiple;


extern CustomButtomTimedMultiple buttonTimedMultiple0;
extern CustomButtomTimedMultiple buttonTimedMultiple1;
extern CustomButtomTimedMultiple buttonTimedMultiple2;
extern CustomButtomTimedMultiple buttonTimedMultiple3;



struct PinAndButton
{
    CustomSimplePin & pin;
    CustomButtomTimedMultiple & button;

    PinAndButton(CustomSimplePin & pin, CustomButtomTimedMultiple & button)
        : pin(pin)
        , button(button)
    {
        // intentionally empty
    }
};


#endif // DUMMYBUTTONS_HPP
