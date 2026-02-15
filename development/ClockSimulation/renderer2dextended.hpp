#ifndef RENDERER_2D_EXTENDED_HPP
#define RENDERER_2D_EXTENDED_HPP

#include "constexprAbs.hpp"
#include "renderer2d.hpp"


class Renderer2dButton : public Renderer2d
{
public:

    Renderer2dButton(Coordinates2d::Position const outsideCorner0,
                     Coordinates2d::Position const outsideCorner1,
                     Renderer2d::Color const outsideColor,
                     int const offsetInside,
                     Renderer2d::Color const insideColor)
        : rendererOutside_(outsideCorner0,
                           outsideCorner1,
                           outsideColor)
        , rendererInside_(outsideCorner0 + Coordinates2d::Distance(offsetInside, offsetInside),
                          Coordinates2d::Dimension(constexpr_abs(outsideCorner1.x - outsideCorner0.x) - 2 * offsetInside,
                                                   constexpr_abs(outsideCorner1.y - outsideCorner0.y) - 2 * offsetInside),
                          insideColor)
    {
        assert(0 <= offsetInside);
        assert(constexpr_abs(outsideCorner0.x - outsideCorner1.x) / 2 >= offsetInside);
        assert(constexpr_abs(outsideCorner0.y - outsideCorner1.y) / 2 >= offsetInside);
    }

    Renderer2dButton(Coordinates2d::Position const outsideCorner,
                     Coordinates2d::Dimension const outsideDimension,
                     Renderer2d::Color const outsideColor,
                     int const offsetInside,
                     Renderer2d::Color const insideColor)
        : Renderer2dButton(outsideCorner,
                           outsideCorner + Coordinates2d::Distance(outsideDimension.getX(), outsideDimension.getY()),
                           outsideColor,
                           offsetInside,
                           insideColor)
    {
        // intentionally empty
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
