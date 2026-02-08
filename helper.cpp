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
#include <time.h>
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

namespace GuiFixtures
{

static constexpr int displayWidth = 264;
static constexpr int displayHeight = 176;
static constexpr int spareSpaceOutside = 0;
static constexpr int lengthButtons = 12;
static constexpr int lengthButtonsInternal = 6;
static constexpr int horizontalOffsetButtonsClock = 10;
static constexpr int widthSevenSegment = 10;
static constexpr int lengthXSevenSegmentSquare = 20;
static constexpr int lengthYSevenSegmentSquare = 59;
static constexpr int lengthSevenSegmentTriangle = 5;
static constexpr int innerSpacingSevenSegmentElements = 3;
static constexpr int horizontalLengthInterSegment = 10;
static constexpr int lengthSegmentDots = 15;
static constexpr int yDistanceDots = 20;
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

// Dots

static Renderer2dAxesAlignedRectangle const clockDot{Coordinates2d::Position(0, 0),
                                                     Coordinates2d::Dimension(lengthSegmentDots, lengthSegmentDots),
                                                     /*outsideColor*/ Colors::black};


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
    typedef std::uint8_t SegmentConfiguration;

public:

    enum Segment
    {
        top,
        middle,
        bottom,
        leftUp,
        leftDown,
        rightUp,
        rightDown
    };

    enum Display
    {
        none,
        number0,
        number1,
        number2,
        number3,
        number4,
        number5,
        number6,
        number7,
        number8,
        number9,
        a,
        b,
        c,
        d,
        e,
        f,
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
        if ((xLengthSevenSegments > position.x) && (yLengthSevenSegments > position.y))
        {
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
        }
        else
        {
            // intentionally empty
        }
        return renderResult;
    }

    void set(Display const display)
    {
        SegmentConfiguration const configuration = configurations_[display];
        segmentsEnabled_[Segment::top].set(0 != ((1 << Segment::top) & configuration));
        segmentsEnabled_[Segment::middle].set(0 != ((1 << Segment::middle) & configuration));
        segmentsEnabled_[Segment::bottom].set(0 != ((1 << Segment::bottom) & configuration));
        segmentsEnabled_[Segment::leftUp].set(0 != ((1 << Segment::leftUp) & configuration));
        segmentsEnabled_[Segment::leftDown].set(0 != ((1 << Segment::leftDown) & configuration));
        segmentsEnabled_[Segment::rightUp].set(0 != ((1 << Segment::rightUp) & configuration));
        segmentsEnabled_[Segment::rightDown].set(0 != ((1 << Segment::rightDown) & configuration));
    }


    constexpr static Display singleDigitToDisplay(int const digit)
    {
        switch (digit)
        {
        case 0: return Display::number0;
        case 1: return Display::number1;
        case 2: return Display::number2;
        case 3: return Display::number3;
        case 4: return Display::number4;
        case 5: return Display::number5;
        case 6: return Display::number6;
        case 7: return Display::number7;
        case 8: return Display::number8;
        case 9: return Display::number9;
        case 0xa: return Display::a;
        case 0xb: return Display::b;
        case 0xc: return Display::c;
        case 0xd: return Display::d;
        case 0xe: return Display::e;
        case 0xf: return Display::f;
        default: return Display::none;
        }
    }


private:

