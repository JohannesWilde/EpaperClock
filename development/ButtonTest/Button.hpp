#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <Arduino.h>

struct ButtonState
{
    ButtonState(bool const isDown, bool const pressed, bool const released)
        : isDown(isDown)
        , pressed(pressed)
        , released(released)
    {
    }

    ButtonState(ButtonState const & other)
        : isDown(other.isDown)
        , pressed(other.pressed)
        , released(other.released)
    {
    }

    bool isDown : 1;
    bool pressed : 1;
    bool released : 1;
}  __attribute__((packed));


template <int pinNumber, int buttonPressedValue>
class Button
{
public:

    Button()
    {
        pinMode(pinNumber, INPUT_PULLUP);
        wasDownPreviously_ = isDown_();
    }

    ~Button()
    {
        pinMode(pinNumber, INPUT);
    }

    /**
   * isDown - button is currently being pressed down.
   */
    bool isDown()
    {
        bool const isDownNow = isDown_();
        wasDownPreviously_ = isDownNow;
        return isDownNow;
    }

    /**
   * pressed - button was pressed.
   */
    bool pressed()
    {
        bool const isDownNow = isDown_();
        bool const wasPressed = pressed_(isDownNow, wasDownPreviously_);
        wasDownPreviously_ = isDownNow;
        return wasPressed;
    }

    /**
   * released - button was released.
   */
    bool released()
    {
        bool const isDownNow = isDown_();
        bool const wasReleased = released_(isDownNow, wasDownPreviously_);
        wasDownPreviously_ = isDownNow;
        return wasReleased;
    }

    /**
     * @brief getState Return the current state of the button.
     * @return ButtonState instance.
     */
    ButtonState getState()
    {
        bool const isDownNow = isDown_();
        ButtonState const buttonState(isDownNow,
                                      pressed_(isDownNow, wasDownPreviously_),
                                      released_(isDownNow, wasDownPreviously_));
        wasDownPreviously_ = isDownNow;
        return buttonState;
    }

protected:

    bool getWasDownPreviously_()
    {
        return wasDownPreviously_;
    }

private:
    bool wasDownPreviously_;

    bool isDown_()
    {
        return (buttonPressedValue == digitalRead(pinNumber));
    }

    static bool pressed_(bool const isDownNow, bool const wasDownPreviously)
    {
        return (!wasDownPreviously && isDownNow);
    }

    static bool released_(bool const isDownNow, bool const wasDownPreviously)
    {
        return (wasDownPreviously && !isDownNow);
    }
};

#endif // BUTTON_HPP
