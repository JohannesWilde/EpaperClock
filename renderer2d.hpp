#ifndef RENDERER_2D_HPP
#define RENDERER_2D_HPP

#include "coordinates2d.hpp"


class Renderer2d
{
public:

    typedef uint8_t Color;

    struct ValidityAndColor
    {
        ValidityAndColor()
            : valid(false)
            , color(0)
        {
            // inentionally empty
        }
        ValidityAndColor(bool const valid, Color const color)
            : valid(valid)
            , color(color)
        {
            // inentionally empty
        }

        ValidityAndColor(ValidityAndColor const & other) = default;
        ValidityAndColor(ValidityAndColor && other) = default;
        ValidityAndColor & operator=(ValidityAndColor const & other) = default;
        ValidityAndColor & operator=(ValidityAndColor && other) = default;

        bool valid;
        Color color;
    };

    virtual ValidityAndColor evaluate(int x, int y) const = 0;

protected:
    Renderer2d() = default;
    Renderer2d(Renderer2d const & other) = default;
    Renderer2d(Renderer2d && other) = default;
    Renderer2d & operator=(Renderer2d const & other) = default;
    Renderer2d & operator=(Renderer2d && other) = default;
};


class Renderer2dAxesAlignedRectangle : public Renderer2d
{
    constexpr void sortMinMaxTo_(int const val0, int const val1,
                                 int * const outMin, int * const outMax)
    {
        if (val0 <= val1)
        {
            *outMin = val0;
            *outMax = val1;
        }
        else
        {
            *outMin = val1;
            *outMax = val0;
        }
    }

public:
    Renderer2dAxesAlignedRectangle(Coordinates2d::Position const corner0,
                      Coordinates2d::Position const corner1,
                      Renderer2d::Color const color)
        : color_(color)
    {
        sortMinMaxTo_(corner0.x, corner1.x, &smallestCoordinate_.x, &biggestCoordinate_.x);
        sortMinMaxTo_(corner0.y, corner1.y, &smallestCoordinate_.y, &biggestCoordinate_.y);
    }

    Renderer2dAxesAlignedRectangle(Coordinates2d::Position const corner,
                      Coordinates2d::Dimension const dimension)
        : smallestCoordinate_(corner)
        , biggestCoordinate_(corner + Coordinates2d::Distance(dimension.getX(), dimension.getY()))
    {
        // intentionally empty
    }

    ValidityAndColor evaluate(int x, int y) const override;

private:

    Coordinates2d::Position smallestCoordinate_;
    Coordinates2d::Position biggestCoordinate_;

    Renderer2d::Color color_;

};


// class Render2dTriangle : public Renderer2d
// {
// public:

//     ValidityAndColor evaluate(int x, int y) const override;

// private:

// };




#endif // RENDERER_2D_HPP
