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


    constexpr int spareSpaceOutside = 4;

    // The further at the front of the vecotr, the further at the front in the rendered image.
    std::vector<std::shared_ptr<Renderer2d>> renderers{
        std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(0, 0), Coordinates2d::Dimension(3, 5), /*color*/ 2),
        std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(61, 32), Coordinates2d::Dimension(3, 5), /*color*/ 128),
        std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(63, 34), Coordinates2d::Dimension(3, 5), /*color*/ 164),
        std::make_shared<Renderer2dTriangle>(Coordinates2d::Position(14, 4),
                                             Coordinates2d::Position(19, 9),
                                             Coordinates2d::Position(19, 9),
                                             /*color*/ 164),
        std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(8, 2), Coordinates2d::Position(19, 2), /*color*/ 0),
        std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(6, 4), Coordinates2d::Position(6, 9), /*color*/ 0),

        std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(spareSpaceOutside, spareSpaceOutside),
                                                         Coordinates2d::Dimension(image_.width() - 2 * spareSpaceOutside, image_.height() - 2 * spareSpaceOutside), /*color*/ 255),
    };

    // std::chrono::steady_clock::time_point const startFill = std::chrono::steady_clock::now();

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

    // std::chrono::steady_clock::time_point const endY = std::chrono::steady_clock::now();
    // std::cout << "y: " << std::chrono::duration_cast<std::chrono::milliseconds>(endY - startY).count() << " ms" << std::endl;
    // std::cout << "overall: " << std::chrono::duration_cast<std::chrono::milliseconds>(endY - startFill).count() << " ms" << std::endl;

    painter->drawImage(/*target*/ QRect(0, 0, viewport.width(), viewport.height()),
                       image_,
                       /*source*/ QRect(0, 0, image_.width(), image_.height()));


    painter->translate(viewport.width() / 2, viewport.height() / 2);
}
