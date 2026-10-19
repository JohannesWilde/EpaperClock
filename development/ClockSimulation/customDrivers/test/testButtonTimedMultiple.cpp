#include <customDrivers/buttonTimedMultiple.hpp>

#include <gtest/gtest.h>


typedef ButtonTimedMultiple<> Button;

static ButtonTimedProperties::Duration_t const durationShort = std::chrono::milliseconds{100};
static ButtonTimedProperties::Duration_t const durationLong = std::chrono::milliseconds{300};
static ButtonTimedProperties::Duration_t const durationCombineMax = std::chrono::milliseconds{100};

static ButtonTimedProperties::Duration_t const durationEpsilon = std::chrono::milliseconds{1};

static Button::Timestamp_t const timestampStart; // Use default constructor for base timestamp.

TEST(ButtonTimedMultiple, wasDownShort)
{
    Button::Timestamp_t timestamp = timestampStart;

    Button button{durationShort, durationLong, durationCombineMax, ButtonState::Up};

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

TEST(ButtonTimedMultiple, wasDownLong)
{
    Button::Timestamp_t timestamp = timestampStart;

    Button button{durationShort, durationLong, durationCombineMax, ButtonState::Up};

    // long press
    button.update(ButtonState::Up, timestamp);
    timestamp += durationShort;

    button.update(ButtonState::Down, timestamp);
    timestamp += durationLong;

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
        EXPECT_FALSE(button.releasedAfterShort(since));
        EXPECT_TRUE(button.releasedAfterLong(since));

        EXPECT_FALSE(button.wasDownShort());
        EXPECT_TRUE(button.wasDownLong());
        EXPECT_FALSE(button.wasUpShort());
        EXPECT_FALSE(button.wasUpLong());
    }
}

TEST(ButtonTimedMultiple, wasUpShort)
{
    Button::Timestamp_t timestamp = timestampStart;

    Button button{durationShort, durationLong, durationCombineMax, ButtonState::Up};

    // short up
    button.update(ButtonState::Down, timestamp);
    timestamp += durationShort;

    button.update(ButtonState::Up, timestamp);
    timestamp += durationShort;

    button.update(ButtonState::Down, timestamp);

    {
        EXPECT_TRUE(button.isDown());
        EXPECT_FALSE(button.isUp());

        Button::Timestamp_t const since = timestampStart;
        EXPECT_TRUE(button.pressed(since));
        EXPECT_FALSE(button.released(since));
        EXPECT_TRUE(button.toggled(since));

        Button::Timestamp_t const now = timestamp;
        EXPECT_EQ(ButtonTimedProperties::Duration::TooShort, button.currentState(now));

        EXPECT_FALSE(button.isDownShort(now));
        EXPECT_FALSE(button.isDownLong(now));
        EXPECT_FALSE(button.isUpShort(now));
        EXPECT_FALSE(button.isUpLong(now));

        EXPECT_TRUE(button.pressedAfterShort(since));
        EXPECT_FALSE(button.pressedAfterLong(since));
        EXPECT_FALSE(button.releasedAfterShort(since));
        EXPECT_FALSE(button.releasedAfterLong(since));

        EXPECT_FALSE(button.wasDownShort());
        EXPECT_FALSE(button.wasDownLong());
        EXPECT_TRUE(button.wasUpShort());
        EXPECT_FALSE(button.wasUpLong());
    }
}

