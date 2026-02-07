// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "helper.h"
#include "renderer2d.hpp"
#include "renderer2dextended.hpp"

#include <QPainter>
#include <QPaintEvent>
#include <QWidget>

#include <algorithm>
#include <memory>
#include <vector>

// #include <chrono>
// #include <iostream>


namespace // anonymous
{

class Colors
{
private:
    Colors() = delete;
public:
    static constexpr Renderer2d::Color black = 0;
    static constexpr Renderer2d::Color white = 255;
};


static constexpr int displayWidth = 264;
static constexpr int displayHeight = 176;
static constexpr int spareSpaceOutside = 0;
static constexpr int lengthButtons = 12;
static constexpr int lengthButtonsInternal = 6;
static constexpr int horizontalOffsetButtonsClock = 10;
static constexpr int widthSevenSegment = 10;
static constexpr int lengthXSevenSegmentSquare = 25;
static constexpr int lengthYSevenSegmentSquare = 59;
static constexpr int lengthSevenSegmentTriangle = 5;
static constexpr int innerSpacingSevenSegmentElements = 3;
static constexpr int horizontalLengthInterSegment = 10;
static constexpr int lengthSegmentDots = 15;
static constexpr int lengthElementSelection = 10;
static constexpr int xOffsetElementSelection = (widthSevenSegment + lengthXSevenSegmentSquare - lengthElementSelection) / 2 + innerSpacingSevenSegmentElements + lengthSevenSegmentTriangle;
static constexpr int yOffsetElementSelection = 4;
// constexpr int  = ;

// (0, 0) is at top left.
// x [first index - width] goes to the right. y [second index - height] goes down.

static constexpr int visibleX = (displayWidth - 2 * spareSpaceOutside);
static constexpr int visibleY = (displayHeight - 2 * spareSpaceOutside);

static constexpr int numberOfButtons = 4;
static constexpr int ySpacingButtons = (displayHeight - 2 * spareSpaceOutside) / numberOfButtons;
static constexpr int xOffsetButtonDown = spareSpaceOutside;
static constexpr int yOffsetButtonDown = spareSpaceOutside + ySpacingButtons / 2 - lengthButtons / 2;
static constexpr int offsetButtonDownInternal = (lengthButtons - lengthButtonsInternal) / 2;
static constexpr int xOffsetButtonDownInternal = xOffsetButtonDown + offsetButtonDownInternal;
static constexpr int yOffsetButtonDownInternal = yOffsetButtonDown + offsetButtonDownInternal;

static constexpr int xSpacingSevenSegment = lengthXSevenSegmentSquare + 2 * lengthSevenSegmentTriangle + 2 * innerSpacingSevenSegmentElements;
static constexpr int ySpacingSevenSegment = lengthYSevenSegmentSquare + 2 * lengthSevenSegmentTriangle + 2 * innerSpacingSevenSegmentElements;
static constexpr int xLengthSevenSegments = xSpacingSevenSegment + widthSevenSegment;
static constexpr int yLengthSevenSegments = 2 * ySpacingSevenSegment + widthSevenSegment;
static constexpr int xSpacingSevenSegments = xLengthSevenSegments + horizontalLengthInterSegment;
static constexpr int xOffsetSegment0 = xOffsetButtonDown + lengthButtons + horizontalOffsetButtonsClock;
static constexpr int yOffsetSegment = (visibleY - yLengthSevenSegments - (yOffsetElementSelection + lengthElementSelection)) / 2;
// constexpr int  = ;
// constexpr int  = ;
// constexpr int  = ;
// constexpr int  = ;
// constexpr int  = ;
// constexpr int  = ;
// constexpr int  = ;
// constexpr int  = ;

// Buttons

static Renderer2dButton const buttonSquare{Coordinates2d::Position(0, 0),
                                           Coordinates2d::Dimension(lengthButtons, lengthButtons),
                                           /*outsideColor*/ Colors::black,
                                           (lengthButtons - lengthButtonsInternal) / 2,
                                           /*insideColor*/ Colors::white};

static Renderer2dTriangle const buttonTriangleUp{Coordinates2d::Position(0,                   lengthButtons),
                                                 Coordinates2d::Position(lengthButtons / 2,   0),
                                                 Coordinates2d::Position(lengthButtons,       lengthButtons),
                                                 /*color*/ Colors::black};

static Renderer2dTriangle const buttonTriangleDown{Coordinates2d::Position(0,                   0),
                                                   Coordinates2d::Position(lengthButtons / 2,   lengthButtons),
                                                   Coordinates2d::Position(lengthButtons,       0),
                                                   /*color*/ Colors::black};

static Renderer2dTriangle const buttonTriangleLeft{Coordinates2d::Position(lengthButtons,       0),
                                                   Coordinates2d::Position(0,                   lengthButtons / 2),
                                                   Coordinates2d::Position(0,                   lengthButtons),
                                                   /*color*/ Colors::black};

static Renderer2dTriangle const buttonTriangleRight{Coordinates2d::Position(0,                   0),
                                                    Coordinates2d::Position(0,                   lengthButtons),
                                                    Coordinates2d::Position(lengthButtons,       lengthButtons / 2),
                                                    /*color*/ Colors::black};

// Element selection [triangle up]
static Renderer2dTriangle const elementSelection{Coordinates2d::Position(0,                             lengthElementSelection),
                                                 Coordinates2d::Position(lengthElementSelection / 2,    0),
                                                 Coordinates2d::Position(lengthElementSelection,        lengthElementSelection),
                                                 /*color*/ Colors::black};

// Seven segment display

static Renderer2dTriangle const sevenSegmentTriangleUp{Coordinates2d::Position(0,                          lengthSevenSegmentTriangle),
                                                       Coordinates2d::Position(widthSevenSegment / 2,      0),
                                                       Coordinates2d::Position(widthSevenSegment,          lengthSevenSegmentTriangle),
                                                       /*color*/ Colors::black};

static Renderer2dTriangle const sevenSegmentTriangleDown{Coordinates2d::Position(0,                        0),
                                                         Coordinates2d::Position(widthSevenSegment / 2,    lengthSevenSegmentTriangle),
                                                         Coordinates2d::Position(widthSevenSegment,        0),
                                                         /*color*/ Colors::black};

static Renderer2dTriangle const sevenSegmentTriangleLeft{Coordinates2d::Position(lengthSevenSegmentTriangle,    0),
                                                         Coordinates2d::Position(0,                             widthSevenSegment / 2),
                                                         Coordinates2d::Position(lengthSevenSegmentTriangle,    widthSevenSegment),
                                                         /*color*/ Colors::black};

static Renderer2dTriangle const sevenSegmentTriangleRight{Coordinates2d::Position(0,                            0),
                                                          Coordinates2d::Position(0,                            widthSevenSegment),
                                                          Coordinates2d::Position(lengthSevenSegmentTriangle,   widthSevenSegment / 2),
                                                          /*color*/ Colors::black};

static Renderer2dAxesAlignedRectangle const sevenSegmentXSquare{Coordinates2d::Position(0, 0),
                                                                Coordinates2d::Dimension(lengthXSevenSegmentSquare, widthSevenSegment),
                                                                /*color*/ 0};

static Renderer2dAxesAlignedRectangle const sevenSegmentYSquare{Coordinates2d::Position(0, 0),
                                                                Coordinates2d::Dimension(widthSevenSegment, lengthYSevenSegmentSquare),
                                                                /*color*/ 0};


static Renderer2dRelative const sevenSegmentTriangleLeftShifted{&sevenSegmentTriangleLeft, Coordinates2d::Position(innerSpacingSevenSegmentElements, 0)};
static Renderer2dRelative const sevenSegmentXSquareShifted{&sevenSegmentXSquare, Coordinates2d::Position(innerSpacingSevenSegmentElements + lengthSevenSegmentTriangle, 0)};
static Renderer2dRelative const sevenSegmentTriangleRightShifted{&sevenSegmentTriangleRight, Coordinates2d::Position(innerSpacingSevenSegmentElements + lengthSevenSegmentTriangle + lengthXSevenSegmentSquare, 0)};

static Renderer2dAccumulated<3> const sevenSegmentXElement{&sevenSegmentTriangleLeftShifted, &sevenSegmentXSquareShifted, &sevenSegmentTriangleRightShifted};


static Renderer2dRelative const sevenSegmentTriangleUpShifted{&sevenSegmentTriangleUp, Coordinates2d::Position(0, innerSpacingSevenSegmentElements)};
static Renderer2dRelative const sevenSegmentYSquareShifted{&sevenSegmentYSquare, Coordinates2d::Position(0, innerSpacingSevenSegmentElements + lengthSevenSegmentTriangle)};
static Renderer2dRelative const sevenSegmentTriangleDownShifted{&sevenSegmentTriangleDown, Coordinates2d::Position(0, innerSpacingSevenSegmentElements + lengthSevenSegmentTriangle + lengthYSevenSegmentSquare)};

static Renderer2dAccumulated<3> const sevenSegmentYElement{&sevenSegmentTriangleUpShifted, &sevenSegmentYSquareShifted, &sevenSegmentTriangleDownShifted};


static Renderer2dRelative const segmentShifted0_(&sevenSegmentYElement, Coordinates2d::Position(0, yOffsetSegment + widthSevenSegment / 2));
static Renderer2dRelative const segmentShifted1_(&sevenSegmentYElement, Coordinates2d::Position(0, yOffsetSegment + widthSevenSegment / 2 + ySpacingSevenSegment));
static Renderer2dRelative const segmentShifted2_(&sevenSegmentYElement, Coordinates2d::Position(xSpacingSevenSegment, yOffsetSegment + widthSevenSegment / 2));
static Renderer2dRelative const segmentShifted3_(&sevenSegmentYElement, Coordinates2d::Position(xSpacingSevenSegment, yOffsetSegment + widthSevenSegment / 2 + ySpacingSevenSegment));

static Renderer2dRelative const segmentShifted4_(&sevenSegmentXElement, Coordinates2d::Position(widthSevenSegment / 2, 0));
static Renderer2dRelative const segmentShifted5_(&sevenSegmentXElement, Coordinates2d::Position(widthSevenSegment / 2, ySpacingSevenSegment));
static Renderer2dRelative const segmentShifted6_(&sevenSegmentXElement, Coordinates2d::Position(widthSevenSegment / 2, 2 * ySpacingSevenSegment));

class Renderer2dSevenSegments : public Renderer2d
{
public:

