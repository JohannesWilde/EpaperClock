#include <customDrivers/buttonTimedMultiple.hpp>

#include <gtest/gtest.h>


typedef ButtonTimedMultiple<> Button;

static ButtonTimedProperties::Duration_t const durationShort = std::chrono::milliseconds{100};
static ButtonTimedProperties::Duration_t const durationLong = std::chrono::milliseconds{300};
static ButtonTimedProperties::Duration_t const durationCombineMax = std::chrono::milliseconds{100};


TEST(ButtonTimedMultiple, nominal)
{
    Button::Timestamp_t timestamp; // Use default constructor for base timestamp.

    Button button{durationShort, durationLong, durationCombineMax, ButtonState::Up};

    timestamp += durationLong;
    button.update(ButtonState::Down, timestamp);

    timestamp += durationShort;
    button.update(ButtonState::Up, timestamp);

    EXPECT_TRUE(button.wasDownShort());
}
