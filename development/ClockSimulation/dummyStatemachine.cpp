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

enum class ValueModification
{
    none,
    up,
    down,
};


static struct tm tmFromTime(std::chrono::system_clock::time_point const clockTime)
{
    std::time_t const time = std::chrono::system_clock::to_time_t(clockTime);

    struct tm tmData{0};
    errno_t const success = localtime_s(&tmData, &time);

    if (0 != success)
    {
        throw std::exception("Failed to convert tmFromTime.");
    }
    else
    {
        // intentionally empty
    }

    return tmData;
}

static std::chrono::system_clock::time_point timeFromTm(struct tm & tmData)
{
    time_t const time = mktime(&tmData);

    if (static_cast<time_t>(-1) == time)
    {
        throw std::exception("Failed to convert timeFromTm.");
    }
    else
    {
        // intentionally empty
    }

    std::chrono::system_clock::time_point const clockTime = std::chrono::system_clock::from_time_t(time);

    return clockTime;
}

static bool timesChangedPerceptibly(std::chrono::system_clock::time_point const from,
                                    std::chrono::system_clock::time_point const to)
{
    bool perceptibleDifference = (std::chrono::minutes(1) <= std::chrono::abs(to - from));

    if (not perceptibleDifference)
    {
        // Check for difference in minutes if to was not aligned with a change of a minute [i.e. seconds = 0].
        try
        {
            struct tm const tmFrom = tmFromTime(from);

            struct tm tmTo = tmFromTime(from);

            perceptibleDifference = (tmFrom.tm_min != tmTo.tm_min);
        }
        catch (std::exception const &)
        {
            // intentionally empty
        }
    }
    else
    {
        // intentionally empty
    }

    return perceptibleDifference;
}

static void applyModificationToValue(int & value, int const min, int const max, ValueModification const modification)
{
    switch (modification)
    {
    case ValueModification::up:
    {
        if (max <= value)
        {
            value = min;
        }
        else
        {
            ++value;
        }
        break;
    }
    case ValueModification::down:
    {
        if (min >= value)
        {
            value = max;
        }
        else
        {
            --value;
        }
        break;
    }
    case ValueModification::none:
    {
        break;
    }
    }
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

    data.timestamp = std::chrono::system_clock::now() + data.timeOffset;
    data.selection = SettingsSelection::hours10;

    data.delay = settingsTimeout;
}

Helpers::AbstractState<DataType> const & StateSettings::process(DataType & data) const
{
    Helpers::AbstractState<DataType> const * nextState = this;

    bool interacted = true;
    ValueModification modification = ValueModification::none;
    if (data.buttonOnOffOrDown.isSingleDownShortFinished())
    {
        modification = ValueModification::down;
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
        modification = ValueModification::up;
    }
    else
    {
        interacted = false;
    }

    if (ValueModification::none != modification)
    {
        try
        {
            struct tm tmData = tmFromTime(data.timestamp);

            switch (data.selection)
            {
            case SettingsSelection::hours10:
            {
                int const digit0 = tmData.tm_hour % 10;
                int digit1 = tmData.tm_hour / 10;
                applyModificationToValue(digit1, 0, 2, modification);
                tmData.tm_hour = 10 * digit1 + digit0;
                break;
            }
            case SettingsSelection::hours1:
            {
                int digit0 = tmData.tm_hour % 10;
                int const digit1 = tmData.tm_hour / 10;
                int const digit0Max = (2 == digit1) ? 3 : 9;
                applyModificationToValue(digit0, 0, digit0Max, modification);
                tmData.tm_hour = 10 * digit1 + digit0;
                break;
            }
            case SettingsSelection::minutes10:
            {
                int const digit0 = tmData.tm_min % 10;
                int digit1 = tmData.tm_min / 10;
                applyModificationToValue(digit1, 0, 5, modification);
                tmData.tm_min = 10 * digit1 + digit0;
                break;
            }
            case SettingsSelection::minutes1:
            {
                int digit0 = tmData.tm_min % 10;
                int const digit1 = tmData.tm_min / 10;
                applyModificationToValue(digit0, 0, 9, modification);
                tmData.tm_min = 10 * digit1 + digit0;
                break;
            }
            }

            data.timestamp = timeFromTm(tmData);

        }
        catch (std::exception const &)
        {
            // intentionally empty
        }
    }
    else
    {
        // intentionally empty
    }

    // display clock and settings selection [on interaction or at first pass].
    data.updateDisplay = interacted || (settingsTimeout == data.delay);

    if (data.updateDisplay)
    {
        /*bool const successfull =*/ applyTimestampToClockGui(data.timestamp, data.clockGui);

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
    std::chrono::system_clock::time_point const set = data.timestamp;
    data.timeOffset = set - now;
}



} // namespace EpaperClock
