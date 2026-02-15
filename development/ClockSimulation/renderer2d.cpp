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

void Renderer2dTriangle::render(Coordinates2d::Position const & offset,
                                Coordinates2d::Dimension const & dimension,
                                Color * const data) const
{
    // determine corners to check

    // y ^
    //   |       2
    //   |      /\
    //   |     /  \
    //   |    / _ - 3
    //   |   /-
    //   |  1
    //  -|-------------> x
    Coordinates2d::Position const dataMax = offset + Coordinates2d::Distance(dimension.getX(), dimension.getY());

    // 1. check in y-range
    if ((corners_[indicesYOrder_.min].y <= dataMax.y) &&
        (corners_[indicesYOrder_.max].y >= offset.y))
    {
        // 2. check in x bounding rectangle
        if ((corners_[/*x-min*/ 0].x <= dataMax.x) &&
            (corners_[/*x-max*/ 2].x >= offset.x))
        {
            int const yMin = std::max(corners_[indicesYOrder_.min].y, offset.y);
            int const yMax = std::min(corners_[indicesYOrder_.max].y, dataMax.y);
            int const yCenter = corners_[indicesYOrder_.center].y;

            for (int y = yMin; yMax >= y; ++y)
            {
                int triangleXBegin = 0;
                int triangleXEnd = 0;

                // 3. Check min [1] --- max [2] line.
                Coordinates2d::Distance const deltaLine1 = corners_[indicesYOrder_.min].distanceTo(corners_[indicesYOrder_.max]);
                if (0 == deltaLine1.y)
                {
                    // x-line or single point - don't calculate gradient of infinity but simply compare to the 1 y-Value.
                    triangleXBegin = corners_[/*x-min*/ 0].x;
                    triangleXEnd = corners_[/*x-max*/ 2].x;
                }
                else
                {
                    // 4. determine y-center [3 in above graphic]
                    size_t indexReference = std::numeric_limits<size_t>::max();
                    if (yCenter <= y)
                    {
                        // a. y-center <= y: min [1] --- max [2], *max [2]* --- center [3]
                        indexReference = indicesYOrder_.max;
                    }
                    else
                    {
                        // b. y-center >  y: min [1] --- max [2], *min [1]* --- center [3]
                        indexReference = indicesYOrder_.min;
                    }

                    Coordinates2d::Distance const deltaLine2 = corners_[indicesYOrder_.center].distanceTo(corners_[indexReference]);
                    if (0 == deltaLine2.y)
                    {
                        // "Horizontal" line - don't calculate gradient of infinity but simply compare to the 1 y-Value.
                        triangleXBegin = corners_[indicesYOrder_.center].x;
                        triangleXEnd = corners_[indexReference].x;
                    }
                    else
                    {
                        triangleXBegin = extraInterpolateX(corners_[indicesYOrder_.min], deltaLine1, y);
                        triangleXEnd = extraInterpolateX(corners_[indicesYOrder_.center], deltaLine2, y);
                    }
                }

                if (triangleXBegin > triangleXEnd)
                {
                    std::swap(triangleXBegin, triangleXEnd);
                }
                else
                {
                    // intentionally empty
                }

                // draw
                int const relativeY = y - offset.y;

                int const relativeXStart = triangleXBegin - offset.x;
                int const relativeXEnd = triangleXEnd - offset.x;

                for (int x = relativeXStart; relativeXEnd > x; ++x)
                {
                    data[relativeY * dimension.getX() + x] = color_;
                }
            }
        }
        else
        {
            // intentionally empty
        }
    }
    else
    {
        // intentionally empty
    }
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
