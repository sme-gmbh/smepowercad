/**********************************************************************
** smepowercad
** Copyright (C) 2015 Smart Micro Engineering GmbH
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/

#ifndef LAYER_H
#define LAYER_H

#include <QBrush>
#include <QColor>
#include <QPen>
#include <QObject>
#include <QString>

#include "caditem.h"

class Layer : public QObject
{
    Q_OBJECT
public:
    explicit Layer(QObject *parent = 0);
    bool isEmpty();

    QString name;
    QPen pen;
    QBrush brush;
    bool on;
    bool solo;
    bool writable;
    int width;
    QString lineType;
    QList<CADitem*> items;
    QList<Layer*> subLayers;
    Layer* parentLayer;

    void serialOut(QByteArray *out);
    void serialIn(QByteArray *in);


signals:

public slots:

};

#endif // LAYER_H
