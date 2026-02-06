// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "helper.h"

#include <QPainter>
#include <QPaintEvent>
#include <QWidget>

#include <algorithm>


//! [0]
Helper::Helper()
{
    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, QColor(0xa6, 0xce, 0x39));

    background = QBrush(QColor(64, 32, 64));
    circleBrush = QBrush(gradient);
    circlePen = QPen(Qt::black);
    circlePen.setWidth(1);
    textPen = QPen(Qt::white);
    textFont.setPixelSize(50);
}
//! [0]

//! [1]
void Helper::paint(QPainter *painter, QPaintEvent *event, QSize const & viewport)
{
    int const textSize = std::min(std::min(viewport.width(), viewport.height()), 50);
    textFont.setPixelSize(textSize);


    QPixmap * pix = new QPixmap(264, 176);
    pix->fill(background.color());

    painter->drawPixmap(/*target*/ QRect(0, 0, viewport.width(), viewport.height()),
                        *pix,
                        /*source*/ QRect(0, 0, pix->width(), pix->height()));

    delete pix;


    painter->translate(viewport.width() / 2, viewport.height() / 2);
//! [1]


//! [3]
    painter->setPen(textPen);
    painter->setFont(textFont);
    painter->drawText(QRect(-textSize, -textSize, 2 * textSize, 2 * textSize), Qt::AlignCenter, QStringLiteral("Qt"));
}
//! [3]
