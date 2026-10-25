// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef HELPER_H
#define HELPER_H

#include "dummybuttons.hpp"
#include "dummyStatemachine.hpp"

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>

#include <chrono>
#include <cstdint>
#include <memory>
#include <variant>
#include <vector>

class Renderer2dClockGui;


//! [0]
class Helper
{
public:
    Helper(PinAndButton buttonSettings,
           PinAndButton buttonRight,
           PinAndButton buttonUp,
           PinAndButton buttonOnOffOrDown);

public:
    void paint(QPainter *painter, QSize const & viewport);

    void mousePress(QPoint const & point);
    void mouseRelease(QPoint const & point);

private:
    QBrush background;
    QBrush circleBrush;
    QFont textFont;
    QPen circlePen;
    QPen textPen;

    int const imageWidth_;
    int const imageHeight_;
    std::vector<uint8_t> renderData_;
    std::vector<QRgb> imageData_;
    QImage image_;

    std::shared_ptr<Renderer2dClockGui> clockGui_;

    PinAndButton pinAndButtonSettings_;
    PinAndButton pinAndButtonRight_;
    PinAndButton pinAndButtonUp_;
    PinAndButton pinAndButtonOnOffOrDown_;

    EpaperClock::Statemachine_t statemachine_;
    EpaperClock::DataType statemachineData_;

};
//! [0]

#endif
