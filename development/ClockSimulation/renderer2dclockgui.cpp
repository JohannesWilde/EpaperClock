#include "renderer2dclockgui.hpp"


namespace GuiFixtures
{

Renderer2dButton const buttonSquare{Coordinates2d::Dimension(lengthButtons, lengthButtons),
                                    /*outsideColor*/ Colors::black,
                                    (lengthButtons - lengthButtonsInternal) / 2,
                                    /*insideColor*/ Colors::white};

Renderer2dAxesAlignedRectangle const buttonSquareFull{Coordinates2d::Dimension(lengthButtons, lengthButtons),
                                                      /*outsideColor*/ Colors::black};

Renderer2dTriangle const buttonTriangleUp{Coordinates2d::Position(0,                         lengthButtons - 1),
                                          Coordinates2d::Position((lengthButtons - 1) / 2,   0),
                                          Coordinates2d::Position(lengthButtons - 1,         lengthButtons - 1),
                                          /*color*/ Colors::black};

Renderer2dTriangle const buttonTriangleDown{Coordinates2d::Position(0,                       0),
                                            Coordinates2d::Position((lengthButtons - 1) / 2, lengthButtons - 1),
                                            Coordinates2d::Position(lengthButtons - 1,       0),
                                            /*color*/ Colors::black};

Renderer2dTriangle const buttonTriangleLeft{Coordinates2d::Position(lengthButtons - 1,   0),
                                            Coordinates2d::Position(0,                   (lengthButtons - 1) / 2),
                                            Coordinates2d::Position(0,                   lengthButtons - 1),
                                            /*color*/ Colors::black};

Renderer2dTriangle const buttonTriangleRight{Coordinates2d::Position(0,                   0),
                                             Coordinates2d::Position(0,                   lengthButtons - 1),
                                             Coordinates2d::Position(lengthButtons - 1,   (lengthButtons - 1) / 2),
                                             /*color*/ Colors::black};

// Element selection [triangle up]
Renderer2dTriangle const elementSelection{Coordinates2d::Position(0,                                 (lengthElementSelection - 1)),
                                          Coordinates2d::Position((lengthElementSelection - 1)/ 2,   0),
                                          Coordinates2d::Position((lengthElementSelection - 1),      (lengthElementSelection - 1)),
                                          /*color*/ Colors::black};

// Dots

Renderer2dAxesAlignedRectangle const clockDot{Coordinates2d::Dimension(lengthSegmentDots, lengthSegmentDots),
                                              /*outsideColor*/ Colors::black};


// Seven segment display

Renderer2dTriangle const sevenSegmentTriangleUp{Coordinates2d::Position(0,                               lengthSevenSegmentTriangle - 1),
                                                Coordinates2d::Position((widthSevenSegment - 1) / 2,     0),
                                                Coordinates2d::Position(widthSevenSegment - 1,           lengthSevenSegmentTriangle - 1),
                                                /*color*/ Colors::black};

Renderer2dTriangle const sevenSegmentTriangleDown{Coordinates2d::Position(0,                             0),
                                                  Coordinates2d::Position((widthSevenSegment - 1) / 2,   lengthSevenSegmentTriangle - 1),
                                                  Coordinates2d::Position((widthSevenSegment - 1),       0),
                                                  /*color*/ Colors::black};

Renderer2dTriangle const sevenSegmentTriangleLeft{Coordinates2d::Position((lengthSevenSegmentTriangle - 1),  0),
                                                  Coordinates2d::Position(0,                                 (widthSevenSegment - 1) / 2),
                                                  Coordinates2d::Position((lengthSevenSegmentTriangle - 1),  (widthSevenSegment - 1)),
                                                  /*color*/ Colors::black};

Renderer2dTriangle const sevenSegmentTriangleRight{Coordinates2d::Position(0,                                0),
                                                   Coordinates2d::Position(0,                                (widthSevenSegment - 1)),
                                                   Coordinates2d::Position(lengthSevenSegmentTriangle - 1,   (widthSevenSegment - 1) / 2),
                                                   /*color*/ Colors::black};

Renderer2dAxesAlignedRectangle const sevenSegmentXSquare{Coordinates2d::Dimension(lengthXSevenSegmentSquare, widthSevenSegment),
                                                         /*color*/ 0};

Renderer2dAxesAlignedRectangle const sevenSegmentYSquare{Coordinates2d::Dimension(widthSevenSegment, lengthYSevenSegmentSquare),
                                                         /*color*/ 0};


Renderer2dRelative const sevenSegmentTriangleLeftShifted{&sevenSegmentTriangleLeft, Coordinates2d::Position(innerSpacingSevenSegmentElements, 0)};
Renderer2dRelative const sevenSegmentXSquareShifted{&sevenSegmentXSquare, Coordinates2d::Position(innerSpacingSevenSegmentElements + lengthSevenSegmentTriangle, 0)};
Renderer2dRelative const sevenSegmentTriangleRightShifted{&sevenSegmentTriangleRight, Coordinates2d::Position(innerSpacingSevenSegmentElements + lengthSevenSegmentTriangle + lengthXSevenSegmentSquare, 0)};

Renderer2dAccumulated<3> const sevenSegmentXElement{&sevenSegmentTriangleLeftShifted, &sevenSegmentXSquareShifted, &sevenSegmentTriangleRightShifted};


Renderer2dRelative const sevenSegmentTriangleUpShifted{&sevenSegmentTriangleUp, Coordinates2d::Position(0, innerSpacingSevenSegmentElements)};
Renderer2dRelative const sevenSegmentYSquareShifted{&sevenSegmentYSquare, Coordinates2d::Position(0, innerSpacingSevenSegmentElements + lengthSevenSegmentTriangle)};
Renderer2dRelative const sevenSegmentTriangleDownShifted{&sevenSegmentTriangleDown, Coordinates2d::Position(0, innerSpacingSevenSegmentElements + lengthSevenSegmentTriangle + lengthYSevenSegmentSquare)};

Renderer2dAccumulated<3> const sevenSegmentYElement{&sevenSegmentTriangleUpShifted, &sevenSegmentYSquareShifted, &sevenSegmentTriangleDownShifted};


Renderer2dRelative const segmentShifted0_(&sevenSegmentYElement, Coordinates2d::Position(0, yOffsetSegment + widthSevenSegment / 2));
Renderer2dRelative const segmentShifted1_(&sevenSegmentYElement, Coordinates2d::Position(0, yOffsetSegment + widthSevenSegment / 2 + ySpacingSevenSegment));
Renderer2dRelative const segmentShifted2_(&sevenSegmentYElement, Coordinates2d::Position(xSpacingSevenSegment, yOffsetSegment + widthSevenSegment / 2));
Renderer2dRelative const segmentShifted3_(&sevenSegmentYElement, Coordinates2d::Position(xSpacingSevenSegment, yOffsetSegment + widthSevenSegment / 2 + ySpacingSevenSegment));

Renderer2dRelative const segmentShifted4_(&sevenSegmentXElement, Coordinates2d::Position(widthSevenSegment / 2, 0));
Renderer2dRelative const segmentShifted5_(&sevenSegmentXElement, Coordinates2d::Position(widthSevenSegment / 2, ySpacingSevenSegment));
Renderer2dRelative const segmentShifted6_(&sevenSegmentXElement, Coordinates2d::Position(widthSevenSegment / 2, 2 * ySpacingSevenSegment));


// on/off
Renderer2dRelative const buttonSettings(&buttonSquare, Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + 0 * ySpacingButtons));
// settings
Renderer2dRelative const buttonSettingsRight(&buttonTriangleRight, Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + 1 * ySpacingButtons));
// up
Renderer2dRelative const buttonUp(&buttonTriangleUp, Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + 2 * ySpacingButtons));
// down
Renderer2dRelative const buttonDown(&buttonTriangleDown, Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + 3 * ySpacingButtons));
// on/off
Renderer2dRelative const buttonOnOff(&buttonSquareFull, Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + 3 * ySpacingButtons));

