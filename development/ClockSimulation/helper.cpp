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



Helper::Helper(PinAndButton buttonSettings,
               PinAndButton buttonRight,
               PinAndButton buttonUp,
               PinAndButton buttonOnOffOrDown)
    : imageWidth_(GuiFixtures::displayWidth)
    , imageHeight_(GuiFixtures::displayHeight)
    , renderData_(imageWidth_ * imageHeight_)
    , imageData_(imageWidth_ * imageHeight_)
    , image_(reinterpret_cast<uchar const *>(imageData_.data()), imageWidth_, imageHeight_, QImage::Format::Format_RGB32)
    , clockGui_(std::make_shared<Renderer2dClockGui>())
    , pinAndButtonSettings_(buttonSettings)
    , pinAndButtonRight_(buttonRight)
    , pinAndButtonUp_(buttonUp)
    , pinAndButtonOnOffOrDown_(buttonOnOffOrDown)
    , statemachine_(EpaperClock::stateSplashScreen)
    , statemachineData_(pinAndButtonSettings_.button,
                        pinAndButtonRight_.button,
                        pinAndButtonUp_.button,
                        pinAndButtonOnOffOrDown_.button,
                        static_cast<bool>(clockGui_) ? *clockGui_ : throw std::exception("Invalid clockGui."),
                        /*previousMinutesLow*/ -1,
                        /*delay*/ 0,
                        /*updateDisplay*/ false)
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


void Helper::paint(QPainter *painter, QSize const & viewport)
{
    pinAndButtonSettings_.button.update();
    pinAndButtonRight_.button.update();
    pinAndButtonUp_.button.update();
    pinAndButtonOnOffOrDown_.button.update();

    statemachine_.process(statemachineData_);

    int const textSize = std::min(std::min(viewport.width(), viewport.height()), 50);
    textFont.setPixelSize(textSize);

    if (statemachineData_.updateDisplay & (0 < imageWidth_) & (0 < imageHeight_))
    {
        // todo: render only updated section? -> check RAM/flash requirements for tracking bounding rect
        // todo: fix dimensions [width - 1]

        std::chrono::steady_clock::time_point const startFill = std::chrono::steady_clock::now();

        std::fill(renderData_.begin(), renderData_.end(), GuiFixtures::Colors::white);

        // std::chrono::steady_clock::time_point const startY = std::chrono::steady_clock::now();

        size_t constexpr numberOfPixelsToRenderAtOnceMax = 1024 * 4;
        size_t const numberOfLinesToRenderAtOnce = numberOfPixelsToRenderAtOnceMax / imageWidth_;
        if (0 < numberOfLinesToRenderAtOnce)
        {

            for (int y = 0; image_.height() > y; y += numberOfLinesToRenderAtOnce)
            {
                // std::chrono::steady_clock::time_point const startX = std::chrono::steady_clock::now();

            //     for (int x = 0; image_.width() > x; ++x)
            //     {
                    // std::chrono::steady_clock::time_point const startR = std::chrono::steady_clock::now();

                    Coordinates2d::Position const offset(0, y);

                    size_t const remainingLines = imageHeight_ - y;
                    size_t const linesToRenderNow = std::min(remainingLines, numberOfLinesToRenderAtOnce);

                    Coordinates2d::Dimension const dimension(imageWidth_, linesToRenderNow);
                    Renderer2d::Color * const data = renderData_.data() + y * imageWidth_;

                    clockGui_->render(offset, dimension, data);

                    // std::chrono::steady_clock::time_point const endR = std::chrono::steady_clock::now();
                    // std::cout << "r [" << x << ", " << y << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(endR - startR).count() << " us" << std::endl;
            //     }

                // std::chrono::steady_clock::time_point const endX = std::chrono::steady_clock::now();
                // std::cout << "x: " << std::chrono::duration_cast<std::chrono::milliseconds>(endX - startX).count() << " ms" << std::endl;
            }
        }
        else
        {
            // Render sub-lines.
            assert(false);
        }

        std::chrono::steady_clock::time_point const endY = std::chrono::steady_clock::now();
        // std::cout << "y: " << std::chrono::duration_cast<std::chrono::milliseconds>(endY - startY).count() << " ms" << std::endl;
        std::cout << "overall: " << std::chrono::duration_cast<std::chrono::microseconds>(endY - startFill).count() << " us" << std::endl;

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

void Helper::mousePress(const QPoint & point)
{
    switch (positionToButton(Coordinates2d::Position{point.x(), point.y()}))
    {
    case ClockGuiButton::None:
    {
        break;
    }
    case ClockGuiButton::Settings:
    {
        pinAndButtonSettings_.pin.set(SimplePin::State::Zero);
        break;
    }
    case ClockGuiButton::Right:
    {
        pinAndButtonRight_.pin.set(SimplePin::State::Zero);
        break;
    }
    case ClockGuiButton::Up:
    {
        pinAndButtonUp_.pin.set(SimplePin::State::Zero);
        break;
    }
    case ClockGuiButton::OnOffOrDown:
    {
        pinAndButtonOnOffOrDown_.pin.set(SimplePin::State::Zero);
        break;
    }
    }
}

void Helper::mouseRelease(const QPoint & point)
{
    switch (positionToButton(Coordinates2d::Position{point.x(), point.y()}))
    {
    case ClockGuiButton::None:
    {
        break;
    }
    case ClockGuiButton::Settings:
    {
        pinAndButtonSettings_.pin.set(SimplePin::State::One);
        break;
    }
    case ClockGuiButton::Right:
    {
        pinAndButtonRight_.pin.set(SimplePin::State::One);
        break;
    }
    case ClockGuiButton::Up:
    {
        pinAndButtonUp_.pin.set(SimplePin::State::One);
        break;
    }
    case ClockGuiButton::OnOffOrDown:
    {
        pinAndButtonOnOffOrDown_.pin.set(SimplePin::State::One);
        break;
    }
    }
}