TEST(ButtonTimedMultiple, wasUpLong)
{
    Button::Timestamp_t timestamp = timestampStart;

    Button button{durationShort, durationLong, durationCombineMax, ButtonState::Up};

    // short up
    button.update(ButtonState::Down, timestamp);
    timestamp += durationShort;

    button.update(ButtonState::Up, timestamp);
    timestamp += durationLong;

    button.update(ButtonState::Down, timestamp);

    {
        EXPECT_TRUE(button.isDown());
        EXPECT_FALSE(button.isUp());

        Button::Timestamp_t const since = timestampStart;
        EXPECT_TRUE(button.pressed(since));
        EXPECT_FALSE(button.released(since));
        EXPECT_TRUE(button.toggled(since));

        Button::Timestamp_t const now = timestamp;
        EXPECT_EQ(ButtonTimedProperties::Duration::TooShort, button.currentState(now));

        EXPECT_FALSE(button.isDownShort(now));
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

TEST(ButtonTimedMultiple, currentDuration)
{
    std::chrono::duration const durationLongLater = std::chrono::milliseconds(2000);

    assert((durationLong - durationShort) > durationEpsilon);
    assert(durationShort > durationEpsilon);

    Button::Timestamp_t const timestampLongBegin = timestampStart + durationLong;
    Button::Timestamp_t const timestampLongLater = timestampLongBegin + durationLongLater;
    Button::Timestamp_t const timestampShortBegin = timestampStart + durationShort;
    Button::Timestamp_t const timestampShortEnd = timestampLongBegin - durationEpsilon;
    Button::Timestamp_t const timestampTooShortBegin = timestampStart;
    Button::Timestamp_t const timestampTooShortEnd = timestampShortBegin - durationEpsilon;

    Button button{durationShort, durationLong, durationCombineMax, ButtonState::Up};

    {
        EXPECT_EQ(ButtonTimedProperties::Duration::TooShort, button.currentState(timestampTooShortBegin));
        EXPECT_EQ(ButtonTimedProperties::Duration::TooShort, button.currentState(timestampTooShortEnd));
        EXPECT_EQ(ButtonTimedProperties::Duration::TooShort, button.currentState(timestampShortBegin));
        EXPECT_EQ(ButtonTimedProperties::Duration::TooShort, button.currentState(timestampShortEnd));
        EXPECT_EQ(ButtonTimedProperties::Duration::TooShort, button.currentState(timestampLongBegin));
        EXPECT_EQ(ButtonTimedProperties::Duration::TooShort, button.currentState(timestampLongLater));

        EXPECT_FALSE(button.isDownShort(timestampShortBegin));
        EXPECT_FALSE(button.isDownLong(timestampLongBegin));
        EXPECT_FALSE(button.isUpShort(timestampShortBegin));
        EXPECT_FALSE(button.isUpLong(timestampLongBegin));
    }

    // First update.
    button.update(ButtonState::Up, timestampStart);

    {
        EXPECT_EQ(ButtonTimedProperties::Duration::TooShort, button.currentState(timestampTooShortBegin));
        EXPECT_EQ(ButtonTimedProperties::Duration::TooShort, button.currentState(timestampTooShortEnd));
        EXPECT_EQ(ButtonTimedProperties::Duration::Short, button.currentState(timestampShortBegin));
        EXPECT_EQ(ButtonTimedProperties::Duration::Short, button.currentState(timestampShortEnd));
        EXPECT_EQ(ButtonTimedProperties::Duration::Long, button.currentState(timestampLongBegin));
        EXPECT_EQ(ButtonTimedProperties::Duration::Long, button.currentState(timestampLongLater));

        EXPECT_FALSE(button.isDownShort(timestampShortBegin));
        EXPECT_FALSE(button.isDownLong(timestampLongBegin));
        EXPECT_TRUE(button.isUpShort(timestampShortBegin));
        EXPECT_TRUE(button.isUpLong(timestampLongBegin));
    }

    // First update.
    button.update(ButtonState::Down, timestampStart);

    {
        EXPECT_EQ(ButtonTimedProperties::Duration::TooShort, button.currentState(timestampTooShortBegin));
        EXPECT_EQ(ButtonTimedProperties::Duration::TooShort, button.currentState(timestampTooShortEnd));
        EXPECT_EQ(ButtonTimedProperties::Duration::Short, button.currentState(timestampShortBegin));
        EXPECT_EQ(ButtonTimedProperties::Duration::Short, button.currentState(timestampShortEnd));
        EXPECT_EQ(ButtonTimedProperties::Duration::Long, button.currentState(timestampLongBegin));
        EXPECT_EQ(ButtonTimedProperties::Duration::Long, button.currentState(timestampLongLater));

        EXPECT_TRUE(button.isDownShort(timestampShortBegin));
        EXPECT_TRUE(button.isDownLong(timestampLongBegin));
        EXPECT_FALSE(button.isUpShort(timestampShortBegin));
        EXPECT_FALSE(button.isUpLong(timestampLongBegin));
    }
}

TEST(ButtonTimedMultiple, shortDown)
{
    Button::Timestamp_t timestamp = timestampStart;

    Button button{durationShort, durationLong, durationCombineMax, ButtonState::Up};

    for (size_t index = 0; 2 > index; ++index)
    {
        // short down
        button.update(ButtonState::Down, timestamp);
        timestamp += durationShort;

        button.update(ButtonState::Up, timestamp);

        Button::Timestamp_t now = timestamp;

        {
            EXPECT_FALSE(button.isDoubleDownShortFinished(now));
            EXPECT_FALSE(button.isSingleDownShortFinished(now));
        }

        now += durationCombineMax;

        {
            EXPECT_FALSE(button.isDoubleDownShortFinished(now));
            EXPECT_FALSE(button.isSingleDownShortFinished(now));
        }

        now += durationEpsilon;

        Button::Timestamp_t const timestampDetection = now;
        {
            EXPECT_FALSE(button.isDoubleDownShortFinished(timestampDetection));
            EXPECT_TRUE(button.isSingleDownShortFinished(timestampDetection)) << "At index " << index << ".";

            // If I query at the same time again, expect it to yield the same results again.
            EXPECT_FALSE(button.isDoubleDownShortFinished(timestampDetection));
            EXPECT_TRUE(button.isSingleDownShortFinished(timestampDetection));
        }

        {
            // At a later point do not report the same click again.
            Button::Timestamp_t const later = now + durationEpsilon;
            EXPECT_FALSE(button.isDoubleDownShortFinished(later));
            EXPECT_FALSE(button.isSingleDownShortFinished(later));
        }

        {
            // At a later point do not report the same click again.
            Button::Timestamp_t const later = now + durationShort;
            EXPECT_FALSE(button.isDoubleDownShortFinished(later));
            EXPECT_FALSE(button.isSingleDownShortFinished(later));
        }

        {
            // At a later point do not report the same click again.
            Button::Timestamp_t const later = now + durationLong;
            EXPECT_FALSE(button.isDoubleDownShortFinished(later));
            EXPECT_FALSE(button.isSingleDownShortFinished(later));
        }


        // Next press.
        timestamp = timestampDetection + durationEpsilon;
    }

}
