#include "renderer2d.hpp"

#include "constexprAbs.hpp"

#include <cassert>
#ifndef NDEBUG
#include <limits>
#endif // NDEBUG

namespace // anonymous
{

constexpr int roundedDivision(int const nominator, int const denominator)
{
    // Round away from 0.
    int rounded = 0;
    if (((nominator > 0) && (denominator > 0)) ||
        ((nominator < 0) && (denominator < 0)))
    {
        rounded = nominator + (denominator / 2); // todo: handle int-overflow
    }
    else
    {
        rounded = nominator - (denominator / 2); // todo: handle int-overflow
    }
    return rounded / denominator;
}

// deltaLine.x must not be 0.
constexpr int extraInterpolateX(Coordinates2d::Position const & linePoint0,
                                Coordinates2d::Distance const & deltaLine,
                                int const y)
{
    assert(0 != deltaLine.y);
    assert((y == linePoint0.y) || (constexpr_abs(deltaLine.y) < constexpr_abs(std::numeric_limits<int>::max() / (y - linePoint0.y)))); // todo: handle int-overflow

    int const deltaYScaled = deltaLine.x * (y - linePoint0.y); // todo: handle int-overflow

    int const deltaXScaled = roundedDivision(deltaYScaled, deltaLine.y);

    return deltaXScaled + linePoint0.x; // todo: handle int-overflow
}

// deltaLine.x must not be 0.
constexpr int extraInterpolateY(Coordinates2d::Position const & linePoint0,
                                Coordinates2d::Distance const & deltaLine,
                                int const x)
{
    assert(0 != deltaLine.x);
    assert((x == linePoint0.x) || (constexpr_abs(deltaLine.y) < constexpr_abs(std::numeric_limits<int>::max() / (x - linePoint0.x)))); // todo: handle int-overflow

    int const deltaXScaled = deltaLine.y * (x - linePoint0.x); // todo: handle int-overflow

    int const deltaYScaled = roundedDivision(deltaXScaled, deltaLine.x);

    return deltaYScaled + linePoint0.y; // todo: handle int-overflow
}

bool checkSameSideOfLine_(Coordinates2d::Position const & linePoint0,
                          Coordinates2d::Position const & linePoint1,
                          Coordinates2d::Position const & sidePoint,
                          Coordinates2d::Position const & checkPoint)
{
    int deltaSide = 0;
    int deltaCheck = 0;

    Coordinates2d::Distance const deltaLine = linePoint1.distanceTo(linePoint0);
    if (0 == deltaLine.x)
    {
        // "Vertical" line - don't calculate gradient of infinity but simply compare to the 1 x-Value.
        deltaSide = sidePoint.x - linePoint0.x;
        deltaCheck = checkPoint.x - linePoint0.x;
    }
    else
    {
        // Otherwise check with extrapolated y-Value along the line at the positions of sidePoint and checkPoint.
        int const sideYReference = extraInterpolateY(linePoint0, deltaLine, sidePoint.x);
        int const checkYReference = extraInterpolateY(linePoint0, deltaLine, checkPoint.x);

        deltaSide = sidePoint.y - sideYReference; // todo: handle int-overflow
        deltaCheck = checkPoint.y - checkYReference; // todo: handle int-overflow
    }

    bool result = false;

    if (0 < deltaSide)
    {
        result = (0 <= deltaCheck);
    }
    else if (0 > deltaSide)
    {
        result = (0 >= deltaCheck);
    }
    else
    {
        result = (0 == deltaCheck);
    }

    return result;
}

} // namespace anonymous



void Renderer2dAxesAlignedRectangle::render(Coordinates2d::Position const & offset,
                                            Coordinates2d::Dimension const & dimension,
                                            Color * const data) const
{
    int const renderYStart = offset.y;
    int const renderYEnd = offset.y + dimension.getY();

    int const rectangleYStart = 0;
    int const rectangleYEnd = dimension_.getY();

    int const actualYStart = std::max(renderYStart, rectangleYStart);
    int const actualYEnd = std::min(renderYEnd, rectangleYEnd);

    int const relativeYStart = actualYStart - renderYStart;
    int const relativeYEnd = actualYEnd - renderYStart;


    int const renderXStart = offset.x;
    int const renderXEnd = offset.x + dimension.getX();

    int const rectangleXStart = 0;
    int const rectangleXEnd = dimension_.getX();

    int const actualXStart = std::max(renderXStart, rectangleXStart);
    int const actualXEnd = std::min(renderXEnd, rectangleXEnd);

    int const relativeXStart = actualXStart - renderXStart;
    int const relativeXEnd = actualXEnd - renderXStart;


    for (int y = relativeYStart; relativeYEnd > y; ++y)
    {
        for (int x = relativeXStart; relativeXEnd > x; ++x)
        {
            data[y * dimension.getX() + x] = color_;
        }
    }
}

