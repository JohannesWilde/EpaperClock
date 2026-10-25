#include "dummyStatemachine.hpp"

#include <chrono>


namespace EpaperClock
{

StateOff const stateOff;
StateSplashScreen const stateSplashScreen;
StateClock const stateClock;
StateSettings const stateSettings;

Helpers::Statemachine<DataType> statemachine(stateOff);


void StateOff::init(DataType & data) const
{
    data.clockGui.buttonSettings.set(false);
    data.clockGui.buttonSettingsRight.set(false);
    data.clockGui.buttonUp.set(false);
    data.clockGui.buttonDown.set(false);
    data.clockGui.buttonOnOff.set(true);

    data.clockGui.selection0.set(false);
    data.clockGui.selection1.set(false);
    data.clockGui.selection2.set(false);
    data.clockGui.selection3.set(false);

    data.clockGui.dotUpper.set(false);
    data.clockGui.dotLower.set(false);

    data.clockGui.sevenSegments0.set(GuiFixtures::Renderer2dSevenSegments::Display::none);
    data.clockGui.sevenSegments1.set(GuiFixtures::Renderer2dSevenSegments::Display::none);
    data.clockGui.sevenSegments2.set(GuiFixtures::Renderer2dSevenSegments::Display::none);
    data.clockGui.sevenSegments3.set(GuiFixtures::Renderer2dSevenSegments::Display::none);

    // Use delay to determine first render below.
    data.delay = 0;
}

Helpers::AbstractState<DataType> const & StateOff::process(DataType & data) const
{
    Helpers::AbstractState<DataType> const * nextState = this;
    if (data.buttonOnOffOrDown.isSingleDownShortFinished())  // todo: react to down - but then wait that released before reacting again...
    {
        nextState = &stateSplashScreen;
    }
    else
    {
        // intentionally empty
    }

    // blank display
    data.updateDisplay = (0 == data.delay);

    if (std::numeric_limits<int>::max() > data.delay)
    {
        ++data.delay;
    }
    else
    {
        // intentionally empty
    }

    return *nextState;
}

void StateOff::deinit(DataType & data) const
{
    // intentionally empty
}


constexpr int splashDelay = 20;

void StateSplashScreen::init(DataType & data) const
{
    data.delay = splashDelay;

    data.clockGui.buttonSettings.set(true);
    data.clockGui.buttonSettingsRight.set(false);
    data.clockGui.buttonUp.set(true);
    data.clockGui.buttonDown.set(true);
    data.clockGui.buttonOnOff.set(true);

    data.clockGui.selection0.set(true);
    data.clockGui.selection1.set(true);
    data.clockGui.selection2.set(true);
    data.clockGui.selection3.set(true);

    data.clockGui.dotUpper.set(true);
    data.clockGui.dotLower.set(true);

    data.clockGui.sevenSegments0.set(GuiFixtures::Renderer2dSevenSegments::Display::number8);
    data.clockGui.sevenSegments1.set(GuiFixtures::Renderer2dSevenSegments::Display::number8);
    data.clockGui.sevenSegments2.set(GuiFixtures::Renderer2dSevenSegments::Display::number8);
    data.clockGui.sevenSegments3.set(GuiFixtures::Renderer2dSevenSegments::Display::number8);
}

Helpers::AbstractState<DataType> const & StateSplashScreen::process(DataType & data) const
{
    Helpers::AbstractState<DataType> const * nextState = this;

    // render only once
    data.updateDisplay = (splashDelay == data.delay);

    bool advance = false;
    if (0 < data.delay)
    {
        --data.delay;
    }
    else
    {
        advance = true;
    }

    if (advance || data.buttonOnOffOrDown.isSingleDownShortFinished()) // todo: down...
    {
        nextState = &stateClock;
    }
    else
    {
        // intentionally empty
    }

    return *nextState;
}

void StateSplashScreen::deinit(DataType & data) const
{
    // intentionally empty
}




void StateClock::init(DataType & data) const
{
    data.clockGui.buttonSettings.set(true);
    data.clockGui.buttonSettingsRight.set(false);
    data.clockGui.buttonUp.set(false);
    data.clockGui.buttonDown.set(false);
    data.clockGui.buttonOnOff.set(true);

    data.clockGui.selection0.set(false);
    data.clockGui.selection1.set(false);
    data.clockGui.selection2.set(false);
    data.clockGui.selection3.set(false);

    data.clockGui.dotUpper.set(true);
    data.clockGui.dotLower.set(true);

    data.previousMinutesLow = -1;
}

Helpers::AbstractState<DataType> const & StateClock::process(DataType & data) const
{
    Helpers::AbstractState<DataType> const * nextState = this;
    if (data.buttonOnOffOrDown.isSingleDownShortFinished())
    {
        nextState = &stateOff;
    }
    else if (data.buttonSettings.isSingleDownShortFinished())
    {
        nextState = &stateSettings;
    }
    else
    {
        // update clock [only if changed]
        bool updateDisplay = true;

        std::chrono::system_clock::time_point const timestamp = std::chrono::system_clock::now();
        std::time_t const nowTime = std::chrono::system_clock::to_time_t(timestamp);

        struct tm nowLocal{0};
        errno_t const success = localtime_s(&nowLocal, &nowTime);
        if (0 == success)
        {
            int const hours = nowLocal.tm_hour;
            int const minutes = nowLocal.tm_min;

            int const hoursLow = hours % 10;
            int const hoursHigh = hours / 10;

            int const minutesLow = minutes % 10;
            int const minutesHigh = minutes / 10;

            if (data.previousMinutesLow != minutesLow)
            {
                data.clockGui.sevenSegments0.set(GuiFixtures::Renderer2dSevenSegments::singleDigitToDisplay(hoursHigh));
                data.clockGui.sevenSegments1.set(GuiFixtures::Renderer2dSevenSegments::singleDigitToDisplay(hoursLow));
                data.clockGui.sevenSegments2.set(GuiFixtures::Renderer2dSevenSegments::singleDigitToDisplay(minutesHigh));
                data.clockGui.sevenSegments3.set(GuiFixtures::Renderer2dSevenSegments::singleDigitToDisplay(minutesLow));

                data.previousMinutesLow = minutesLow;
            }
            else
            {
                updateDisplay = false;
            }
        }
        else
        {
            data.clockGui.sevenSegments0.set(GuiFixtures::Renderer2dSevenSegments::Display::number8);
            data.clockGui.sevenSegments1.set(GuiFixtures::Renderer2dSevenSegments::Display::number8);
            data.clockGui.sevenSegments2.set(GuiFixtures::Renderer2dSevenSegments::Display::number8);
            data.clockGui.sevenSegments3.set(GuiFixtures::Renderer2dSevenSegments::Display::number8);
        }

        data.updateDisplay = updateDisplay;
    }
    return *nextState;
}

void StateClock::deinit(DataType & data) const
{
    // intentionally empty
}




void StateSettings::init(DataType & data) const
{
    data.delay = 200;

    // todo: display clock and settings selection
    data.updateDisplay = true;

    // todo: readout current time to data
}

Helpers::AbstractState<DataType> const & StateSettings::process(DataType & data) const
{
    Helpers::AbstractState<DataType> const * nextState = this;
    bool interacted = true;
    if (data.buttonOnOffOrDown.isSingleDownShortFinished())
    {
        // down
    }
    else if (data.buttonSettings.isSingleDownShortFinished())
    {
        nextState = &stateClock;
    }
    else if (data.buttonRight.isSingleDownShortFinished())
    {
        // right
    }
    else if (data.buttonUp.isSingleDownShortFinished())
    {
        // up
    }
    else
    {
        interacted = false;
    }

    if (interacted)
    {
        if (0 < data.delay)
        {
            --data.delay;
        }
        else
        {
            nextState = &stateClock;
        }
    }
    else
    {
        data.delay = 200;
    }

    return *nextState;
}

void StateSettings::deinit(DataType & data) const
{
    // apply new time from data
}



} // namespace EpaperClock