// selections
Renderer2dRelative const selection0(&elementSelection, Coordinates2d::Position(xOffsetSegment0 + xOffsetElementSelection, yOffsetSegment + yLengthSevenSegments + yOffsetElementSelection));
Renderer2dRelative const selection1(&elementSelection, Coordinates2d::Position(xOffsetSegment0 + xOffsetElementSelection + xSpacingSevenSegments, yOffsetSegment + yLengthSevenSegments + yOffsetElementSelection));
Renderer2dRelative const selection2(&elementSelection, Coordinates2d::Position(xOffsetSegment0 + xOffsetElementSelection + 2 * xSpacingSevenSegments + horizontalLengthInterSegment + lengthSegmentDots, yOffsetSegment + yLengthSevenSegments + yOffsetElementSelection));
Renderer2dRelative const selection3(&elementSelection, Coordinates2d::Position(xOffsetSegment0 + xOffsetElementSelection + 3 * xSpacingSevenSegments + horizontalLengthInterSegment + lengthSegmentDots, yOffsetSegment + yLengthSevenSegments + yOffsetElementSelection));

// dots
Renderer2dRelative const dotUpper(&clockDot, Coordinates2d::Position(xOffsetSegment0 + 2 * xSpacingSevenSegments,
                                                                            yOffsetSegment + yLengthSevenSegments / 2 - lengthSegmentDots - yDistanceDots / 2));
