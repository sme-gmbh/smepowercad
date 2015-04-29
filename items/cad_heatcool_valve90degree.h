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

#ifndef CAD_HEATCOOL_VALVE90DEGREE_H
#define CAD_HEATCOOL_VALVE90DEGREE_H

#include "caditem.h"
#include "items/cad_basic_pipe.h"

class CAD_HeatCool_Valve90Degree : public CADitem
{
public:
    CAD_HeatCool_Valve90Degree();
    virtual ~CAD_HeatCool_Valve90Degree();
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
    qreal a1, a2, d, fe, ff, l, s;
    CAD_basic_pipe *pipe_lower, *flange_lower, *pipe_right, *flange_right, *endcap;
};

#endif // CAD_HEATCOOL_VALVE90DEGREE_H