    // (1 << rightDown) | (1 << rightUp) | (1 << leftDown) | (1 << leftUp) | (1 << bottom) | (1 << middle) | (1 << top)
    constexpr static std::array<SegmentConfiguration, 17> const configurations_{
        /*None*/ 0,
        /*number0*/ (1 << rightDown) | (1 << rightUp) | (1 << leftDown) | (1 << leftUp) | (1 << bottom) | (1 << top),
        /*number1*/ (1 << rightDown) | (1 << rightUp),
        /*number2*/ (1 << rightUp) | (1 << leftDown) | (1 << bottom) | (1 << middle) | (1 << top),
        /*number3*/ (1 << rightDown) | (1 << rightUp) | (1 << bottom) | (1 << middle) | (1 << top),
        /*number4*/ (1 << rightDown) | (1 << rightUp) | (1 << leftUp) | (1 << middle),
        /*number5*/ (1 << rightDown) | (1 << leftUp) | (1 << bottom) | (1 << middle) | (1 << top),
        /*number6*/ (1 << rightDown) | (1 << leftDown) | (1 << leftUp) | (1 << bottom) | (1 << middle) | (1 << top),
        /*number7*/ (1 << rightDown) | (1 << rightUp) | (1 << top),
        /*number8*/ (1 << rightDown) | (1 << rightUp) | (1 << leftDown) | (1 << leftUp) | (1 << bottom) | (1 << middle) | (1 << top),
        /*number9*/ (1 << rightDown) | (1 << rightUp) | (1 << leftUp) | (1 << bottom) | (1 << middle) | (1 << top),
        /*A*/ (1 << rightDown) | (1 << rightUp) | (1 << leftDown) | (1 << leftUp) | (1 << middle) | (1 << top),
        /*b*/ (1 << rightDown) | (1 << leftDown) | (1 << leftUp) | (1 << bottom) | (1 << middle),
        /*C*/ (1 << leftDown) | (1 << leftUp) | (1 << bottom) | (1 << top),
        /*d*/ (1 << rightDown) | (1 << rightUp) | (1 << leftDown) | (1 << bottom) | (1 << middle),
        /*E*/ (1 << leftDown) | (1 << leftUp) | (1 << bottom) | (1 << middle) | (1 << top),
        /*F*/ (1 << leftDown) | (1 << leftUp) | (1 << middle) | (1 << top),
    };
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


// on/off
static const Renderer2dRelative buttonOnOff(&buttonSquare, Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown));
// settings
static const Renderer2dRelative buttonSettings(&buttonSquare, Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + ySpacingButtons));
static const Renderer2dRelative buttonSettingsRight(&buttonTriangleRight, Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + ySpacingButtons));
// up
static const Renderer2dRelative buttonUp(&buttonTriangleUp, Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + 2 * ySpacingButtons));
// down
static const Renderer2dRelative buttonDown(&buttonTriangleDown, Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + 3 * ySpacingButtons));

// selections
static const Renderer2dRelative selection0(&elementSelection, Coordinates2d::Position(xOffsetSegment0 + xOffsetElementSelection, yOffsetSegment + yLengthSevenSegments + yOffsetElementSelection));
static const Renderer2dRelative selection1(&elementSelection, Coordinates2d::Position(xOffsetSegment0 + xOffsetElementSelection + xSpacingSevenSegments, yOffsetSegment + yLengthSevenSegments + yOffsetElementSelection));
static const Renderer2dRelative selection2(&elementSelection, Coordinates2d::Position(xOffsetSegment0 + xOffsetElementSelection + 2 * xSpacingSevenSegments + horizontalLengthInterSegment + lengthSegmentDots, yOffsetSegment + yLengthSevenSegments + yOffsetElementSelection));
static const Renderer2dRelative selection3(&elementSelection, Coordinates2d::Position(xOffsetSegment0 + xOffsetElementSelection + 3 * xSpacingSevenSegments + horizontalLengthInterSegment + lengthSegmentDots, yOffsetSegment + yLengthSevenSegments + yOffsetElementSelection));

// dots
static const Renderer2dRelative dotUpper(&clockDot, Coordinates2d::Position(xOffsetSegment0 + 2 * xSpacingSevenSegments,
                                                                            yOffsetSegment + yLengthSevenSegments / 2 - lengthSegmentDots - yDistanceDots / 2));
static const Renderer2dRelative dotLower(&clockDot, Coordinates2d::Position(xOffsetSegment0 + 2 * xSpacingSevenSegments,
                                                                            yOffsetSegment + yLengthSevenSegments / 2 + yDistanceDots / 2));

} // namespace GuiFixtures