    enum class Segment
    {
        top,
        middle,
        bottom,
        leftUp,
        leftDown,
        rightUp,
        rightDown
    };

    Renderer2dSevenSegments()
        : segmentsEnabled_{Renderer2dEnabled{&segmentsShifted_[0],},
                           Renderer2dEnabled{&segmentsShifted_[1],},
                           Renderer2dEnabled{&segmentsShifted_[2],},
                           Renderer2dEnabled{&segmentsShifted_[3],},
                           Renderer2dEnabled{&segmentsShifted_[4],},
                           Renderer2dEnabled{&segmentsShifted_[5],},
                           Renderer2dEnabled{&segmentsShifted_[6],},}
    {
        // intentionally empty
    }

    Renderer2dSevenSegments(Renderer2dSevenSegments const & other) = default;
    Renderer2dSevenSegments(Renderer2dSevenSegments && other) = default;
    Renderer2dSevenSegments & operator=(Renderer2dSevenSegments const & other) = default;
    Renderer2dSevenSegments & operator=(Renderer2dSevenSegments && other) = default;

    virtual ValidityAndColor evaluate(Coordinates2d::Position const & position) const
    {
        ValidityAndColor renderResult;
        for (Renderer2d const & renderer : segmentsEnabled_)
        {
            renderResult = renderer.evaluate(position);
            if (renderResult.valid)
            {
                break; // Don't look at further renderers.
            }
            else
            {
                // intentionally empty
            }
        }
        return renderResult;
    }



private:

