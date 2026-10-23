#ifndef EPAPER_CLOCK_DUMMY_STATEMACHINE_HPP
#define EPAPER_CLOCK_DUMMY_STATEMACHINE_HPP

#include "dummybuttons.hpp"

#include <helpers/statemachine.hpp>


namespace EpaperClock
{

struct DataType
{
    CustomButtomTimedMultiple const & buttonSettings;
    CustomButtomTimedMultiple const & buttonRight;
    CustomButtomTimedMultiple const & buttonUp;
    CustomButtomTimedMultiple const & buttonOnOffOrDown;

    int delay;
    bool updateDisplay;
};

class StateOff : public Helpers::AbstractState<DataType>
{
    void init(DataType & data) const override;
    AbstractState const & process(DataType & data) const override;
    void deinit(DataType & data) const override;
};

extern StateOff const stateOff;

class StateSplashScreen : public Helpers::AbstractState<DataType>
{
    void init(DataType & data) const override;
    AbstractState const & process(DataType & data) const override;
    void deinit(DataType & data) const override;
};

extern StateSplashScreen const stateSplashScreen;

class StateClock : public Helpers::AbstractState<DataType>
{
    void init(DataType & data) const override;
    AbstractState const & process(DataType & data) const override;
    void deinit(DataType & data) const override;
};

extern StateClock const stateClock;

class StateSettings : public Helpers::AbstractState<DataType>
{
    void init(DataType & data) const override;
    AbstractState const & process(DataType & data) const override;
    void deinit(DataType & data) const override;
};

extern StateSettings const stateSettings;


typedef Helpers::Statemachine<EpaperClock::DataType> Statemachine_t;

} // namespace EpaperClock

#endif // EPAPER_CLOCK_DUMMY_STATEMACHINE_HPP