constexpr Renderer2dTriangle::Renderer2dTriangle(std::array<Coordinates2d::Position, 3> corners,
                                                 Color const color)
    : corners_(corners)
    , color_(color)
{
    // sort ascending in x [very inefficiently - but only once during construction]
    for (size_t index = 0; (corners_.size() - 1) > index; )
    {
        size_t const nextIndex = index + 1;
        if ((corners_[nextIndex].x < corners_[index].x) ||
            ((corners_[nextIndex].x == corners_[index].x) && (corners_[nextIndex].y < corners_[index].y)))
        {
            // swap entries
            {
                int const temp = corners_[index].x;
                corners_[index].x = corners_[nextIndex].x;
                corners_[nextIndex].x = temp;
            }

            {
                int const temp = corners_[index].y;
                corners_[index].y = corners_[nextIndex].y;
                corners_[nextIndex].y = temp;
            }

            // re-iterate swapping
            index = 0;
        }
        else
        {
            ++index;
        }
    }
}

void Renderer2dTriangle::render(Coordinates2d::Position const & offset,
                                Coordinates2d::Dimension const & dimension,
                                Color * const data) const
{
    // ValidityAndColor result;

    // // First check enclosing axes-aligned rectangle to contain the position [necessary condition].
    // if (((corners_[0].x <= position.x) || (corners_[1].x <= position.x) || (corners_[2].x <= position.x)) &&
    //     ((corners_[0].x >= position.x) || (corners_[1].x >= position.x) || (corners_[2].x >= position.x)) &&
    //     ((corners_[0].y <= position.y) || (corners_[1].y <= position.y) || (corners_[2].y <= position.y)) &&
    //     ((corners_[0].y >= position.y) || (corners_[1].y >= position.y) || (corners_[2].y >= position.y)))
    // {
    //     // Now check the point is actually contained in the triangle [sufficient condition].
    //     bool insideTriangle = true;

    //     if (insideTriangle)
    //     {
    //         insideTriangle = checkSameSideOfLine_(corners_[0],
    //                                               corners_[1],
    //                                               corners_[2],
    //                                               position);
    //     }
    //     else
    //     {
    //         // intentionally empty
    //     }
    //     if (insideTriangle)
    //     {
    //         insideTriangle = checkSameSideOfLine_(corners_[0],
    //                                               corners_[2],
    //                                               corners_[1],
    //                                               position);
    //     }
    //     else
    //     {
    //         // intentionally empty
    //     }
    //     if (insideTriangle)
    //     {
    //         insideTriangle = checkSameSideOfLine_(corners_[1],
    //                                               corners_[2],
    //                                               corners_[0],
    //                                               position);
    //     }
    //     else
    //     {
    //         // intentionally empty
    //     }

    //     if (insideTriangle)
    //     {
    //         result.valid = true;
    //         result.color = color_;
    //     }
    //     else
    //     {
    //         result.valid = false;
    //     }
    // }
    // else
    // {
    //     result.valid = false;
    // }

    // return result;
}

void Renderer2dRelative::render(Coordinates2d::Position const & offset,
                                Coordinates2d::Dimension const & dimension,
                                Color * const data) const
{
    Coordinates2d::Position const shiftedOffset = offset + offset_;
    renderer_->render(shiftedOffset, dimension, data);
}


void Renderer2dEnabled::render(Coordinates2d::Position const & offset,
                               Coordinates2d::Dimension const & dimension,
                               Color * const data) const
{
    if (enabled_)
    {
        renderer_->render(offset, dimension, data);
    }
    else
    {
        // intentionally empty
    }
}
