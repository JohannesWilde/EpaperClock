// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "glwidget.h"
#include "window.h"

#include <QGridLayout>
#include <QLabel>
#include <QTimer>

#include <chrono>


//! [0]
Window::Window()
{
    setWindowTitle(tr("Simulating Clock"));

    guiWidget_ = new GLWidget(&helper, this);
    labelWidget_ = new QLabel(tr("Text"));
    labelWidget_->setAlignment(Qt::AlignHCenter);
    labelWidget_->setSizePolicy(QSizePolicy(/*horizontal*/ QSizePolicy::Policy::Expanding,
                                            /*vertical*/ QSizePolicy::Policy::Fixed));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(guiWidget_, 0, 1);
    layout->addWidget(labelWidget_, 1, 1);
    setLayout(layout);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Window::update);
    timer->start(200);
}


void Window::update()
{
    std::chrono::system_clock::time_point const now = std::chrono::system_clock::now();

    guiWidget_->animate(now);
}

//! [0]
