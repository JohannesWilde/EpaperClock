#ifndef EPAPER_CLOCK_DUMMY_STATEMACHINE_HPP
#define EPAPER_CLOCK_DUMMY_STATEMACHINE_HPP

#include "dummybuttons.hpp"
#include "renderer2dclockgui.hpp"

#include <helpers/statemachine.hpp>

#include <chrono>


namespace EpaperClock
{

struct DataType
{
    CustomButtomTimedMultiple const & buttonSettings;
    CustomButtomTimedMultiple const & buttonRight;
    CustomButtomTimedMultiple const & buttonUp;
    CustomButtomTimedMultiple const & buttonOnOffOrDown;

    Renderer2dClockGui & clockGui;

    std::chrono::system_clock::time_point timestamp;
    std::chrono::system_clock::duration timeOffset;
    int delay;

    bool updateDisplay;

    DataType(CustomButtomTimedMultiple const & buttonSettings,
             CustomButtomTimedMultiple const & buttonRight,
             CustomButtomTimedMultiple const & buttonUp,
             CustomButtomTimedMultiple const & buttonOnOffOrDown,
             Renderer2dClockGui & clockGui)
        : buttonSettings(buttonSettings)
        , buttonRight(buttonRight)
        , buttonUp(buttonUp)
        , buttonOnOffOrDown(buttonOnOffOrDown)
        , clockGui(clockGui)
        , timeOffset(std::chrono::minutes(0))
        , delay(-1)
        , updateDisplay(false)
    {
        // intentionally empty
    }

    DataType(DataType const &) = default;
    DataType(DataType &&) = default;
    DataType & operator=(DataType const &) = default;
    DataType & operator=(DataType &&) = default;

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
