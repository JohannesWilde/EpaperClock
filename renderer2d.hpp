#ifndef RENDERER_2D_HPP
#define RENDERER_2D_HPP

#include "coordinates2d.hpp"

#include <cassert>
#include <array>


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

    virtual ValidityAndColor evaluate(Coordinates2d::Position const & position) const = 0;

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
                                   Coordinates2d::Dimension const dimension,
                                   Renderer2d::Color const color)
        : smallestCoordinate_(corner)
        , biggestCoordinate_(corner + Coordinates2d::Distance(dimension.getX(), dimension.getY()))
        , color_(color)
    {
        // intentionally empty
    }

    Renderer2dAxesAlignedRectangle(Renderer2dAxesAlignedRectangle const & other) = default;
    Renderer2dAxesAlignedRectangle(Renderer2dAxesAlignedRectangle && other) = default;
    Renderer2dAxesAlignedRectangle & operator=(Renderer2dAxesAlignedRectangle const & other) = default;
    Renderer2dAxesAlignedRectangle & operator=(Renderer2dAxesAlignedRectangle && other) = default;

    ValidityAndColor evaluate(Coordinates2d::Position const & position) const override;

private:

    Coordinates2d::Position smallestCoordinate_;
    Coordinates2d::Position biggestCoordinate_;

    Renderer2d::Color color_;

};


class Renderer2dTriangle : public Renderer2d
{
public:
    Renderer2dTriangle(std::array<Coordinates2d::Position, 3> corners,
                       Renderer2d::Color const color)
        : corners_(corners)
        , color_(color)
    {
        // intentionally empty
    }

    Renderer2dTriangle(Coordinates2d::Position const & corner0,
                       Coordinates2d::Position const & corner1,
                       Coordinates2d::Position const & corner2,
                       Renderer2d::Color const color)
        : corners_{corner0, corner1, corner2, }
        , color_(color)
    {
        // intentionally empty
    }

    Renderer2dTriangle(Renderer2dTriangle const & other) = default;
    Renderer2dTriangle(Renderer2dTriangle && other) = default;
    Renderer2dTriangle & operator=(Renderer2dTriangle const & other) = default;
    Renderer2dTriangle & operator=(Renderer2dTriangle && other) = default;

    ValidityAndColor evaluate(Coordinates2d::Position const & position) const override;

private:

    std::array<Coordinates2d::Position, 3> corners_;
    Renderer2d::Color color_;

};


class Renderer2dRelative : public Renderer2d
{
public:

    Renderer2dRelative(Renderer2d const * other,
                       Coordinates2d::Position const & virtualOrigin)
        : renderer_(other)
        , offset_(-1 * virtualOrigin.x, -1 * virtualOrigin.y)
    {
        assert(nullptr != other);
    }

    Renderer2dRelative(Renderer2dRelative const & other) = default;
    Renderer2dRelative(Renderer2dRelative && other) = default;
    Renderer2dRelative & operator=(Renderer2dRelative const & other) = default;
    Renderer2dRelative & operator=(Renderer2dRelative && other) = default;

    ValidityAndColor evaluate(Coordinates2d::Position const & position) const override;

private:

    Renderer2d const * renderer_;
    Coordinates2d::Distance offset_;

};


template<size_t count>
class Renderer2dAccumulated : public Renderer2d
{
public:

    // None of the provided renderers must be nullptr.
    template<typename... Args>
    Renderer2dAccumulated(Args&&... others)
        : renderers_(others...)
    {
#ifndef NDEBUG
        for (Renderer2d const * const renderer : renderers_)
        {
            assert(nullptr != renderer);
        }
#endif // NDEBUG
    }

    Renderer2dAccumulated(Renderer2dAccumulated const & other) = default;
    Renderer2dAccumulated(Renderer2dAccumulated && other) = default;
    Renderer2dAccumulated & operator=(Renderer2dAccumulated const & other) = default;
    Renderer2dAccumulated & operator=(Renderer2dAccumulated && other) = default;

    ValidityAndColor evaluate(Coordinates2d::Position const & position) const override
    {
        Renderer2d::ValidityAndColor renderResult;

        for (Renderer2d const * const renderer : renderers_)
        {
            renderResult = renderer->evaluate(position);
            if (renderResult.valid)
            {
                break; // Don't look at further renderers.
            }
            else
            {
                // intentionally empty
            }
        }

        return renderResult;
    }

private:

    std::array<Renderer2d const *, count> renderers_;

};


#endif // RENDERER_2D_HPP
