#ifndef BUTTON_TIMED_HPP
#define BUTTON_TIMED_HPP

#include <Arduino.h>
#include "Button.hpp"


struct ButtonTimedState : public ButtonState
{
    ButtonTimedState(bool const isDown,
                     bool const pressed,
                     bool const released,
                     bool const longDuration,
                     bool const longDurationPressed,
                     bool const longDurationReleased)
        : ButtonState(isDown, pressed, released)
        , longDuration(longDuration)
        , longDurationPressed(longDurationPressed)
        , longDurationReleased(longDurationReleased)
    {
    }

    ButtonTimedState(ButtonState const & buttonState,
                     bool const longDuration,
                     bool const longDurationPressed,
                     bool const longDurationReleased)
        : ButtonState(buttonState)
        , longDuration(longDuration)
        , longDurationPressed(longDurationPressed)
        , longDurationReleased(longDurationReleased)
    {
    }

    ButtonTimedState(ButtonTimedState const & other)
        : ButtonState(other.isDown, other.pressed, other.released)
        , longDuration(other.longDuration)
        , longDurationPressed(other.longDurationPressed)
        , longDurationReleased(other.longDurationReleased)
    {
    }

    bool longDuration : 1;
    bool longDurationPressed : 1;
    bool longDurationReleased : 1;

} __attribute__((packed));


template <int pinNumber, int buttonPressedValue>
class ButtonTimed : public Button<pinNumber, buttonPressedValue>
{
public:
    typedef unsigned long TimeMs_t;

    ButtonTimed(TimeMs_t const & currentTimeMs)
        : lastTimeToggledMs_(currentTimeMs)
    {
    }

    ~ButtonTimed()
    {
    }

    /**
   * isDown - button is currently being pressed down.
   */
    bool isDown(TimeMs_t const & currentTimeMs)
    {
        bool const wasDownPreviouslyBackup = Button<pinNumber, buttonPressedValue>::getWasDownPreviously_();
        bool const isDownNow = Button<pinNumber, buttonPressedValue>::isDown();
        // Button<pinNumber, buttonPressedValue>::wasDownPreviously_ now holds the isDownNow value
        potentiallyUpdateLastTimeToggled_(wasDownPreviouslyBackup, Button<pinNumber, buttonPressedValue>::getWasDownPreviously_(), currentTimeMs);
        return isDownNow;
    }

    /**
   * pressed - button was pressed.
   */
    bool pressed(TimeMs_t const & currentTimeMs)
    {
        bool const wasDownPreviouslyBackup = Button<pinNumber, buttonPressedValue>::getWasDownPreviously_();
        bool const isPressed = Button<pinNumber, buttonPressedValue>::pressed();
        // Button<pinNumber, buttonPressedValue>::wasDownPreviously_ now holds the isDownNow value
        potentiallyUpdateLastTimeToggled_(wasDownPreviouslyBackup, Button<pinNumber, buttonPressedValue>::getWasDownPreviously_(), currentTimeMs);
        return isPressed;
    }

    /**
   * released - button was released.
   */
    bool released(TimeMs_t const & currentTimeMs)
    {
        bool const wasDownPreviouslyBackup = Button<pinNumber, buttonPressedValue>::getWasDownPreviously_();
        bool const wasReleased = Button<pinNumber, buttonPressedValue>::released();
        // Button<pinNumber, buttonPressedValue>::wasDownPreviously_ now holds the isDownNow value
        potentiallyUpdateLastTimeToggled_(wasDownPreviouslyBackup, Button<pinNumber, buttonPressedValue>::getWasDownPreviously_(), currentTimeMs);
        return wasReleased;
    }

    /**
     * @brief getState Return the current state of the button.
     * @return ButtonTimedState instance.
     */
    ButtonTimedState getState(TimeMs_t const & currentTimeMs)
    {
        bool const wasDownPreviouslyBackup = Button<pinNumber, buttonPressedValue>::getWasDownPreviously_();
        ButtonState const buttonState = Button<pinNumber, buttonPressedValue>::getState();
        //
        bool const wasLongDurationPreviously = isLongDuration(currentTimeMs, lastTimeToggledMs_);
        // Button<pinNumber, buttonPressedValue>::wasDownPreviously_ now holds the isDownNow value
        potentiallyUpdateLastTimeToggled_(wasDownPreviouslyBackup, Button<pinNumber, buttonPressedValue>::getWasDownPreviously_(), currentTimeMs);
        ButtonTimedState const buttonTimedState(buttonState,
                                                isLongDuration(currentTimeMs, lastTimeToggledMs_),
                                                wasLongDurationPreviously && buttonState.pressed,
                                                wasLongDurationPreviously && buttonState.released);
        return buttonTimedState;
    }

private:

    void potentiallyUpdateLastTimeToggled_(bool const wasPreviouslyDown,
                                           bool const isDownNow,
                                           TimeMs_t const & currentTimeMs)
    {
        if (isDownNow != wasPreviouslyDown)
        {
            lastTimeToggledMs_ = currentTimeMs;
        }
    }

    static bool isLongDuration(TimeMs_t const & currentTimeMs, TimeMs_t const & lastTimeMs)
    {
        return ((currentTimeMs - lastTimeMs) >= durationLongMs_);
    }

    static TimeMs_t constexpr durationLongMs_ = 1000;
    TimeMs_t lastTimeToggledMs_;

};

#endif // BUTTON_TIMED_HPP
