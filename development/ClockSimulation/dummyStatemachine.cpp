#include "dummyStatemachine.hpp"

#include <chrono>


namespace EpaperClock
{

StateOff const stateOff;
StateSplashScreen const stateSplashScreen;
StateClock const stateClock;
StateSettings const stateSettings;

Helpers::Statemachine<DataType> statemachine(stateOff);

constexpr int splashDelay = 20;
constexpr int settingsTimeout = 200;


static bool timesChangedPerceptibly(std::chrono::system_clock::time_point const from,
                                    std::chrono::system_clock::time_point const to)
{
    bool perceptibleDifference = (std::chrono::minutes(1) <= std::chrono::abs(to - from));

    if (not perceptibleDifference)
    {
        // Check for difference in minutes if to was not aligned with a change of a minute [i.e. seconds = 0].

        struct tm tmFrom{0};
        {
            std::time_t const time = std::chrono::system_clock::to_time_t(from);
            errno_t const success = localtime_s(&tmFrom, &time);

            if (0 != success)
            {
                return true;
            }
            else
            {
                // intentionally empty
            }
        }

        struct tm tmTo{0};
        {
            std::time_t const time = std::chrono::system_clock::to_time_t(to);
            errno_t const success = localtime_s(&tmTo, &time);

            if (0 != success)
            {
                return true;
            }
            else
            {
                // intentionally empty
            }
        }

        perceptibleDifference = (tmFrom.tm_min != tmTo.tm_min);
    }
    else
    {
        // intentionally empty
    }

    return perceptibleDifference;
}

static bool applyTimestampToClockGui(std::chrono::system_clock::time_point const timestamp, Renderer2dClockGui & clockGui)
{
    std::time_t const nowTime = std::chrono::system_clock::to_time_t(timestamp);

    struct tm nowLocal{0};
    errno_t const success = localtime_s(&nowLocal, &nowTime);

    bool const successfull = (0 == success);
    if (successfull)
    {
        int const hours = nowLocal.tm_hour;
        int const minutes = nowLocal.tm_min;

        int const hoursLow = hours % 10;
        int const hoursHigh = hours / 10;

        int const minutesLow = minutes % 10;
        int const minutesHigh = minutes / 10;

        clockGui.sevenSegments0.set(GuiFixtures::Renderer2dSevenSegments::singleDigitToDisplay(hoursHigh));
        clockGui.sevenSegments1.set(GuiFixtures::Renderer2dSevenSegments::singleDigitToDisplay(hoursLow));
        clockGui.sevenSegments2.set(GuiFixtures::Renderer2dSevenSegments::singleDigitToDisplay(minutesHigh));
        clockGui.sevenSegments3.set(GuiFixtures::Renderer2dSevenSegments::singleDigitToDisplay(minutesLow));
    }
    else
    {
        clockGui.sevenSegments0.set(GuiFixtures::Renderer2dSevenSegments::Display::number8);
        clockGui.sevenSegments1.set(GuiFixtures::Renderer2dSevenSegments::Display::number8);
        clockGui.sevenSegments2.set(GuiFixtures::Renderer2dSevenSegments::Display::number8);
        clockGui.sevenSegments3.set(GuiFixtures::Renderer2dSevenSegments::Display::number8);
    }

    return successfull;
}


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


void StateSplashScreen::init(DataType & data) const
{
    data.delay = splashDelay;

    data.clockGui.buttonSettings.set(true);
    data.clockGui.buttonSettingsRight.set(true);
    data.clockGui.buttonUp.set(true);
    data.clockGui.buttonDown.set(false);
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

    data.timestamp = std::chrono::system_clock::now() - std::chrono::minutes(1);
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
        std::chrono::system_clock::time_point const timestamp = std::chrono::system_clock::now();

        data.updateDisplay = timesChangedPerceptibly(data.timestamp, timestamp);

        if (data.updateDisplay)
        {
            bool const successfull = applyTimestampToClockGui(timestamp + data.timeOffset, data.clockGui);
            if (successfull)
            {
                data.timestamp = timestamp;
            }
            else
            {
                // intentionally empty
            }
        }
        else
        {
            // intentionally empty
        }
    }
    return *nextState;
}

void StateClock::deinit(DataType & data) const
{
    // intentionally empty
}




void StateSettings::init(DataType & data) const
{
    data.clockGui.buttonSettings.set(true);
    data.clockGui.buttonSettingsRight.set(true);
    data.clockGui.buttonUp.set(true);
    data.clockGui.buttonDown.set(true);
    data.clockGui.buttonOnOff.set(false);

    data.clockGui.dotUpper.set(true);
    data.clockGui.dotLower.set(true);

    data.timestamp = std::chrono::system_clock::now() - std::chrono::minutes(1);
    data.selection = SettingsSelection::hours10;

    data.delay = settingsTimeout;
}

Helpers::AbstractState<DataType> const & StateSettings::process(DataType & data) const
{
    Helpers::AbstractState<DataType> const * nextState = this;

    bool interacted = true;
    int modificationMultiplier = 0;
    if (data.buttonOnOffOrDown.isSingleDownShortFinished())
    {
        modificationMultiplier = -1;
    }
    else if (data.buttonSettings.isSingleDownShortFinished())
    {
        nextState = &stateClock;
    }
    else if (data.buttonRight.isSingleDownShortFinished())
    {
        data.selection = EpaperClock::next(data.selection);
    }
    else if (data.buttonUp.isSingleDownShortFinished())
    {
        modificationMultiplier = 1;
    }
    else
    {
        interacted = false;
    }

    if (0 != modificationMultiplier)
    {
        std::chrono::minutes modificationDuration;
        switch (data.selection)
        {
        case SettingsSelection::hours10:
        {
            modificationDuration = std::chrono::minutes(600);
            break;
        }
        case SettingsSelection::hours1:
        {
            modificationDuration = std::chrono::minutes(60);
            break;
        }
        case SettingsSelection::minutes10:
        {
            modificationDuration = std::chrono::minutes(10);
            break;
        }
        case SettingsSelection::minutes1:
        {
            modificationDuration = std::chrono::minutes(1);
            break;
        }
        }

        data.timeOffset += modificationMultiplier * modificationDuration;
    }
    else
    {
        // intentionally empty
    }

    // display clock and settings selection [on interaction or at first pass].
    data.updateDisplay = interacted || (settingsTimeout == data.delay);

    if (data.updateDisplay)
    {
        /*bool const successfull =*/ applyTimestampToClockGui(data.timestamp + data.timeOffset, data.clockGui);

        data.clockGui.selection0.set(SettingsSelection::hours10 == data.selection);
        data.clockGui.selection1.set(SettingsSelection::hours1 == data.selection);
        data.clockGui.selection2.set(SettingsSelection::minutes10 == data.selection);
        data.clockGui.selection3.set(SettingsSelection::minutes1 == data.selection);
    }
    else
    {
        // intentionally empty
    }

    if (interacted)
    {
        // Reset timeout.
        data.delay = settingsTimeout;
    }
    else
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

    return *nextState;
}

void StateSettings::deinit(DataType & data) const
{
    // Recalc offset to match current time.
    std::chrono::system_clock::time_point const now = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point const set = data.timestamp + data.timeOffset;
    data.timeOffset = set - now;
}



} // namespace EpaperClock
