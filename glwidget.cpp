// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "glwidget.h"
#include "helper.h"

#include <QPainter>
#include <QTimer>

//! [0]
GLWidget::GLWidget(Helper *helper, QWidget *parent)
    : QOpenGLWidget(parent), helper(helper)
{
    setAutoFillBackground(false);
    setMinimumSize(20, 20);
}
//! [0]

//! [1]
void GLWidget::animate(std::chrono::system_clock::time_point const now)
{
    timestamp_ = now;
    update();
}
//! [1]

//! [2]
void GLWidget::paintEvent(QPaintEvent *event)
{
    QSize const widgetSize = size();

    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    helper->paint(&painter, event, widgetSize);
    painter.end();
}
//! [2]
