#include "renderer2dextended.hpp"


void Renderer2dButton::render(Coordinates2d::Position const & offset,
            Coordinates2d::Dimension const & dimension,
            Color * const data) const
{
    rendererOutside_.render(offset, dimension, data);
    rendererInside_.render(offset, dimension, data);
}
