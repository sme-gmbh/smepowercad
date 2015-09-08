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

#ifndef CAD_CLEANROOM_DOORSLIDINGDOUBLE_H
#define CAD_CLEANROOM_DOORSLIDINGDOUBLE_H

#include "caditem.h"
#include "items/cad_basic_box.h"
#include "items/cad_basic_line.h"

class CAD_Cleanroom_DoorSlidingDouble : public CADitem
{
public:
    CAD_Cleanroom_DoorSlidingDouble();
    virtual ~CAD_Cleanroom_DoorSlidingDouble();
    virtual QList<CADitemTypes::ItemType> flangable_items(int flangeIndex);
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual QMatrix4x4 rotationOfFlange(quint8 num);
//    virtual void paint(GLWidget* glwidget);

//    QOpenGLBuffer arrayBufVertices;
//    QOpenGLBuffer indexBufFaces;
//    QOpenGLBuffer indexBufLines;

    qreal h, b, g;
    CAD_basic_box *door_left, *door_right;
    CAD_basic_line *arrow_front_11, *arrow_front_12, *arrow_front_13, *arrow_front_14;
    CAD_basic_line *arrow_back_11, *arrow_back_12, *arrow_back_13, *arrow_back_14;
    CAD_basic_line *arrow_top_11, *arrow_top_12, *arrow_top_13, *arrow_top_14;
    CAD_basic_line *arrow_front_21, *arrow_front_22, *arrow_front_23, *arrow_front_24;
    CAD_basic_line *arrow_back_21, *arrow_back_22, *arrow_back_23, *arrow_back_24;
    CAD_basic_line *arrow_top_21, *arrow_top_22, *arrow_top_23, *arrow_top_24;
};

#endif // CAD_CLEANROOM_DOORSLIDINGDOUBLE_H
