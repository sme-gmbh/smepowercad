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

#ifndef CAD_BASIC_TURN_H
#define CAD_BASIC_TURN_H

#include "caditem.h"

#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>

class CAD_basic_turn : public CADitem
{
public:
    CAD_basic_turn();
    virtual ~CAD_basic_turn();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual void paint(GLWidget* glwidget);
    virtual QMatrix4x4 rotationOfFlange(quint8 num);


    qreal radius_pipe;
    qreal radius_turn;
    qreal angle_turn;
    qreal wallThickness;

//    QVector3D vertices[2][11][21];
//    QVector3D direction;

    QOpenGLBuffer arrayBufVertices;
    QOpenGLBuffer indexBufFaces;
    QOpenGLBuffer indexBufLines;
};

#endif // CAD_BASIC_TURN_H
