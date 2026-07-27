#include "renderer2dclockgui.hpp"

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
