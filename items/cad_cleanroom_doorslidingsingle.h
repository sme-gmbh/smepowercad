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

#ifndef CAD_CLEANROOM_DOORSLIDINGSINGLE_H
#define CAD_CLEANROOM_DOORSLIDINGSINGLE_H

#include "caditem.h"
#include "items/cad_basic_box.h"
#include "items/cad_basic_line.h"

class CAD_Cleanroom_DoorSlidingSingle : public CADitem
{
public:
    CAD_Cleanroom_DoorSlidingSingle();
    virtual ~CAD_Cleanroom_DoorSlidingSingle();
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
    bool opensToLeft;
    CAD_basic_box *door;
    CAD_basic_line *arrow_front_1, *arrow_front_2, *arrow_front_3, *arrow_front_4;
    CAD_basic_line *arrow_back_1, *arrow_back_2, *arrow_back_3, *arrow_back_4;
    CAD_basic_line *arrow_top_1, *arrow_top_2, *arrow_top_3, *arrow_top_4;

};

#endif // CAD_CLEANROOM_DOORSLIDINGSINGLE_H