class Renderer2dClockGui : public Renderer2d
{
public:

    Renderer2dClockGui()
        : buttonOnOff(&GuiFixtures::buttonOnOff)
        , buttonSettings(&GuiFixtures::buttonSettings)
        , buttonSettingsRight(&GuiFixtures::buttonSettingsRight, false)
        , buttonUp(&GuiFixtures::buttonUp)
        , buttonDown(&GuiFixtures::buttonDown)
        , selection0(&GuiFixtures::selection0)
        , selection1(&GuiFixtures::selection1)
        , selection2(&GuiFixtures::selection2)
        , selection3(&GuiFixtures::selection3)
        , dotUpper(&GuiFixtures::dotUpper)
        , dotLower(&GuiFixtures::dotLower)
        , sevenSegments0Relative_(&sevenSegments0, Coordinates2d::Position(GuiFixtures::xOffsetSegment0,
                                                                           GuiFixtures::yOffsetSegment))
        , sevenSegments1Relative_(&sevenSegments1, Coordinates2d::Position(GuiFixtures::xOffsetSegment0 + GuiFixtures::xSpacingSevenSegments,
                                                                           GuiFixtures::yOffsetSegment))
        , sevenSegments2Relative_(&sevenSegments2, Coordinates2d::Position(GuiFixtures::xOffsetSegment0 + 2 * GuiFixtures::xSpacingSevenSegments + GuiFixtures::horizontalLengthInterSegment + GuiFixtures::lengthSegmentDots,
                                                                           GuiFixtures::yOffsetSegment))
        , sevenSegments3Relative_(&sevenSegments3, Coordinates2d::Position(GuiFixtures::xOffsetSegment0 + 3 * GuiFixtures::xSpacingSevenSegments + GuiFixtures::horizontalLengthInterSegment + GuiFixtures::lengthSegmentDots,
                                                                           GuiFixtures::yOffsetSegment))
    {
        // intentionally empty
    }

    ValidityAndColor evaluate(Coordinates2d::Position const & position) const override
    {
        ValidityAndColor result;

        for (Renderer2d const * const renderer : std::array<Renderer2d const *, 15>{
                 &sevenSegments0Relative_,
                 &sevenSegments1Relative_,
                 &sevenSegments2Relative_,
                 &sevenSegments3Relative_,
                 &buttonOnOff,
                 &buttonSettings,
                 &buttonSettingsRight,
                 &buttonUp,
                 &buttonDown,
                 &selection0,
                 &selection1,
                 &selection2,
                 &selection3,
                 &dotUpper,
                 &dotLower,})
        {
            result = renderer->evaluate(position);
            if (result.valid)
            {
                break; // Don't look at further renderers.
            }
            else
            {
                // intentionally empty
            }
        }

        return result;
    }

    Renderer2dEnabled buttonOnOff;
    Renderer2dEnabled buttonSettings;
    Renderer2dEnabled buttonSettingsRight;
    Renderer2dEnabled buttonUp;
    Renderer2dEnabled buttonDown;
    Renderer2dEnabled selection0;
    Renderer2dEnabled selection1;
    Renderer2dEnabled selection2;
    Renderer2dEnabled selection3;
    Renderer2dEnabled dotUpper;
    Renderer2dEnabled dotLower;

    GuiFixtures::Renderer2dSevenSegments sevenSegments0;
    GuiFixtures::Renderer2dSevenSegments sevenSegments1;
    GuiFixtures::Renderer2dSevenSegments sevenSegments2;
    GuiFixtures::Renderer2dSevenSegments sevenSegments3;

private:

    Renderer2dRelative sevenSegments0Relative_;
    Renderer2dRelative sevenSegments1Relative_;
    Renderer2dRelative sevenSegments2Relative_;
    Renderer2dRelative sevenSegments3Relative_;
};

