#include <ArduinoDrivers/button.hpp>
#include <ArduinoDrivers/buttonTimedProperties.hpp>
#include <ArduinoDrivers/simplePinBit.hpp>


static constexpr SimplePin::State downState = SimplePin::State::Zero;
static constexpr SimplePin::State upState = SimplePin::State::One;

static uint8_t buttonsMemory[1] = {0, };

typedef SimplePinBit</*bit*/ 0,
                     /*byte*/ buttonsMemory,
                     /*byteOffset*/ 0,
                     /*StateInit*/ SimplePin::State::One,
                     /*StateDeinit*/ SimplePin::State::One> simplePin0;
typedef SimplePinBit</*bit*/ 1,
                     /*byte*/ buttonsMemory,
                     /*byteOffset*/ 0,
                     /*StateInit*/ SimplePin::State::One,
                     /*StateDeinit*/ SimplePin::State::One> simplePin1;
typedef SimplePinBit</*bit*/ 2,
                     /*byte*/ buttonsMemory,
                     /*byteOffset*/ 0,
                     /*StateInit*/ SimplePin::State::One,
                     /*StateDeinit*/ SimplePin::State::One> simplePin2;
typedef SimplePinBit</*bit*/ 3,
                     /*byte*/ buttonsMemory,
                     /*byteOffset*/ 0,
                     /*StateInit*/ SimplePin::State::One,
                     /*StateDeinit*/ SimplePin::State::One> simplePin3;

typedef Button<simplePin0, downState> button0;
typedef Button<simplePin1, downState> button1;
typedef Button<simplePin2, downState> button2;
typedef Button<simplePin3, downState> button3;
