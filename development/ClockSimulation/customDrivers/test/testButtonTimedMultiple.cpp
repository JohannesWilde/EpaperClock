#include <customDrivers/buttonTimedMultiple.hpp>

#include <gtest/gtest.h>


typedef ButtonTimedMultiple<> Button;

static ButtonTimedProperties::Duration_t const durationShort = std::chrono::milliseconds{100};
static ButtonTimedProperties::Duration_t const durationLong = std::chrono::milliseconds{300};
static ButtonTimedProperties::Duration_t const durationCombineMax = std::chrono::milliseconds{100};

static Button::Timestamp_t const timestampStart; // Use default constructor for base timestamp.

TEST(ButtonTimedMultiple, nominal)
{
    Button::Timestamp_t timestamp = timestampStart;

    Button button{durationShort,
                durationLong,
                durationCombineMax,
                ButtonState::Up};

    // initialized



    // short press
    button.update(ButtonState::Up, timestamp);

    timestamp += durationShort;
    button.update(ButtonState::Down, timestamp);

    timestamp += durationShort;
    button.update(ButtonState::Up, timestamp);

    {
        EXPECT_FALSE(button.isDown());
        EXPECT_TRUE(button.isUp());

        Button::Timestamp_t const since = timestampStart;
        EXPECT_FALSE(button.pressed(since));
        EXPECT_TRUE(button.released(since));
        EXPECT_TRUE(button.toggled(since));

        Button::Timestamp_t const now = timestamp;
        EXPECT_EQ(ButtonTimedProperties::Duration::TooShort, button.currentState(now));

        EXPECT_FALSE(button.isDownShort(now));
        EXPECT_FALSE(button.isDownLong(now));
        EXPECT_FALSE(button.isUpShort(now));
        EXPECT_FALSE(button.isUpLong(now));

        EXPECT_FALSE(button.pressedAfterShort(since));
        EXPECT_FALSE(button.pressedAfterLong(since));
        EXPECT_TRUE(button.releasedAfterShort(since));
        EXPECT_FALSE(button.releasedAfterLong(since));

        EXPECT_TRUE(button.wasDownShort());
        EXPECT_FALSE(button.wasDownLong());
        EXPECT_FALSE(button.wasUpShort());
        EXPECT_FALSE(button.wasUpLong());
    }
}

TEST(ButtonTimedMultiple, clearHistory)
{
    Button::Timestamp_t timestamp = timestampStart;

    Button button{durationShort, durationLong, durationCombineMax, ButtonState::Up};

    timestamp += durationLong;
    button.update(ButtonState::Down, timestamp);
    timestamp += durationShort;
    button.update(ButtonState::Up, timestamp);
    timestamp += durationLong;
    button.update(ButtonState::Down, timestamp);
    timestamp += durationShort;
    button.update(ButtonState::Up, timestamp);
    timestamp += 2 * durationLong;
    button.update(ButtonState::Down, timestamp);

    {
        EXPECT_TRUE(button.isDown());
        EXPECT_FALSE(button.isUp());

        Button::Timestamp_t const since = timestampStart;
        EXPECT_TRUE(button.pressed(since));
        EXPECT_FALSE(button.released(since));
        EXPECT_TRUE(button.toggled(since));

        Button::Timestamp_t const now = timestamp + durationShort;
        EXPECT_EQ(ButtonTimedProperties::Duration::Short, button.currentState(now));

        EXPECT_TRUE(button.isDownShort(now));
        EXPECT_FALSE(button.isDownLong(now));
        EXPECT_FALSE(button.isUpShort(now));
        EXPECT_FALSE(button.isUpLong(now));

        EXPECT_FALSE(button.pressedAfterShort(since));
        EXPECT_TRUE(button.pressedAfterLong(since));
        EXPECT_FALSE(button.releasedAfterShort(since));
        EXPECT_FALSE(button.releasedAfterLong(since));

        EXPECT_FALSE(button.wasDownShort());
        EXPECT_FALSE(button.wasDownLong());
        EXPECT_FALSE(button.wasUpShort());
        EXPECT_TRUE(button.wasUpLong());
    }


    button.clearHistory(ButtonState::Up);

    {
        EXPECT_FALSE(button.isDown());
        EXPECT_TRUE(button.isUp());

        Button::Timestamp_t const since = timestampStart;
        EXPECT_FALSE(button.pressed(since));
        EXPECT_FALSE(button.released(since));
        EXPECT_FALSE(button.toggled(since));

        Button::Timestamp_t const now = timestampStart;
        EXPECT_EQ(ButtonTimedProperties::Duration::TooShort, button.currentState(now));

        EXPECT_FALSE(button.isDownShort());
        EXPECT_FALSE(button.isDownLong());
        EXPECT_FALSE(button.isUpShort());
        EXPECT_FALSE(button.isUpLong());

        EXPECT_FALSE(button.pressedAfterShort(since));
        EXPECT_FALSE(button.pressedAfterLong(since));
        EXPECT_FALSE(button.releasedAfterShort(since));
        EXPECT_FALSE(button.releasedAfterLong(since));

        EXPECT_FALSE(button.wasDownShort());
        EXPECT_FALSE(button.wasDownLong());
        EXPECT_FALSE(button.wasUpShort());
        EXPECT_FALSE(button.wasUpLong());
    }
}
