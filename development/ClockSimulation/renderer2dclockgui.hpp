#ifndef RENDERER_2D_CLOCK_GUI_HPP
#define RENDERER_2D_CLOCK_GUI_HPP

#include "renderer2d.hpp"
#include "renderer2dextended.hpp"


namespace GuiFixtures
{

class Colors
{
private:
    Colors() = delete;
public:
    static constexpr Renderer2d::Color black = 0;
    static constexpr Renderer2d::Color white = 255;
};

constexpr int displayWidth = 264;
constexpr int displayHeight = 176;
constexpr int spareSpaceOutside = 0;
constexpr int lengthButtons = 13;
constexpr int lengthButtonsInternal = 6;
constexpr int horizontalOffsetButtonsClock = 10;
constexpr int widthSevenSegment = 11;
constexpr int lengthXSevenSegmentSquare = 18;
constexpr int lengthYSevenSegmentSquare = 57;
constexpr int lengthSevenSegmentTriangle = 6;
constexpr int innerSpacingSevenSegmentElements = 3;
constexpr int horizontalLengthInterSegment = 10;
constexpr int lengthSegmentDots = 13;
constexpr int yDistanceDots = 22;
constexpr int lengthElementSelection = 11;
constexpr int xOffsetElementSelection = (widthSevenSegment + lengthXSevenSegmentSquare - lengthElementSelection) / 2 + innerSpacingSevenSegmentElements + lengthSevenSegmentTriangle;
constexpr int yOffsetElementSelection = 4;

// (0, 0) is at top left.
// x [first index - width] goes to the right. y [second index - height] goes down.

constexpr int visibleX = (displayWidth - 2 * spareSpaceOutside);
constexpr int visibleY = (displayHeight - 2 * spareSpaceOutside);

constexpr int numberOfButtons = 4;
constexpr int ySpacingButtons = (displayHeight - 2 * spareSpaceOutside) / numberOfButtons;
constexpr int xOffsetButtonDown = spareSpaceOutside;
constexpr int yOffsetButtonDown = spareSpaceOutside + ySpacingButtons / 2 - lengthButtons / 2;
constexpr int offsetButtonDownInternal = (lengthButtons - lengthButtonsInternal) / 2;
constexpr int xOffsetButtonDownInternal = xOffsetButtonDown + offsetButtonDownInternal;
constexpr int yOffsetButtonDownInternal = yOffsetButtonDown + offsetButtonDownInternal;

constexpr int xSpacingSevenSegment = lengthXSevenSegmentSquare + 2 * lengthSevenSegmentTriangle + 2 * innerSpacingSevenSegmentElements;
constexpr int ySpacingSevenSegment = lengthYSevenSegmentSquare + 2 * lengthSevenSegmentTriangle + 2 * innerSpacingSevenSegmentElements;
constexpr int xLengthSevenSegments = xSpacingSevenSegment + widthSevenSegment;
constexpr int yLengthSevenSegments = 2 * ySpacingSevenSegment + widthSevenSegment;
constexpr int xSpacingSevenSegments = xLengthSevenSegments + horizontalLengthInterSegment;
constexpr int xOffsetSegment0 = xOffsetButtonDown + lengthButtons + horizontalOffsetButtonsClock;
constexpr int yOffsetSegment = (visibleY - yLengthSevenSegments - (yOffsetElementSelection + lengthElementSelection)) / 2;

// Buttons

extern Renderer2dButton const buttonSquare;
extern Renderer2dAxesAlignedRectangle const buttonSquareFull;
extern Renderer2dTriangle const buttonTriangleUp;
extern Renderer2dTriangle const buttonTriangleDown;
extern Renderer2dTriangle const buttonTriangleLeft;
extern Renderer2dTriangle const buttonTriangleRight;

// Element selection [triangle up]
extern Renderer2dTriangle const elementSelection;

// Dots
extern Renderer2dAxesAlignedRectangle const clockDot;


// Seven segment display

extern Renderer2dTriangle const sevenSegmentTriangleUp;
extern Renderer2dTriangle const sevenSegmentTriangleDown;
extern Renderer2dTriangle const sevenSegmentTriangleLeft;
extern Renderer2dTriangle const sevenSegmentTriangleRight;

extern Renderer2dAxesAlignedRectangle const sevenSegmentXSquare;
extern Renderer2dAxesAlignedRectangle const sevenSegmentYSquare;

extern Renderer2dRelative const sevenSegmentTriangleLeftShifted;
extern Renderer2dRelative const sevenSegmentXSquareShifted;
extern Renderer2dRelative const sevenSegmentTriangleRightShifted;

extern Renderer2dAccumulated<3> const sevenSegmentXElement;

extern Renderer2dRelative const sevenSegmentTriangleUpShifted;
extern Renderer2dRelative const sevenSegmentYSquareShifted;
extern Renderer2dRelative const sevenSegmentTriangleDownShifted;

extern Renderer2dAccumulated<3> const sevenSegmentYElement;

extern Renderer2dRelative const segmentShifted0_;
extern Renderer2dRelative const segmentShifted1_;
extern Renderer2dRelative const segmentShifted2_;
extern Renderer2dRelative const segmentShifted3_;

extern Renderer2dRelative const segmentShifted4_;
extern Renderer2dRelative const segmentShifted5_;
extern Renderer2dRelative const segmentShifted6_;


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

