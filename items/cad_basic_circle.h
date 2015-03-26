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

#ifndef CAD_BASIC_CIRCLE_H
#define CAD_BASIC_CIRCLE_H

#include "caditem.h"
#include <QList>

class CAD_basic_circle : public CADitem
{
public:
    CAD_basic_circle();
    virtual ~CAD_basic_circle();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual void paint(GLWidget* glwidget);
    virtual QMatrix4x4 rotationOfFlange(quint8 num);

    QVector3D center;
    qreal radius;
    qreal width;
    bool widthByLayer;
    bool widthByBlock;
    //QList<QVector3D> circle;

    QOpenGLBuffer arrayBufVertices;
    QOpenGLBuffer indexBufLines;
};

#endif // CAD_BASIC_CIRCLE_H