    static constexpr size_t enumToIndex_(Segment const segment)
    {
        switch (segment)
        {
        case Segment::top       : return 0;
        case Segment::middle    : return 1;
        case Segment::bottom    : return 2;
        case Segment::leftUp    : return 3;
        case Segment::leftDown  : return 4;
        case Segment::rightUp   : return 5;
        case Segment::rightDown : return 6;
        }
    }

    static std::array<Renderer2dRelative const, 7> const segmentsShifted_;

    std::array<Renderer2dEnabled, 7> segmentsEnabled_;

};

std::array<Renderer2dRelative const, 7> const Renderer2dSevenSegments::segmentsShifted_{
    /*top      */  Renderer2dRelative{&sevenSegmentXElement, Coordinates2d::Position(widthSevenSegment / 2, 0)},
    /*middle   */  Renderer2dRelative{&sevenSegmentXElement, Coordinates2d::Position(widthSevenSegment / 2, ySpacingSevenSegment)},
    /*bottom   */  Renderer2dRelative{&sevenSegmentXElement, Coordinates2d::Position(widthSevenSegment / 2, 2 * ySpacingSevenSegment)},
    /*leftUp   */  Renderer2dRelative{&sevenSegmentYElement, Coordinates2d::Position(0, widthSevenSegment / 2)},
    /*leftDown */  Renderer2dRelative{&sevenSegmentYElement, Coordinates2d::Position(0, widthSevenSegment / 2 + ySpacingSevenSegment)},
    /*rightUp  */  Renderer2dRelative{&sevenSegmentYElement, Coordinates2d::Position(xSpacingSevenSegment, widthSevenSegment / 2)},
    /*rightDown*/  Renderer2dRelative{&sevenSegmentYElement, Coordinates2d::Position(xSpacingSevenSegment, widthSevenSegment / 2 + ySpacingSevenSegment)},
};


static Renderer2dSevenSegments sevenSegments0;
static Renderer2dSevenSegments sevenSegments1;
static Renderer2dSevenSegments sevenSegments2;
static Renderer2dSevenSegments sevenSegments3;





// The further at the front of the vecotr, the further at the front in the rendered image.
static std::vector<std::shared_ptr<Renderer2d>> renderers{
    // on/off
    std::make_shared<Renderer2dRelative>(&buttonSquare, Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown)),
    // settings
    std::make_shared<Renderer2dRelative>(&buttonSquare, Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + ySpacingButtons)),
    // std::make_shared<Renderer2dRelative>(&buttonTriangleRight, Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + ySpacingButtons)),
    // up
    std::make_shared<Renderer2dRelative>(&buttonTriangleUp, Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + 2 * ySpacingButtons)),
    // down
    std::make_shared<Renderer2dRelative>(&buttonTriangleDown, Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + 3 * ySpacingButtons)),

    // segment 0
    std::make_shared<Renderer2dRelative>(&sevenSegments0, Coordinates2d::Position(xOffsetSegment0, yOffsetSegment)),
    std::make_shared<Renderer2dRelative>(&sevenSegments1, Coordinates2d::Position(xOffsetSegment0 + xSpacingSevenSegments, yOffsetSegment)),

    std::make_shared<Renderer2dRelative>(&sevenSegments2, Coordinates2d::Position(xOffsetSegment0 + 2 * xSpacingSevenSegments + horizontalLengthInterSegment + lengthSegmentDots, yOffsetSegment)),
    std::make_shared<Renderer2dRelative>(&sevenSegments3, Coordinates2d::Position(xOffsetSegment0 + 3 * xSpacingSevenSegments + horizontalLengthInterSegment + lengthSegmentDots, yOffsetSegment)),

    // selection 0
    std::make_shared<Renderer2dRelative>(&elementSelection, Coordinates2d::Position(xOffsetSegment0 + xOffsetElementSelection, yOffsetSegment + yLengthSevenSegments + yOffsetElementSelection)),

    // std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(61, 32), Coordinates2d::Dimension(3, 5), /*color*/ 128),
    // std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(63, 34), Coordinates2d::Dimension(3, 5), /*color*/ 164),
    // std::make_shared<Renderer2dTriangle>(Coordinates2d::Position(14, 4),
    //                                      Coordinates2d::Position(19, 9),
    //                                      Coordinates2d::Position(19, 9),
    //                                      /*color*/ 164),
    // std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(8, 2), Coordinates2d::Position(19, 2), /*color*/ 0),
    // std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(6, 4), Coordinates2d::Position(6, 9), /*color*/ 0),

    // Background - not necessary but only as to highlight spareSpaceOutside.
    std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(spareSpaceOutside, spareSpaceOutside),
                                                     Coordinates2d::Dimension(displayWidth - 2 * spareSpaceOutside, displayHeight - 2 * spareSpaceOutside), /*color*/ 255),
};


} // namespace anonymous


