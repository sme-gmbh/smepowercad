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

#ifndef CAD_AIR_DUCTTRANSITIONRECTROUND_H
#define CAD_AIR_DUCTTRANSITIONRECTROUND_H

#include "caditem.h"
#include "cad_basic_duct.h"
#include "cad_basic_pipe.h"

class CAD_air_ductTransitionRectRound : public CADitem
{
public:
    CAD_air_ductTransitionRectRound();
    virtual ~CAD_air_ductTransitionRectRound();
    virtual QList<CADitemTypes::ItemType> flangable_items(int flangeIndex);
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual void paint(GLWidget* glwidget);
    virtual QMatrix4x4 rotationOfFlange(quint8 num);

    qreal a; //height of rect side
    qreal b; //width of rect side
    qreal d; //diameter of round size
    qreal e; //offset y
    qreal f; //offset z
    qreal l; //length of transition
    qreal u; //endcap length

    qreal ff, fe;
    qreal s;

    QVector3D roundside[2][2][32]; //front <-> back, inner <-> outer, discrete circle
    QVector3D rectside[2][2][4]; //front <-> back, inner <-> outer, "discrete circle"

    CAD_basic_pipe *flange_round, *endcap_round;
    CAD_basic_duct *flange_rect, *endcap_rect;

//    QOpenGLBuffer arrayBufVertices;
//    QOpenGLBuffer indexBufFaces;
//    QOpenGLBuffer indexBufLines;
};

#endif // CAD_AIR_DUCTTRANSITIONRECTROUND_H
