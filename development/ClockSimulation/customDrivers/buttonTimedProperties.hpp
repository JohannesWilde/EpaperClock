#ifndef BUTTON_TIMED_PROPERTIES_HPP
#define BUTTON_TIMED_PROPERTIES_HPP

// ----------------------------------------------------------------------------------------------------

#include <chrono>

// ----------------------------------------------------------------------------------------------------

namespace ButtonTimedProperties
{

typedef std::chrono::milliseconds Duration_t;

enum class Duration
{
    TooShort,
    Short,
    Long
};

} // namespace ButtonTimedProperties

// ----------------------------------------------------------------------------------------------------

#endif // BUTTON_TIMED_PROPERTIES_HPP
