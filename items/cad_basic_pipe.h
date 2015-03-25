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

#ifndef CAD_BASIC_PIPE_H
#define CAD_BASIC_PIPE_H

#include "caditem.h"

#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>

class CAD_basic_pipe : public CADitem
{
public:
    CAD_basic_pipe();
    virtual ~CAD_basic_pipe();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual void paint(GLWidget* glwidget);
    virtual QMatrix4x4 rotationOfFlange(quint8 num);


    qreal radius;
    qreal length;
    qreal wallThickness;
    QVector3D direction;

    QList<QVector3D> vertices_outer_bottom;
    QList<QVector3D> vertices_inner_bottom;

    QList<QVector3D> vertices_outer_top;
    QList<QVector3D> vertices_inner_top;

    QOpenGLBuffer arrayBufVertices;
    QOpenGLBuffer indexBufFaces;
    QOpenGLBuffer indexBufLines;


};

#endif // CAD_BASIC_PIPE_H
