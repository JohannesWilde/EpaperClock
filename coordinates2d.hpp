#ifndef COORDINATES_2D_HPP
#define COORDINATES_2D_HPP

#include <iostream>

namespace Coordinates2d
{

class Position;


class Dimension
{
public:
    constexpr Dimension()
        : x_(0)
        , y_(0)
    {
        // intentionally empty
    }

    Dimension(int const x, int const y)
        : x_(x)
        , y_(y)
    {
        if ((x < 0) || (y < 0))
        {
            throw std::runtime_error("Dimension must not be negative.");
        }
    }

    constexpr Dimension(Dimension const & other) = default;
    constexpr Dimension(Dimension && other) = default;
    constexpr Dimension & operator=(Dimension const & other) = default;
    constexpr Dimension & operator=(Dimension && other) = default;

    Dimension operator+(Dimension const & other) const
    {
        return Dimension{x_ + other.x_, y_ + other.y_};
    }

    Dimension operator-(Dimension const & other) const
    {
        if ((other.x_ > x_) || (other.y_ > y_))
        {
            throw std::runtime_error("Dimension can't be subtracted - too big in at least one dimension.");
        }
        return Dimension{x_ - other.x_, y_ - other.y_};
    }

    constexpr Dimension & operator+=(Dimension const & other)
    {
          x_ += other.x_;
          y_ += other.y_;
          return *this;
    }

    Dimension & operator-=(Dimension const & other)
    {
        if ((other.x_ > x_) || (other.y_ > y_))
        {
            throw std::runtime_error("Dimension can't be subtracted - too big in at least one dimension.");
        }
        x_ -= other.x_;
        y_ -= other.y_;
        return *this;
    }

    constexpr bool operator==(Dimension const & other) const
    {
        return ((x_ == other.x_) && (y_ == other.y_));
    }

    constexpr int getX() const
    {
        return x_;
    }

    constexpr int getY() const
    {
        return y_;
    }

    void setX(int const value)
    {
        if (0 > value)
        {
            throw std::runtime_error("x must not be negative.");
        }
        x_ = value;
    }

    void setY(int const value)
    {
        if (0 > value)
        {
            throw std::runtime_error("y must not be negative.");
        }
        y_ = value;
    }

    constexpr int size() const
    {
        return x_ * y_;
    }

    constexpr bool contains(Position const & position) const;

private:

    int x_;
    int y_;

};


class Distance
{
public:
    constexpr Distance()
        : x(0)
        , y(0)
    {
    }

    constexpr Distance(int const x, int const y)
        : x(x)
        , y(y)
    {
    }

    constexpr Distance(Distance const & other) = default;
    constexpr Distance(Distance && other) = default;
    constexpr Distance & operator=(Distance const & other) = default;
    constexpr Distance & operator=(Distance && other) = default;

    constexpr Distance(Position const & first, Position const & second);

    double norm() const;

    constexpr Distance operator-() const
    {
        return Distance{-x, -y};
    }

    constexpr Distance operator+(Distance const & other) const
    {
        return Distance{x + other.x, y + other.y};
    }

    constexpr Distance operator-(Distance const & other) const
    {
        return *this + (-other);
    }

    constexpr Distance & operator+=(Distance const & other)
    {
          x += other.x;
          y += other.y;
          return *this;
    }

    constexpr Distance & operator-=(Distance const & other)
    {
        *this += -other;
        return *this;
    }

    constexpr bool operator==(Distance const & other) const
    {
        return ((x == other.x) && (y == other.y));
    }

    constexpr Distance operator*(int const factor) const
    {
        return Distance{x * factor, y * factor};
    }

    constexpr Distance operator/(int const factor) const
    {
        return Distance{x / factor, y / factor};
    }

    constexpr Distance & operator*=(int const factor)
    {
          x *= factor;
          y *= factor;
          return *this;
    }

    constexpr Distance & operator/=(int const factor)
    {
        x /= factor;
        y /= factor;
        return *this;
    }

    int x;
    int y;

};

class Position
{
public:
    constexpr Position()
        : x(0)
        , y(0)
    {
    }

    constexpr Position(int const x, int const y)
        : x(x)
        , y(y)
    {
    }

    constexpr Position(Position const & other) = default;
    constexpr Position(Position && other) = default;
    constexpr Position & operator=(Position const & other) = default;
    constexpr Position & operator=(Position && other) = default;

    constexpr Distance distanceTo(Position const & other = Position(0, 0)) const
    {
        return Distance(x - other.x, y - other.y);
    }

    constexpr Position operator-() const
    {
        return Position{-x, -y};
    }

    constexpr Position operator+(Distance const & other) const
    {
        return Position{x + other.x, y + other.y};
    }

    constexpr Position operator-(Distance const & other) const
    {
        return *this + (-other);
    }

    constexpr Position & operator+=(Distance const & other)
    {
          x += other.x;
          y += other.y;
          return *this;
    }

    Position & operator-=(Distance const & other)
    {
        *this += -other;
        return *this;
    }

    constexpr bool operator==(Position const & other) const
    {
        return ((x == other.x) && (y == other.y));
    }

    int x;
    int y;
};

} // namespace Coordinates2d

std::ostream & operator<<(std::ostream & os, Coordinates2d::Dimension const & value);
std::ostream & operator<<(std::ostream & os, Coordinates2d::Distance const & value);
std::ostream & operator<<(std::ostream & os, Coordinates2d::Position const & value);

#endif // COORDINATES_2D_HPP
