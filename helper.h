// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef HELPER_H
#define HELPER_H

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>

#include <chrono>
#include <memory>
#include <vector>

class Renderer2dClockGui;



//! [0]
class Helper
{
public:
    Helper();

public:
    void paint(QPainter *painter, QSize const & viewport, std::chrono::system_clock::time_point const timestamp);

private:
    QBrush background;
    QBrush circleBrush;
    QFont textFont;
    QPen circlePen;
    QPen textPen;

    int const imageWidth_;
    int const imageHeight_;
    std::vector<QRgb> imageData_;
    QImage image_;

    int previousMinutesLow_;

    std::shared_ptr<Renderer2dClockGui> clockGui_;

};
//! [0]

#endif
