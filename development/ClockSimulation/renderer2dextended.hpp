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
        : rendererOutside_(outsideDimension,
                           outsideColor)
        , rendererInside_(Coordinates2d::Dimension(outsideDimension.getX() - 2 * offsetInside,
                                                   outsideDimension.getY() - 2 * offsetInside),
                          insideColor)
        , rendererInsideRelative_(&rendererInside_,
                                  Coordinates2d::Position(offsetInside, offsetInside))
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
    Renderer2dRelative rendererInsideRelative_;

};

#endif // RENDERER_2D_EXTENDED_HPP
