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

#ifndef CAD_BASIC_BOX_H
#define CAD_BASIC_BOX_H

#include "caditem.h"

class CAD_basic_box : public CADitem
{
public:
    CAD_basic_box();
    virtual ~CAD_basic_box();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual void paint(GLWidget* glwidget);

    QOpenGLBuffer arrayBufVertices;
    QOpenGLBuffer indexBufFaces;
    QOpenGLBuffer indexBufLines;

    QVector3D pos_bot_1;
    QVector3D pos_bot_2;
    QVector3D pos_bot_3;
    QVector3D pos_bot_4;
    QVector3D pos_top_1;
    QVector3D pos_top_2;
    QVector3D pos_top_3;
    QVector3D pos_top_4;

    QVector3D size;

    QVector3D normal_bot;
    QVector3D normal_top;
    QVector3D normal_left;
    QVector3D normal_right;
    QVector3D normal_front;
    QVector3D normal_back;
};

#endif // CAD_BASIC_BOX_H
