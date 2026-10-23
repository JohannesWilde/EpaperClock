#include "dummyStatemachine.hpp"


namespace EpaperClock
{

StateOff const stateOff;
StateSplashScreen const stateSplashScreen;
StateClock const stateClock;
StateSettings const stateSettings;

Helpers::Statemachine<DataType> statemachine(stateOff);


void StateOff::init(DataType & data) const
{
    // todo: blank display
    data.updateDisplay = true;
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
    return *nextState;
}

void StateOff::deinit(DataType & data) const
{
    // intentionally empty
}




void StateSplashScreen::init(DataType & data) const
{
    data.delay = 20;

    // todo: display splash image
    data.updateDisplay = true;
}

Helpers::AbstractState<DataType> const & StateSplashScreen::process(DataType & data) const
{
    Helpers::AbstractState<DataType> const * nextState = this;

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
    // todo: display clock
    data.updateDisplay = true;
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
        // todo: update clock [only if changed]
        data.updateDisplay = /*changed ?*/ true;
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
