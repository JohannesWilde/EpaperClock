// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "glwidget.h"
#include "window.h"

#include <QGridLayout>
#include <QLabel>
#include <QTimer>

//! [0]
Window::Window()
{
    setWindowTitle(tr("Simulating Clock"));

    GLWidget *openGL = new GLWidget(&helper, this);
    QLabel *openGLLabel = new QLabel(tr("Text"));
    openGLLabel->setAlignment(Qt::AlignHCenter);
    openGLLabel->setSizePolicy(QSizePolicy(/*horizontal*/ QSizePolicy::Policy::Expanding,
                                           /*vertical*/ QSizePolicy::Policy::Fixed));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(openGL, 0, 1);
    layout->addWidget(openGLLabel, 1, 1);
    setLayout(layout);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, openGL, &GLWidget::animate);
    timer->start(200);
}
//! [0]
