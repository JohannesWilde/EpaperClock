#include "renderer2d.hpp"

Renderer2d::ValidityAndColor Renderer2dAxesAlignedRectangle::evaluate(int x, int y) const
{
    ValidityAndColor result;

    if ((smallestCoordinate_.x <= x) &&
        (biggestCoordinate_.x >= x)  &&
        (smallestCoordinate_.y <= y) &&
        (biggestCoordinate_.y >= y))
    {
        result.valid = true;
        result.color = color_;
    }
    else
    {
        result.valid = false;
    }

    return result;
}
