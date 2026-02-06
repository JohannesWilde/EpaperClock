// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "helper.h"
#include "renderer2d.hpp"

#include <QPainter>
#include <QPaintEvent>
#include <QWidget>

#include <algorithm>
#include <memory>
#include <vector>

// #include <chrono>
// #include <iostream>


Helper::Helper()
    : imageWidth_(264)
    , imageHeight_(176)
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

    constexpr int displayWidth = 264;
    constexpr int displayHeight = 176;
    constexpr int spareSpaceOutside = 4;
    constexpr int lengthButtons = 12;
    constexpr int lengthButtonsInternal = 6;
    constexpr int horizontalOffsetButtonsClock = 15;
    constexpr int xOffsetElementSelection = 0;
    constexpr int yOffsetElementSelection = 2;
    constexpr int lengthElementSelection = 10;
    constexpr int widthSevenSegment = 10;
    constexpr int lengthXSevenSegmentSquare = 30;
    constexpr int lengthYSevenSegmentSquare = 40;
    constexpr int lengthSevenSegmentTriangle = 5;
    constexpr int innerSpacingSevenSegmentElements = 3;
    constexpr int horizontalLengthInterSegment = 10;
    constexpr int lengthSegmentDots = 20;
    // constexpr int  = ;

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

    constexpr int xLengthSevenSegments = lengthXSevenSegmentSquare + 2 * lengthSevenSegmentTriangle + widthSevenSegment + 2 * innerSpacingSevenSegmentElements;
    constexpr int yLengthSevenSegments = 2 * lengthYSevenSegmentSquare + 4 * lengthSevenSegmentTriangle + widthSevenSegment + 4 * innerSpacingSevenSegmentElements;
    constexpr int xSpacingSevenSegments = xLengthSevenSegments + horizontalLengthInterSegment;
    constexpr int xOffsetSegment0 = xOffsetButtonDown + lengthButtons + horizontalOffsetButtonsClock;
    constexpr int yOffsetSegment = (visibleY - yLengthSevenSegments - 2 * (lengthElementSelection + yOffsetElementSelection)) / 2;
    constexpr int relativeOffsetSegmentBase = widthSevenSegment / 2 + innerSpacingSevenSegmentElements;
    // constexpr int  = ;
    // constexpr int  = ;
    // constexpr int  = ;
    // constexpr int  = ;
    // constexpr int  = ;
    // constexpr int  = ;
    // constexpr int  = ;
    // constexpr int  = ;


    // The further at the front of the vecotr, the further at the front in the rendered image.
    std::vector<std::shared_ptr<Renderer2d>> renderers{
        // on/off
        std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(xOffsetButtonDownInternal, yOffsetButtonDownInternal),
                                                         Coordinates2d::Dimension(lengthButtonsInternal, lengthButtonsInternal),
                                                         /*color*/ 255),
        std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown),
                                                         Coordinates2d::Dimension(lengthButtons, lengthButtons),
                                                         /*color*/ 0),
        // settings
        std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(xOffsetButtonDownInternal, yOffsetButtonDownInternal + ySpacingButtons),
                                                         Coordinates2d::Dimension(lengthButtonsInternal, lengthButtonsInternal),
                                                         /*color*/ 255),
        std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + ySpacingButtons),
                                                         Coordinates2d::Dimension(lengthButtons, lengthButtons),
                                                         /*color*/ 0),
        // std::make_shared<Renderer2dTriangle>(Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + ySpacingButtons),
        //                                      Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + ySpacingButtons + lengthButtons),
        //                                      Coordinates2d::Position(xOffsetButtonDown + lengthButtons, yOffsetButtonDown + ySpacingButtons + lengthButtons / 2),
        //                                      /*color*/ 0),
        // up
        std::make_shared<Renderer2dTriangle>(Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + 2 * ySpacingButtons + lengthButtons),
                                             Coordinates2d::Position(xOffsetButtonDown + lengthButtons / 2, yOffsetButtonDown + 2 * ySpacingButtons),
                                             Coordinates2d::Position(xOffsetButtonDown + lengthButtons, yOffsetButtonDown + 2 * ySpacingButtons + lengthButtons),
                                             /*color*/ 0),
        // down
        std::make_shared<Renderer2dTriangle>(Coordinates2d::Position(xOffsetButtonDown, yOffsetButtonDown + 3 * ySpacingButtons),
                                             Coordinates2d::Position(xOffsetButtonDown + lengthButtons / 2, yOffsetButtonDown + 3 * ySpacingButtons + lengthButtons),
                                             Coordinates2d::Position(xOffsetButtonDown + lengthButtons, yOffsetButtonDown + 3 * ySpacingButtons),
                                             /*color*/ 0),

        // segment
        // todo: relative position class, combining class
        std::make_shared<Renderer2dTriangle>(Coordinates2d::Position(xOffsetSegment0,                           yOffsetSegment + relativeOffsetSegmentBase + lengthSevenSegmentTriangle),
                                             Coordinates2d::Position(xOffsetSegment0 + widthSevenSegment / 2,   yOffsetSegment + relativeOffsetSegmentBase),
                                             Coordinates2d::Position(xOffsetSegment0 + widthSevenSegment,       yOffsetSegment + relativeOffsetSegmentBase + lengthSevenSegmentTriangle),
                                             /*color*/ 0),
        std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(xOffsetSegment0, yOffsetSegment + relativeOffsetSegmentBase + lengthSevenSegmentTriangle),
                                                         Coordinates2d::Dimension(widthSevenSegment, lengthYSevenSegmentSquare),
                                                         /*color*/ 0),
        std::make_shared<Renderer2dTriangle>(Coordinates2d::Position(xOffsetSegment0,                           yOffsetSegment + relativeOffsetSegmentBase + lengthSevenSegmentTriangle + lengthYSevenSegmentSquare),
                                             Coordinates2d::Position(xOffsetSegment0 + widthSevenSegment / 2,   yOffsetSegment + relativeOffsetSegmentBase + 2 * lengthSevenSegmentTriangle + lengthYSevenSegmentSquare),
                                             Coordinates2d::Position(xOffsetSegment0 + widthSevenSegment,       yOffsetSegment + relativeOffsetSegmentBase + lengthSevenSegmentTriangle + lengthYSevenSegmentSquare),
                                             /*color*/ 0),

        std::make_shared<Renderer2dTriangle>(Coordinates2d::Position(xOffsetSegment0 + relativeOffsetSegmentBase + lengthSevenSegmentTriangle,  yOffsetSegment),
                                             Coordinates2d::Position(xOffsetSegment0 + relativeOffsetSegmentBase,                               yOffsetSegment + widthSevenSegment / 2),
                                             Coordinates2d::Position(xOffsetSegment0 + relativeOffsetSegmentBase + lengthSevenSegmentTriangle,  yOffsetSegment + widthSevenSegment),
                                             /*color*/ 0),
        std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(xOffsetSegment0 + relativeOffsetSegmentBase + lengthSevenSegmentTriangle, yOffsetSegment),
                                                         Coordinates2d::Dimension(lengthXSevenSegmentSquare, widthSevenSegment),
                                                         /*color*/ 0),
        std::make_shared<Renderer2dTriangle>(Coordinates2d::Position(xOffsetSegment0 + relativeOffsetSegmentBase + lengthSevenSegmentTriangle + lengthXSevenSegmentSquare,       yOffsetSegment),
                                             Coordinates2d::Position(xOffsetSegment0 + relativeOffsetSegmentBase + 2 * lengthSevenSegmentTriangle + lengthXSevenSegmentSquare,   yOffsetSegment + widthSevenSegment / 2),
                                             Coordinates2d::Position(xOffsetSegment0 + relativeOffsetSegmentBase + lengthSevenSegmentTriangle + lengthXSevenSegmentSquare,       yOffsetSegment + widthSevenSegment),
                                             /*color*/ 0),



        // std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(61, 32), Coordinates2d::Dimension(3, 5), /*color*/ 128),
        // std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(63, 34), Coordinates2d::Dimension(3, 5), /*color*/ 164),
        // std::make_shared<Renderer2dTriangle>(Coordinates2d::Position(14, 4),
        //                                      Coordinates2d::Position(19, 9),
        //                                      Coordinates2d::Position(19, 9),
        //                                      /*color*/ 164),
        // std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(8, 2), Coordinates2d::Position(19, 2), /*color*/ 0),
        // std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(6, 4), Coordinates2d::Position(6, 9), /*color*/ 0),

        std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(spareSpaceOutside, spareSpaceOutside),
                                                         Coordinates2d::Dimension(image_.width() - 2 * spareSpaceOutside, image_.height() - 2 * spareSpaceOutside), /*color*/ 255),
    };

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
