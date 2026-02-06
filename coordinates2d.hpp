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

    Dimension(Dimension const & other) = default;
    Dimension(Dimension && other) = default;
    Dimension & operator=(Dimension const & other) = default;
    Dimension & operator=(Dimension && other) = default;

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

    Dimension & operator+=(Dimension const & other)
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

    bool operator==(Dimension const & other) const
    {
        return ((x_ == other.x_) && (y_ == other.y_));
    }

    int getX() const
    {
        return x_;
    }

    int getY() const
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

    int size() const
    {
        return x_ * y_;
    }

    bool contains(Position const & position) const;

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

    Distance(Distance const & other) = default;
    Distance(Distance && other) = default;
    Distance & operator=(Distance const & other) = default;
    Distance & operator=(Distance && other) = default;

    Distance(Position const & first, Position const & second);

    double norm() const;

    Distance operator-() const
    {
        return Distance{-x, -y};
    }

    Distance operator+(Distance const & other) const
    {
        return Distance{x + other.x, y + other.y};
    }

    Distance operator-(Distance const & other) const
    {
        return *this + (-other);
    }

    Distance & operator+=(Distance const & other)
    {
          x += other.x;
          y += other.y;
          return *this;
    }

    Distance & operator-=(Distance const & other)
    {
        *this += -other;
        return *this;
    }

    bool operator==(Distance const & other) const
    {
        return ((x == other.x) && (y == other.y));
    }

    Distance operator*(int const factor) const
    {
        return Distance{x * factor, y * factor};
    }

    Distance operator/(int const factor) const
    {
        return Distance{x / factor, y / factor};
    }

    Distance & operator*=(int const factor)
    {
          x *= factor;
          y *= factor;
          return *this;
    }

    Distance & operator/=(int const factor)
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
    Position()
        : x(0)
        , y(0)
    {
    }

    Position(int const x, int const y)
        : x(x)
        , y(y)
    {
    }

    Position(Position const & other) = default;
    Position(Position && other) = default;
    Position & operator=(Position const & other) = default;
    Position & operator=(Position && other) = default;

    Distance distanceTo(Position const & other = Position(0, 0)) const
    {
        return Distance(x - other.x, y - other.y);
    }

    Position operator-() const
    {
        return Position{-x, -y};
    }

    Position operator+(Distance const & other) const
    {
        return Position{x + other.x, y + other.y};
    }

    Position operator-(Distance const & other) const
    {
        return *this + (-other);
    }

    Position & operator+=(Distance const & other)
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

    bool operator==(Position const & other) const
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
