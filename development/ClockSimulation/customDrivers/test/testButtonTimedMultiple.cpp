#include <customDrivers/buttonTimedMultiple.hpp>

#include <gtest/gtest.h>


typedef ButtonTimedMultiple<> Button;

TEST(ButtonTimedMultiple, nominal)
{
    Button::Timestamp_t timestamp = Button::now();

    ButtonTimedProperties::Duration_t const durationShort = std::chrono::milliseconds{100};
    ButtonTimedProperties::Duration_t const durationLong = std::chrono::milliseconds{300};
    ButtonTimedProperties::Duration_t const durationCombineMax = std::chrono::milliseconds{100};

    Button button{durationShort, durationLong, durationCombineMax, ButtonState::Up};

    timestamp += durationLong;
    button.update(ButtonState::Down, timestamp);

    timestamp += durationShort;
    button.update(ButtonState::Up, timestamp);

    EXPECT_TRUE(button.wasDownShort());
}