    void render(Coordinates2d::Position const & offset,
                Coordinates2d::Dimension const & dimension,
                Color * const data) const override
    {
        // if ((xLengthSevenSegments > position.x) && (yLengthSevenSegments > position.y))
        // {
            for (Renderer2d const & renderer : segmentsEnabled_)
            {
                renderer.render(offset, dimension, data);
            }
        // }
        // else
        // {
        //     // intentionally empty
        // }
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


// on/off
static const Renderer2dRelative buttonSettings(&buttonSquare, Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + 0 * ySpacingButtons));
// settings
static const Renderer2dRelative buttonSettingsRight(&buttonTriangleRight, Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + 1 * ySpacingButtons));
// up
static const Renderer2dRelative buttonUp(&buttonTriangleUp, Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + 2 * ySpacingButtons));
// down
static const Renderer2dRelative buttonDown(&buttonTriangleDown, Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + 3 * ySpacingButtons));
// on/off
static const Renderer2dRelative buttonOnOff(&buttonSquareFull, Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + 3 * ySpacingButtons));

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
        , buttonSettingsRight(&GuiFixtures::buttonSettingsRight)
        , buttonUp(&GuiFixtures::buttonUp)
        , buttonDown(&GuiFixtures::buttonDown, false)
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


    void render(Coordinates2d::Position const & offset,
                Coordinates2d::Dimension const & dimension,
                Color * const data) const override
    {
        sevenSegments0Relative_.render(offset, dimension, data);
        sevenSegments1Relative_.render(offset, dimension, data);
        sevenSegments2Relative_.render(offset, dimension, data);
        sevenSegments3Relative_.render(offset, dimension, data);
        buttonOnOff.render(offset, dimension, data);
        buttonSettings.render(offset, dimension, data);
        buttonSettingsRight.render(offset, dimension, data);
        buttonUp.render(offset, dimension, data);
        buttonDown.render(offset, dimension, data);
        selection0.render(offset, dimension, data);
        selection1.render(offset, dimension, data);
        selection2.render(offset, dimension, data);
        selection3.render(offset, dimension, data);
        dotUpper.render(offset, dimension, data);
        dotLower.render(offset, dimension, data);
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


enum class ClockGuiButton
{
    None,
    Settings,
    Right,
    Up,
    OnOffOrDown
};

char const * toString(ClockGuiButton const button);

ClockGuiButton positionToButton(Coordinates2d::Position const pos);

#endif // RENDERER_2D_CLOCK_GUI_HPP