static Renderer2dClockGui clockGui;


} // namespace anonymous


Helper::Helper()
    : imageWidth_(GuiFixtures::displayWidth)
    , imageHeight_(GuiFixtures::displayHeight)
    , imageData_(imageWidth_ * imageHeight_)
    , image_(reinterpret_cast<uchar const *>(imageData_.data()), imageWidth_, imageHeight_, QImage::Format::Format_RGB32)
    , previousMinutesLow_(-1)
{
    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, QColor(0xa6, 0xce, 0x39));

    background = Qt::white; // QBrush(QColor(45, 83, 129));
    circleBrush = QBrush(gradient);
    circlePen = QPen(Qt::black);
    circlePen.setWidth(1);
    textPen = QPen(Qt::green);
    textFont.setPixelSize(50);
}


void Helper::paint(QPainter *painter, QSize const & viewport, std::chrono::system_clock::time_point const timestamp)
{
    int const textSize = std::min(std::min(viewport.width(), viewport.height()), 50);
    textFont.setPixelSize(textSize);


    std::chrono::system_clock::time_point const now = std::chrono::system_clock::now();
    std::time_t const nowTime = std::chrono::system_clock::to_time_t(now);

    bool updateRender = true;
    // MSVC
    // static __inline errno_t __CRTDECL localtime_s(
    //     _Out_ struct tm*    const _Tm,
    //     _In_  time_t const* const _Time
    //     )

    struct tm nowLocal{0};
    errno_t const success = localtime_s(&nowLocal, &nowTime);
    if (0 == success)
    {
        int const hours = nowLocal.tm_hour;
        int const minutes = nowLocal.tm_min;

        int const hoursLow = hours % 10;
        int const hoursHigh = hours / 10;

        int const minutesLow = minutes % 10;
        int const minutesHigh = minutes / 10;

        if (previousMinutesLow_ != minutesLow)
        {
            clockGui.sevenSegments0.set(GuiFixtures::Renderer2dSevenSegments::singleDigitToDisplay(hoursHigh));
            clockGui.sevenSegments1.set(GuiFixtures::Renderer2dSevenSegments::singleDigitToDisplay(hoursLow));
            clockGui.sevenSegments2.set(GuiFixtures::Renderer2dSevenSegments::singleDigitToDisplay(minutesHigh));
            clockGui.sevenSegments3.set(GuiFixtures::Renderer2dSevenSegments::singleDigitToDisplay(minutesLow));

            previousMinutesLow_ = minutesLow;
        }
        else
        {
            updateRender = false;
        }
    }
    else
    {
        clockGui.sevenSegments0.set(GuiFixtures::Renderer2dSevenSegments::Display::number8);
        clockGui.sevenSegments1.set(GuiFixtures::Renderer2dSevenSegments::Display::number8);
        clockGui.sevenSegments2.set(GuiFixtures::Renderer2dSevenSegments::Display::number8);
        clockGui.sevenSegments3.set(GuiFixtures::Renderer2dSevenSegments::Display::number8);
    }

    if (updateRender)
    {
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

                Renderer2d::ValidityAndColor const renderResult = clockGui.evaluate(position);
                if (renderResult.valid)
                {
                    imageData_[y * imageWidth_ + x] = QColor(renderResult.color, renderResult.color, renderResult.color).rgba();
                }
                else
                {
                    // intentionally empty
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

        image_ = QImage(reinterpret_cast<uchar const *>(imageData_.data()), imageWidth_, imageHeight_, QImage::Format::Format_RGB32);
    }
    else
    {
        // intentionally empty
    }

    painter->drawImage(/*target*/ QRect(0, 0, viewport.width(), viewport.height()),
                       image_,
                       /*source*/ QRect(0, 0, image_.width(), image_.height()));


    painter->translate(viewport.width() / 2, viewport.height() / 2);
}
