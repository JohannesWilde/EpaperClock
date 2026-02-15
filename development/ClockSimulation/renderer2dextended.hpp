#ifndef RENDERER_2D_EXTENDED_HPP
#define RENDERER_2D_EXTENDED_HPP

#include "constexprAbs.hpp"
#include "renderer2d.hpp"


class Renderer2dButton : public Renderer2d
{
public:

    Renderer2dButton(Coordinates2d::Dimension const & outsideDimension,
                     Renderer2d::Color const outsideColor,
                     int const offsetInside,
                     Renderer2d::Color const insideColor)
        : rendererOutside_(Coordinates2d::Position(0, 0),
                           Coordinates2d::Position(0, 0) + Coordinates2d::Distance(outsideDimension.getX(), outsideDimension.getY()),
                           outsideColor)
        , rendererInside_(rendererOutside_.getSmallestCoordinate() + Coordinates2d::Distance(offsetInside, offsetInside),
                          Coordinates2d::Dimension(constexpr_abs(rendererOutside_.getBiggestCoordinate().x - rendererOutside_.getSmallestCoordinate().x) - 2 * offsetInside,
                                                   constexpr_abs(rendererOutside_.getBiggestCoordinate().y - rendererOutside_.getSmallestCoordinate().y) - 2 * offsetInside),
                          insideColor)
    {
        assert(0 <= offsetInside);
    }

    Renderer2dButton(Renderer2dButton const & other) = default;
    Renderer2dButton(Renderer2dButton && other) = default;
    Renderer2dButton & operator=(Renderer2dButton const & other) = default;
    Renderer2dButton & operator=(Renderer2dButton && other) = default;

    void render(Coordinates2d::Position const & offset,
                Coordinates2d::Dimension const & dimension,
                Color * const data) const override;

private:

    Renderer2dAxesAlignedRectangle rendererOutside_;
    Renderer2dAxesAlignedRectangle rendererInside_;

};

#endif // RENDERER_2D_EXTENDED_HPP
