// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef WINDOW_H
#define WINDOW_H

#include "helper.hpp"

#include <QWidget>

class GLWidget;
class QLabel;

//! [0]
class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

protected:

    void update();

private:
    Helper helper;

    GLWidget * guiWidget_;
    QLabel * labelWidget_;
};
//! [0]

#endif
