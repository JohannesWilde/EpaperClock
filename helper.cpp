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


Helper::Helper()
    : image_(264, 176, QImage::Format::Format_RGB32)
{
    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, QColor(0xa6, 0xce, 0x39));

    background = QBrush(Qt::white);
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

    // The further at the front of the vecotr, the further at the front in the rendered image.
    std::vector<std::shared_ptr<Renderer2d>> renderers{
        std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(0, 0), Coordinates2d::Dimension(3, 5), /*color*/ 2),
        std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(61, 32), Coordinates2d::Dimension(3, 5), /*color*/ 128),
        std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(63, 34), Coordinates2d::Dimension(3, 5), /*color*/ 164),
        std::make_shared<Renderer2dTriangle>(Coordinates2d::Position(14, 4),
                                             Coordinates2d::Position(19, 9),
                                             Coordinates2d::Position(8, 6),
                                             /*color*/ 164),
        std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(8, 2), Coordinates2d::Position(19, 2), /*color*/ 0),
        std::make_shared<Renderer2dAxesAlignedRectangle>(Coordinates2d::Position(6, 4), Coordinates2d::Position(6, 9), /*color*/ 0),
    };


    image_.fill(background.color());

    for (int y = 0; image_.height() > y; ++y)
    {
        for (int x = 0; image_.width() > x; ++x)
        {
            for (std::shared_ptr<Renderer2d> const & renderer : renderers)
            {
                assert(static_cast<bool>(renderer));
                Renderer2d::ValidityAndColor const renderResult = renderer->evaluate(Coordinates2d::Position(x, y));
                if (renderResult.valid)
                {
                    image_.setPixelColor(x, y, QColor(renderResult.color, renderResult.color, renderResult.color));
                    break; // Don't look at further renderers.
                }
                else
                {
                    // intentionally empty
                }
            }
        }
    }

    painter->drawImage(/*target*/ QRect(0, 0, viewport.width(), viewport.height()),
                       image_,
                       /*source*/ QRect(0, 0, image_.width(), image_.height()));


    painter->translate(viewport.width() / 2, viewport.height() / 2);



}
