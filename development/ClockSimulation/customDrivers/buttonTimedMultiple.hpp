#ifndef DYNAMIC_BUTTON_TIMED_HPP
#define DYNAMIC_BUTTON_TIMED_HPP

// ----------------------------------------------------------------------------------------------------

#include "buttonTimedProperties.hpp"

#include <array>
#include <cassert>
#include <chrono>
#include <optional>

// ----------------------------------------------------------------------------------------------------

enum class ButtonState
{
    Up,
    Down
};


/**
 * @brief The ButtonTimed class tracks the durations of HistoryLength_ previous states.
 * Intrinsically it has support for determining whether the last press was tooShort,
 * short or long.
 * For more info see ButtonTimedMultiple.
 */
template <size_t HistoryLength_ = 2>
class ButtonTimed
{
public:
    static_assert(2 <= HistoryLength_);

    typedef std::chrono::steady_clock::time_point Timestamp_t;

    static Timestamp_t now()
    {
        return std::chrono::steady_clock::now();
    }


    ButtonTimed(ButtonTimedProperties::Duration_t const durationShort,
                ButtonTimedProperties::Duration_t const durationLong,
                ButtonState const state = ButtonState::Up)
        : durationShort_(durationShort)
        , durationLong_(durationLong)
        , state_(state)
        , currentTimestamp_(history_.data())
    {
        clearHistory(state);

        assert(ButtonTimedProperties::Duration_t(0) < durationShort);
        assert(durationShort < durationLong);
    }

    void clearHistory(ButtonState const state = ButtonState::Up)
    {
        for (TimestampOptional_t & value : history_)
        {
            value.reset();
        }

        state_ = state;
    }

    void update(ButtonState const state, Timestamp_t const timestamp = now())
    {
        assert(static_cast<bool>(currentTimestamp_)); // Assert the pointer is valid.
        if ((not static_cast<bool>(*currentTimestamp_)) || (state != state_))
        {
            currentTimestamp_ = otherTimestamp_(currentTimestamp_, true, 1);
            *currentTimestamp_ = timestamp;

            state_ = state;
        }
        else
        {
            // intentionally empty
        }
    }


    bool isDown() const
    {
        return ButtonState::Down == state_;
    }

    bool isUp() const
    {
        return ButtonState::Up == state_;
    }


    bool pressed(Timestamp_t const & since) const
    {
        return (isDown() && somethingHappened_(since));
    }

    bool released(Timestamp_t const & since) const
    {
        return (!isDown() && somethingHappened_(since));
    }

    bool toggled(Timestamp_t const & since) const
    {
        // This only states that something changed since since. But - depending on the HistoryLength_ and
        // the frequency with which buttons are pressed and released - I can't know what state there was at
        // since. So use any change as the next best thing. If the calls are more frequent than the button
        // presses/releases then this should work as expected.
        return somethingHappened_(since);
    }


    ButtonTimedProperties::Duration currentState(Timestamp_t const timestamp = now()) const
    {
        ButtonTimedProperties::Duration duration = ButtonTimedProperties::Duration::TooShort;
        assert(static_cast<bool>(currentTimestamp_)); // Assert the pointer is valid.
        if (static_cast<bool>(*currentTimestamp_)) // Check optional to contain a value.
        {
            duration = durationToState_(std::chrono::duration_cast<ButtonTimedProperties::Duration_t>(timestamp - currentTimestamp_->value()));
        }
        else
        {
            // intentionally empty
        }
        return duration;
    }

    // With an offset of 0 you will have to specify now() as timestamp. Otherwise timestamp is of no relevance.
    ButtonTimedProperties::Duration previousState(size_t const offset = 1, Timestamp_t const * const timestamp = nullptr) const
    {
        return durationToState_(previousDuration_(offset, timestamp));
    }

    // convenience access methods

    bool isDownShort(Timestamp_t const timestamp = now())
    {
        return (isDown() &&
                (ButtonTimedProperties::Duration::Short == currentState(timestamp)));
    }

    bool isDownLong(Timestamp_t const timestamp = now())
    {
        return (isDown() &&
                (ButtonTimedProperties::Duration::Long == currentState(timestamp)));
    }

