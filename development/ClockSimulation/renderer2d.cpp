#include "renderer2d.hpp"

#include "constexprAbs.hpp"

#include <cassert>
#ifndef NDEBUG
#include <limits>
#endif // NDEBUG

namespace // anonymous
{

// deltaLine.x must not be 0.
constexpr int extraInterpolateY(Coordinates2d::Position const & linePoint0,
                                Coordinates2d::Distance const & deltaLine,
                                int const x)
{
    assert(0 != deltaLine.x);
    assert((x == linePoint0.x) || (constexpr_abs(deltaLine.y) < constexpr_abs(std::numeric_limits<int>::max() / (x - linePoint0.x)))); // todo: handle int-overflow

    int const deltaScaled = deltaLine.y * (x - linePoint0.x);

    // Round away from 0.
    int deltaRound = 0;
    if (((deltaScaled > 0) && (deltaLine.x > 0)) ||
        ((deltaScaled < 0) && (deltaLine.x < 0)))
    {
        deltaRound = deltaScaled + (deltaLine.x / 2); // todo: handle int-overflow
    }
    else
    {
        deltaRound = deltaScaled - (deltaLine.x / 2); // todo: handle int-overflow
    }

    return deltaRound / deltaLine.x + linePoint0.y; // todo: handle int-overflow
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

    int const rectangleYStart = smallestCoordinate_.y;
    int const rectangleYEnd = biggestCoordinate_.y;

    int const actualYStart = std::max(renderYStart, rectangleYStart);
    int const actualYEnd = std::min(renderYEnd, rectangleYEnd);

    int const relativeYStart = actualYStart - renderYStart;
    int const relativeYEnd = actualYEnd - renderYStart;


    int const renderXStart = offset.x;
    int const renderXEnd = offset.x + dimension.getX();

    int const rectangleXStart = smallestCoordinate_.x;
    int const rectangleXEnd = biggestCoordinate_.x;

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
