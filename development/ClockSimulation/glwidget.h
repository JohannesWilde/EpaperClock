// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>

#include <chrono>


//! [0]
class Helper;

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget(Helper *helper, QWidget *parent);

public slots:
    void animate(std::chrono::system_clock::time_point const timestamp);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Helper *helper;
    std::chrono::system_clock::time_point timestamp_;
};
//! [0]

#endif
