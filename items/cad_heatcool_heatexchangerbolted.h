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

#ifndef CAD_HEATCOOL_HEATEXCHANGERBOLTED_H
#define CAD_HEATCOOL_HEATEXCHANGERBOLTED_H

#include "caditem.h"

#include "caditem.h"
#include "items/cad_basic_box.h"
#include "items/cad_basic_pipe.h"

class CAD_HeatCool_HeatexchangerBolted : public CADitem
{
public:
    CAD_HeatCool_HeatexchangerBolted();
    virtual ~CAD_HeatCool_HeatexchangerBolted();
    virtual QList<CADitemTypes::ItemType> flangable_items();
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
    qreal a, a2, b, d, e, f, l, l2, l3, s;
    CAD_basic_box *body, *plate_front, *plate_back, *mounting_top, *mounting_bottom;
    CAD_basic_pipe *pipe_1, *pipe_2, *pipe_3, *pipe_4;
};

#endif // CAD_HEATCOOL_HEATEXCHANGERBOLTED_H