Helper::Helper()
    : imageWidth_(displayWidth)
    , imageHeight_(displayHeight)
    , imageData_(imageWidth_ * imageHeight_)
    , image_(reinterpret_cast<uchar const *>(imageData_.data()), imageWidth_, imageHeight_, QImage::Format::Format_RGB32)
{
    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, QColor(0xa6, 0xce, 0x39));

    background = QBrush(QColor(45, 83, 129));
    circleBrush = QBrush(gradient);
    circlePen = QPen(Qt::black);
    circlePen.setWidth(1);
    textPen = QPen(Qt::green);
    textFont.setPixelSize(50);
}


void Helper::paint(QPainter *painter, QPaintEvent *event, QSize const & viewport)
{
    int const textSize = std::min(std::min(viewport.width(), viewport.height()), 50);
    textFont.setPixelSize(textSize);



    std::chrono::steady_clock::time_point const startFill = std::chrono::steady_clock::now();

    std::fill(imageData_.begin(), imageData_.end(), background.color().rgba());

    // std::chrono::steady_clock::time_point const startY = std::chrono::steady_clock::now();

    for (int y = 0; image_.height() > y; ++y)
    {
        // std::chrono::steady_clock::time_point const startX = std::chrono::steady_clock::now();

        for (int x = 0; image_.width() > x; ++x)
        {
            // std::chrono::steady_clock::time_point const startR = std::chrono::steady_clock::now();

            Coordinates2d::Position const position(x, y);

            QRgb & pixel = imageData_[y * imageWidth_ + x];

            for (std::shared_ptr<Renderer2d> const & renderer : renderers)
            {
                assert(static_cast<bool>(renderer));
                Renderer2d::ValidityAndColor const renderResult = renderer->evaluate(position);
                if (renderResult.valid)
                {
                    pixel = QColor(renderResult.color, renderResult.color, renderResult.color).rgba();
                    break; // Don't look at further renderers.
                }
                else
                {
                    // intentionally empty
                }
            }

            // std::chrono::steady_clock::time_point const endR = std::chrono::steady_clock::now();
            // std::cout << "r [" << x << ", " << y << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(endR - startR).count() << " us" << std::endl;
        }

        // std::chrono::steady_clock::time_point const endX = std::chrono::steady_clock::now();
        // std::cout << "x: " << std::chrono::duration_cast<std::chrono::milliseconds>(endX - startX).count() << " ms" << std::endl;
    }

    std::chrono::steady_clock::time_point const endY = std::chrono::steady_clock::now();
    // std::cout << "y: " << std::chrono::duration_cast<std::chrono::milliseconds>(endY - startY).count() << " ms" << std::endl;
    std::cout << "overall: " << std::chrono::duration_cast<std::chrono::milliseconds>(endY - startFill).count() << " ms" << std::endl;

    painter->drawImage(/*target*/ QRect(0, 0, viewport.width(), viewport.height()),
                       image_,
                       /*source*/ QRect(0, 0, image_.width(), image_.height()));


    painter->translate(viewport.width() / 2, viewport.height() / 2);
}
