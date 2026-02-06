#include "renderer2d.hpp"

#include <cassert>


namespace // anonymous
{

// deltaLine.x must not be 0.
constexpr int extraInterpolateY(Coordinates2d::Position const & linePoint0,
                                Coordinates2d::Distance const & deltaLine,
                                int const x)
{
    assert(0 != deltaLine.x);

    int const deltaScaled = deltaLine.y * (x - linePoint0.x);

    // Round away from 0.
    int deltaRound = 0;
    if (((deltaScaled > 0) && (deltaLine.x > 0)) ||
        ((deltaScaled < 0) && (deltaLine.x < 0)))
    {
        deltaRound = deltaScaled + (deltaLine.x / 2);
    }
    else
    {
        deltaRound = deltaScaled - (deltaLine.x / 2);
    }

    return deltaRound / deltaLine.x + linePoint0.y;
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

        // todo: handle int-overflow
        deltaSide = sidePoint.y - sideYReference;
        deltaCheck = checkPoint.y - checkYReference;
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


Renderer2d::ValidityAndColor Renderer2dAxesAlignedRectangle::evaluate(Coordinates2d::Position const & position) const
{
    ValidityAndColor result;

    if ((smallestCoordinate_.x <= position.x) &&
        (biggestCoordinate_.x >= position.x)  &&
        (smallestCoordinate_.y <= position.y) &&
        (biggestCoordinate_.y >= position.y))
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

Renderer2d::ValidityAndColor Renderer2dTriangle::evaluate(Coordinates2d::Position const & position) const
{
    ValidityAndColor result;

    // First check enclosing axes-aligned rectangle to contain the position [necessary condition].
    if (((corners_[0].x <= position.x) || (corners_[1].x <= position.x) || (corners_[2].x <= position.x)) &&
        ((corners_[0].x >= position.x) || (corners_[1].x >= position.x) || (corners_[2].x >= position.x)) &&
        ((corners_[0].y <= position.y) || (corners_[1].y <= position.y) || (corners_[2].y <= position.y)) &&
        ((corners_[0].y >= position.y) || (corners_[1].y >= position.y) || (corners_[2].y >= position.y)))
    {
        // Now check the point is actually contained in the triangle [sufficient condition].
        bool insideTriangle = true;

        if (insideTriangle)
        {
            insideTriangle = checkSameSideOfLine_(corners_[0],
                                                  corners_[1],
                                                  corners_[2],
                                                  position);
        }
        else
        {
            // intentionally empty
        }
        if (insideTriangle)
        {
            insideTriangle = checkSameSideOfLine_(corners_[0],
                                                  corners_[2],
                                                  corners_[1],
                                                  position);
        }
        else
        {
            // intentionally empty
        }
        if (insideTriangle)
        {
            insideTriangle = checkSameSideOfLine_(corners_[1],
                                                  corners_[2],
                                                  corners_[0],
                                                  position);
        }
        else
        {
            // intentionally empty
        }

        if (insideTriangle)
        {
            result.valid = true;
            result.color = color_;
        }
        else
        {
            result.valid = false;
        }
    }
    else
    {
        result.valid = false;
    }

    return result;
}
