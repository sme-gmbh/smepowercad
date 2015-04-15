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

#ifndef CAD_HEATCOOL_32WAYVALVE_H
#define CAD_HEATCOOL_32WAYVALVE_H

#include "caditem.h"
#include "items/cad_basic_pipe.h"

class CAD_HeatCool_32WayValve : public CADitem
{
public:
    CAD_HeatCool_32WayValve();
    virtual ~CAD_HeatCool_32WayValve();
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

    qreal d, l, a1, a2, fe, ff, s;
    CAD_basic_pipe *pipe_lower, *flange_lower, *pipe, *flange_right, *flange_left, *endcap;
};

#endif // CAD_HEATCOOL_32WAYVALVE_H
