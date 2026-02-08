#ifndef CONSTEXPRABS_HPP
#define CONSTEXPRABS_HPP

#include <cassert>
#include <limits>


// cmath's std::abs is only constexpr as of C++23. But even selection C++23 did not work with my current MSVC.
constexpr int constexpr_abs(int const value)
{
    assert(std::numeric_limits<int>::min() != value);
    return (0 > value) ? (-1 * value) : value;
}

#endif // CONSTEXPRABS_HPP
