#include "coordinates2d.hpp"

#include <math.h>

namespace Coordinates2d
{

constexpr Distance::Distance(const Position &first, const Position &second)
    : x(second.x - first.x)
    , y(second.y - first.y)
{

}

double Distance::norm() const
{
    return sqrt(static_cast<double>(x * x + y * y));
}

constexpr bool Dimension::contains(const Position &position) const
{
    return ((0 <= position.x) && (x_ > position.x) && (0 <= position.y) && (y_ > position.y));
}

} // namespace Coordinates2d


std::ostream & operator<<(std::ostream & os, Coordinates2d::Dimension const & value)
{
    os << value.getX() << " x " << value.getY();
    return os;
}

std::ostream & operator<<(std::ostream & os, Coordinates2d::Distance const & value)
{
    os << '[' << value.x << ", " << value.y << ']';
    return os;
}

std::ostream & operator<<(std::ostream & os, Coordinates2d::Position const & value)
{
    os << '(' << value.x << ", " << value.y << ')';
    return os;
}
