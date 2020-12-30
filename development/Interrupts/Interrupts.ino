#include <avr/sleep.h>
#include <avr/interrupt.h>

constexpr byte LED = 13;
constexpr byte BUTTON = 3;
uint8_t constexpr keepAlivePin = 2;

static volatile bool externalInterrupt = false;

ISR (TIMER2_COMPA_vect) 
{
  // Timer 2 has reached its comparison value
    PINB |= (1 << PINB5); // only toggle D13 = PORTB5 for oscilloscope measurement
}

// Interrupt Service Routine (ISR)
void switchPressed ()
{
  externalInterrupt = true;
}  // end of switchPressed

void powerDown()
{
  sleep_enable();
  sleep_mode ();            // here the device is actually put to sleep!!
  sleep_disable();
}

void setup ()
{
  pinMode (LED, OUTPUT);  // so we can update the LED
  digitalWrite (BUTTON, HIGH);  // internal pull-up resistor
  attachInterrupt (digitalPinToInterrupt (BUTTON), switchPressed, CHANGE);  // attach interrupt handler

  // this reconfigures TIMER2 - thus PWM [analogWrite] will not work as expected on Pins 3 and 11
  TCCR2A = 0b00000010; // COM2A1, COM2A0, COM2B1, COM2B0, 0, 0, WGM21, WGM20: no output, CTC-mode
  TCCR2B = 0b00000111; // FOC2A, FOC2B, 0, 0, WGM22, CS22, CS21, CS20: 1024 prescaler
  OCR2A = 49; // Output compare register [49 * 1024 @ 1MHz = 50176us]
  TIMSK2 = 0b00000010; // 0, 0, 0, 0, 0, OCIE2B, OCIE2A, TOIE2: Output Compare Match Interrupt Enable Timer2 A
  
  set_sleep_mode (SLEEP_MODE_PWR_SAVE);
  
  sei();
}  // end of setup

void loop ()
{    
  powerDown();

  if (externalInterrupt)
  {
    externalInterrupt = false;
    pinMode(keepAlivePin, OUTPUT);
    delay(500);                       // wait for a second
    pinMode(keepAlivePin, INPUT);
    delay(500);
  }
  else
  {    
//    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
//    delay(500);                       // wait for a second
//    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//    delay(500);                       // wait for a second
  }
  
}