    bool isUpShort(Timestamp_t const timestamp = now())
    {
        return (isUp() &&
                (ButtonTimedProperties::Duration::Short == currentState(timestamp)));
    }

    bool isUpLong(Timestamp_t const timestamp = now())
    {
        return (isUp() &&
                (ButtonTimedProperties::Duration::Long == currentState(timestamp)));
    }

    bool pressedAfterShort(Timestamp_t const & since)
    {
        return (pressed(since) &&
                (ButtonTimedProperties::Duration::Short == previousState()));
    }

    bool pressedAfterLong(Timestamp_t const & since)
    {
        return (pressed(since) &&
                (ButtonTimedProperties::Duration::Long == previousState()));
    }

    bool releasedAfterShort(Timestamp_t const & since)
    {
        return (released(since) &&
                (ButtonTimedProperties::Duration::Short == previousState()));
    }

    bool releasedAfterLong(Timestamp_t const & since)
    {
        return (released(since) &&
                (ButtonTimedProperties::Duration::Long == previousState()));
    }

    bool wasDownShort()
    {
        return (!isDown() &&
                (ButtonTimedProperties::Duration::Short == previousState()));
    }

    bool wasDownLong()
    {
        return (!isDown() &&
                (ButtonTimedProperties::Duration::Long == previousState()));
    }

    bool wasUpShort()
    {
        return (!isUp() &&
                (ButtonTimedProperties::Duration::Short == previousState()));
    }

    bool wasUpLong()
    {
        return (!isUp() &&
                (ButtonTimedProperties::Duration::Long == previousState()));
    }

protected:

    ButtonTimedProperties::Duration_t previousDuration_(size_t const offset, Timestamp_t const * const timestamp = nullptr) const
    {
        ButtonTimedProperties::Duration_t duration{0};
        if (HistoryLength_ > offset)
        {
            // Determine time at offset.
            // An offset of 0 corresponds to "now() - currentTimestamp_->value()".
            // An offset of 1 to "currentTimestamp_->value() - (currentTimestamp_ - 1)->value()" [taking into account circular memory].
            // ...
            TimestampOptional_t const * const previous = otherTimestamp_(currentTimestamp_, false, offset);

            // Determine next timestamp.
            TimestampOptional_t nextTimestamp;
            if (0 == offset)
            {
                if (nullptr != timestamp)
                {
                    nextTimestamp = *timestamp;
                }
                else
                {
                    nextTimestamp = now();
                }
            }
            else
            {
                nextTimestamp = *otherTimestamp_(currentTimestamp_, false, offset - 1);
            }

            // Ignore "uninitialized" timestamps and keep 0, i.e. too short, instead.
            // If I am correct, previous being valid implies nextTimestamp to be valid. So only check previous.
            if (static_cast<bool>(*previous) /*&& static_cast<bool>(nextTimestamp)*/)
            {
                duration = std::chrono::duration_cast<std::chrono::milliseconds>(nextTimestamp.value() - previous->value());
            }
            else
            {
                // intentionally empty
            }
        }
        else
        {
            // Keep 0 for offsets out of range.
        }
        return duration;
    }

    ButtonTimedProperties::Duration durationToState_(ButtonTimedProperties::Duration_t const & duration) const
    {
        ButtonTimedProperties::Duration state = ButtonTimedProperties::Duration::TooShort;
        if (duration >= durationLong_)
        {
            state = ButtonTimedProperties::Duration::Long;
        }
        else if (duration >= durationShort_)
        {
            state = ButtonTimedProperties::Duration::Short;
        }
        else
        {
            // intentionally empty
        }
        return state;
    }

    constexpr bool somethingHappened_(Timestamp_t const & since) const
    {
        assert(static_cast<bool>(currentTimestamp_)); // Assert the pointer is valid.
        return static_cast<bool>(*currentTimestamp_) /* Check optional to contain a value. */
                && (since < currentTimestamp_->value());
    }


    ButtonTimedProperties::Duration_t getDurationShort_() const
    {
        return durationShort_;
    };

    ButtonTimedProperties::Duration_t getDurationLong_() const
    {
        return durationLong_;
    };


private:

    ButtonTimedProperties::Duration_t const durationShort_;
    ButtonTimedProperties::Duration_t const durationLong_;

    ButtonState state_;

    typedef std::optional<Timestamp_t> TimestampOptional_t;

    std::array<TimestampOptional_t, HistoryLength_> history_;
    TimestampOptional_t * currentTimestamp_;

    TimestampOptional_t * otherTimestamp_(
        TimestampOptional_t * const currentTimestamp,
        bool const forward,
        size_t const offset) const
    {
        // assert(currentTimestamp >= history_);
        // assert(currentTimestamp < (history_ + HistoryLength_));

        size_t const circularlyReducedOffset = offset % HistoryLength_;

        TimestampOptional_t * otherTimestamp = nullptr;
        if (forward)
        {
            otherTimestamp = currentTimestamp + circularlyReducedOffset;
            if (((&*history_.cbegin()) + history_.size()) <= otherTimestamp)
            {
                otherTimestamp -= HistoryLength_;
            }
            else
            {
                // no underflow possible
            }
        }
        else
        {
            otherTimestamp = currentTimestamp - circularlyReducedOffset;
            if ((&*history_.cbegin()) > otherTimestamp)
            {
                otherTimestamp += HistoryLength_;
            }
            else
            {
                // no overflow possible
            }
        }
        return otherTimestamp;
    }

};

// ----------------------------------------------------------------------------------------------------

template <size_t HistoryLength_ = 5>
class ButtonTimedMultiple : public ButtonTimed<HistoryLength_>
{
    typedef ButtonTimed<HistoryLength_> BaseButton;

public:

    static_assert(5 <= HistoryLength_);

    ButtonTimedMultiple(ButtonTimedProperties::Duration_t const durationShort,
                        ButtonTimedProperties::Duration_t const durationLong,
                        ButtonTimedProperties::Duration_t const durationCombineMax,
                        ButtonState const state = ButtonState::Up)
        : BaseButton(durationShort,
                     durationLong,
                     state)
        , durationCombineMax_(durationCombineMax)
    {
        assert(durationShort <= durationCombineMax);
    }

    // convenience access methods

    bool isDoubleDownShortFinished(BaseButton::Timestamp_t const & since)
    {
        return (BaseButton::somethingHappened_(since) &&
                BaseButton::isUp() &&
                // Current long enough not to potentially belong to the next one.
                (durationCombineMax_ < ButtonTimedMultiple::previousDuration_(0)) &&
                // Second short button press.
                (ButtonTimedProperties::Duration::Short == ButtonTimedMultiple::previousState(1)) &&
                // Intermediate not tooShort, but short enough to fit durationCombineMax_.
                (BaseButton::getDurationShort_() <= ButtonTimedMultiple::previousDuration_(2)) &&
                (durationCombineMax_ >= ButtonTimedMultiple::previousDuration_(2)) &&
                // First short button press.
                (ButtonTimedProperties::Duration::Short == ButtonTimedMultiple::previousState(3)) &&
                // Rule out more than double press.
                ((durationCombineMax_ < ButtonTimedMultiple::previousDuration_(4)) ||
                 (0 == ButtonTimedMultiple::previousDuration_(4)))
               );
    }

    bool isSingleDownShortFinished(BaseButton::Timestamp_t const & since)
    {
        return (BaseButton::somethingHappened_(since) &&
                BaseButton::isUp() &&
                // Current long enough not to potentially belong to the next one.
                (durationCombineMax_ < ButtonTimedMultiple::previousDuration_(0)) &&
                // First short button press.
                (ButtonTimedProperties::Duration::Short == ButtonTimedMultiple::previousState(1)) &&
                // Rule out more than single press.
                ((durationCombineMax_ < ButtonTimedMultiple::previousDuration_(2)) ||
                 (0 == ButtonTimedMultiple::previousDuration_(2)))
               );
    }

private:

    ButtonTimedProperties::Duration_t const durationCombineMax_;

};

// ----------------------------------------------------------------------------------------------------

#endif // DYNAMIC_BUTTON_TIMED_HPP