Renderer2dRelative const dotLower(&clockDot, Coordinates2d::Position(xOffsetSegment0 + 2 * xSpacingSevenSegments,
                                                                            yOffsetSegment + yLengthSevenSegments / 2 + yDistanceDots / 2));

} // namespace GuiFixtures


using namespace GuiFixtures;

std::array<Renderer2dRelative const, 7> const Renderer2dSevenSegments::segmentsShifted_{
    /*top      */  Renderer2dRelative{&sevenSegmentXElement, Coordinates2d::Position(widthSevenSegment / 2, 0)},
    /*middle   */  Renderer2dRelative{&sevenSegmentXElement, Coordinates2d::Position(widthSevenSegment / 2, ySpacingSevenSegment)},
    /*bottom   */  Renderer2dRelative{&sevenSegmentXElement, Coordinates2d::Position(widthSevenSegment / 2, 2 * ySpacingSevenSegment)},
    /*leftUp   */  Renderer2dRelative{&sevenSegmentYElement, Coordinates2d::Position(0, widthSevenSegment / 2)},
    /*leftDown */  Renderer2dRelative{&sevenSegmentYElement, Coordinates2d::Position(0, widthSevenSegment / 2 + ySpacingSevenSegment)},
    /*rightUp  */  Renderer2dRelative{&sevenSegmentYElement, Coordinates2d::Position(xSpacingSevenSegment, widthSevenSegment / 2)},
    /*rightDown*/  Renderer2dRelative{&sevenSegmentYElement, Coordinates2d::Position(xSpacingSevenSegment, widthSevenSegment / 2 + ySpacingSevenSegment)},
    };

const char * toString(const ClockGuiButton button)
{
    switch (button)
    {
    case ClockGuiButton::None:          return "None";
    case ClockGuiButton::Settings:      return "Settings";
    case ClockGuiButton::Right:         return "Right";
    case ClockGuiButton::Up:            return "Up";
    case ClockGuiButton::OnOffOrDown:   return "OnOffOrDown";
    }
    return "Unknown";
}

ClockGuiButton positionToButton(const Coordinates2d::Position pos)
{
    ClockGuiButton button = ClockGuiButton::None;

    if ((GuiFixtures::xOffsetButtonDown < pos.x) &&
            ((GuiFixtures::xOffsetButtonDown + GuiFixtures::lengthButtons) >= pos.x))
    {
        if (((GuiFixtures::yOffsetButtonDown + 0 * GuiFixtures::ySpacingButtons) < pos.y) &&
                ((GuiFixtures::yOffsetButtonDown + 0 * GuiFixtures::ySpacingButtons + GuiFixtures::lengthButtons) >= pos.y))
        {
            button = ClockGuiButton::Settings;
        }
        else if (((GuiFixtures::yOffsetButtonDown + 1 * GuiFixtures::ySpacingButtons) < pos.y) &&
                 ((GuiFixtures::yOffsetButtonDown + 1 * GuiFixtures::ySpacingButtons + GuiFixtures::lengthButtons) >= pos.y))
        {
            button = ClockGuiButton::Right;
        }
        else if (((GuiFixtures::yOffsetButtonDown + 2 * GuiFixtures::ySpacingButtons) < pos.y) &&
                 ((GuiFixtures::yOffsetButtonDown + 2 * GuiFixtures::ySpacingButtons + GuiFixtures::lengthButtons) >= pos.y))
        {
            button = ClockGuiButton::Up;
        }
        else if (((GuiFixtures::yOffsetButtonDown + 3 * GuiFixtures::ySpacingButtons) < pos.y) &&
                 ((GuiFixtures::yOffsetButtonDown + 3 * GuiFixtures::ySpacingButtons + GuiFixtures::lengthButtons) >= pos.y))
        {
            button = ClockGuiButton::OnOffOrDown;
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

    return button;
}
