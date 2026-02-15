// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "helper.hpp"
#include "renderer2d.hpp"
#include "renderer2dclockgui.hpp"

#include <QPainter>
#include <QPaintEvent>
#include <QWidget>

#include <algorithm>
#include <memory>
#include <time.h>
#include <vector>

// #include <chrono>
// #include <iostream>



Helper::Helper()
    : imageWidth_(GuiFixtures::displayWidth)
    , imageHeight_(GuiFixtures::displayHeight)
    , renderData_(imageWidth_ * imageHeight_)
    , imageData_(imageWidth_ * imageHeight_)
    , image_(reinterpret_cast<uchar const *>(imageData_.data()), imageWidth_, imageHeight_, QImage::Format::Format_RGB32)
    , reducedDisplaySymbols_(false)
    , previousMinutesLow_(-1)
    , clockGui_(std::make_shared<Renderer2dClockGui>())
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



    std::chrono::steady_clock::time_point const steadyNow = std::chrono::steady_clock::now();
    if (std::holds_alternative<std::monostate>(firstRender_))
    {
        firstRender_ = steadyNow;
    }
    else
    {
        // intentionally empty
    }


    bool updateRender = true;

    if (steadyNow == std::get<std::chrono::steady_clock::time_point>(firstRender_))
    {
        // Show all symbols for the first second.
    }
    else if (std::chrono::seconds(1) >= steadyNow - std::get<std::chrono::steady_clock::time_point>(firstRender_))
    {
        // But render only once.
        updateRender = false;
    }
    else
    {
        if (not reducedDisplaySymbols_)
        {
            clockGui_->buttonUp.set(false);
            clockGui_->buttonDown.set(false);
            clockGui_->selection0.set(false);
            clockGui_->selection1.set(false);
            clockGui_->selection2.set(false);
            clockGui_->selection3.set(false);
            clockGui_->buttonDown.set(false);

            reducedDisplaySymbols_ = true;
        }
        else
        {
            // intentionally empty
        }


        std::time_t const nowTime = std::chrono::system_clock::to_time_t(timestamp);

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
                clockGui_->sevenSegments0.set(GuiFixtures::Renderer2dSevenSegments::singleDigitToDisplay(hoursHigh));
                clockGui_->sevenSegments1.set(GuiFixtures::Renderer2dSevenSegments::singleDigitToDisplay(hoursLow));
                clockGui_->sevenSegments2.set(GuiFixtures::Renderer2dSevenSegments::singleDigitToDisplay(minutesHigh));
                clockGui_->sevenSegments3.set(GuiFixtures::Renderer2dSevenSegments::singleDigitToDisplay(minutesLow));

                previousMinutesLow_ = minutesLow;
            }
            else
            {
                updateRender = false;
            }
        }
        else
        {
            clockGui_->sevenSegments0.set(GuiFixtures::Renderer2dSevenSegments::Display::number8);
            clockGui_->sevenSegments1.set(GuiFixtures::Renderer2dSevenSegments::Display::number8);
            clockGui_->sevenSegments2.set(GuiFixtures::Renderer2dSevenSegments::Display::number8);
            clockGui_->sevenSegments3.set(GuiFixtures::Renderer2dSevenSegments::Display::number8);
        }
    }

    if (updateRender)
    {
        // std::chrono::steady_clock::time_point const startFill = std::chrono::steady_clock::now();

        std::fill(renderData_.begin(), renderData_.end(), GuiFixtures::Colors::white);

        // std::chrono::steady_clock::time_point const startY = std::chrono::steady_clock::now();

        // for (int y = 0; image_.height() > y; ++y)
        // {
        //     // std::chrono::steady_clock::time_point const startX = std::chrono::steady_clock::now();

        //     for (int x = 0; image_.width() > x; ++x)
        //     {
                // std::chrono::steady_clock::time_point const startR = std::chrono::steady_clock::now();

                Coordinates2d::Position const offset(0, 0);
                Coordinates2d::Dimension const dimension(imageWidth_, imageHeight_);
                Renderer2d::Color * const data = renderData_.data();

                clockGui_->render(offset, dimension, data);

                // std::chrono::steady_clock::time_point const endR = std::chrono::steady_clock::now();
                // std::cout << "r [" << x << ", " << y << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(endR - startR).count() << " us" << std::endl;
        //     }

        //     // std::chrono::steady_clock::time_point const endX = std::chrono::steady_clock::now();
        //     // std::cout << "x: " << std::chrono::duration_cast<std::chrono::milliseconds>(endX - startX).count() << " ms" << std::endl;
        // }

        // std::chrono::steady_clock::time_point const endY = std::chrono::steady_clock::now();
        // std::cout << "y: " << std::chrono::duration_cast<std::chrono::milliseconds>(endY - startY).count() << " ms" << std::endl;
        // std::cout << "overall: " << std::chrono::duration_cast<std::chrono::milliseconds>(endY - startFill).count() << " ms" << std::endl;

        std::vector<Renderer2d::Color>::const_iterator render = renderData_.cbegin();
        std::vector<Renderer2d::Color>::const_iterator renderEnd = renderData_.cend();
        std::vector<QRgb>::iterator image = imageData_.begin();
        for (; renderEnd > render; ++render, ++image)
        {
            Renderer2d::Color const color = *render;
            *image = QColor(color, color, color).rgba();
        }

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
