#ifndef RENDERER_2D_HPP
#define RENDERER_2D_HPP

#include "coordinates2d.hpp"

#include <array>
#include <cassert>
#include <cstdint>


class Renderer2d
{
public:

    typedef std::uint8_t Color;

    virtual void render(Coordinates2d::Position const & offset,
                        Coordinates2d::Dimension const & dimension,
                        Color * const data) const = 0;

protected:
    constexpr Renderer2d() = default;
    constexpr Renderer2d(Renderer2d const & other) = default;
    constexpr Renderer2d(Renderer2d && other) = default;
    constexpr Renderer2d & operator=(Renderer2d const & other) = default;
    constexpr Renderer2d & operator=(Renderer2d && other) = default;
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
    constexpr Renderer2dAxesAlignedRectangle(Coordinates2d::Position const corner0,
                                             Coordinates2d::Position const corner1,
                                             Renderer2d::Color const color)
        : color_(color)
    {
        sortMinMaxTo_(corner0.x, corner1.x, &smallestCoordinate_.x, &biggestCoordinate_.x);
        sortMinMaxTo_(corner0.y, corner1.y, &smallestCoordinate_.y, &biggestCoordinate_.y);
    }

    constexpr Renderer2dAxesAlignedRectangle(Coordinates2d::Position const corner,
                                             Coordinates2d::Dimension const dimension,
                                             Renderer2d::Color const color)
        : smallestCoordinate_(corner)
        , biggestCoordinate_(corner + Coordinates2d::Distance(dimension.getX(), dimension.getY()))
        , color_(color)
    {
        // intentionally empty
    }

    constexpr Renderer2dAxesAlignedRectangle(Renderer2dAxesAlignedRectangle const & other) = default;
    constexpr Renderer2dAxesAlignedRectangle(Renderer2dAxesAlignedRectangle && other) = default;
    constexpr Renderer2dAxesAlignedRectangle & operator=(Renderer2dAxesAlignedRectangle const & other) = default;
    constexpr Renderer2dAxesAlignedRectangle & operator=(Renderer2dAxesAlignedRectangle && other) = default;

    void render(Coordinates2d::Position const & offset,
                Coordinates2d::Dimension const & dimension,
                Color * const data) const override;

private:

    Coordinates2d::Position smallestCoordinate_;
    Coordinates2d::Position biggestCoordinate_;

    Renderer2d::Color color_;

};


class Renderer2dTriangle : public Renderer2d
{
public:
    constexpr Renderer2dTriangle(std::array<Coordinates2d::Position, 3> corners,
                                 Renderer2d::Color const color)
        : corners_(corners)
        , color_(color)
    {
        // intentionally empty
    }

    constexpr Renderer2dTriangle(Coordinates2d::Position const & corner0,
                                 Coordinates2d::Position const & corner1,
                                 Coordinates2d::Position const & corner2,
                                 Renderer2d::Color const color)
        : corners_{corner0, corner1, corner2, }
        , color_(color)
    {
        // intentionally empty
    }

    constexpr Renderer2dTriangle(Renderer2dTriangle const & other) = default;
    constexpr Renderer2dTriangle(Renderer2dTriangle && other) = default;
    constexpr Renderer2dTriangle & operator=(Renderer2dTriangle const & other) = default;
    constexpr Renderer2dTriangle & operator=(Renderer2dTriangle && other) = default;

    void render(Coordinates2d::Position const & offset,
                Coordinates2d::Dimension const & dimension,
                Color * const data) const override;

private:

    std::array<Coordinates2d::Position, 3> corners_;
    Renderer2d::Color color_;

};


class Renderer2dRelative : public Renderer2d
{
public:

    constexpr Renderer2dRelative(Renderer2d const * other,
                                 Coordinates2d::Position const & virtualOrigin)
        : renderer_(other)
        , offset_(-1 * virtualOrigin.x, -1 * virtualOrigin.y)
    {
        assert(nullptr != other);
    }

    constexpr Renderer2dRelative(Renderer2dRelative const & other) = default;
    constexpr Renderer2dRelative(Renderer2dRelative && other) = default;
    constexpr Renderer2dRelative & operator=(Renderer2dRelative const & other) = default;
    constexpr Renderer2dRelative & operator=(Renderer2dRelative && other) = default;

    void render(Coordinates2d::Position const & offset,
                Coordinates2d::Dimension const & dimension,
                Color * const data) const override;

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
    constexpr Renderer2dAccumulated(Args&&... others)
        : renderers_{others...}
    {
#ifndef NDEBUG
        for (Renderer2d const * const renderer : renderers_)
        {
            assert(nullptr != renderer);
        }
#endif // NDEBUG
    }

    constexpr Renderer2dAccumulated(Renderer2dAccumulated const & other) = default;
    constexpr Renderer2dAccumulated(Renderer2dAccumulated && other) = default;
    constexpr Renderer2dAccumulated & operator=(Renderer2dAccumulated const & other) = default;
    constexpr Renderer2dAccumulated & operator=(Renderer2dAccumulated && other) = default;


    void render(Coordinates2d::Position const & offset,
                Coordinates2d::Dimension const & dimension,
                Color * const data) const override
    {
        for (Renderer2d const * const renderer : renderers_)
        {
            renderer->render(offset, dimension, data);
        }
    }

private:

    std::array<Renderer2d const *, count> renderers_;

};


class Renderer2dEnabled : public Renderer2d
{
public:

    constexpr Renderer2dEnabled(Renderer2d const * other,
                                bool const enabled = true)
        : renderer_(other)
        , enabled_(enabled)
    {
        // intentionally empty
    }

    constexpr Renderer2dEnabled(Renderer2dEnabled const & other) = default;
    constexpr Renderer2dEnabled(Renderer2dEnabled && other) = default;
    constexpr Renderer2dEnabled & operator=(Renderer2dEnabled const & other) = default;
    constexpr Renderer2dEnabled & operator=(Renderer2dEnabled && other) = default;

    void render(Coordinates2d::Position const & offset,
                Coordinates2d::Dimension const & dimension,
                Color * const data) const override;

    constexpr void set(bool const enabled)
    {
        enabled_ = enabled;
    }

    constexpr bool get() const
    {
        return enabled_;
    }

private:

    Renderer2d const * renderer_;
    bool enabled_;

};


#endif // RENDERER_2D_HPP
