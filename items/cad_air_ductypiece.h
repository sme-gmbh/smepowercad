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

#ifndef CAD_AIR_DUCTYPIECE_H
#define CAD_AIR_DUCTYPIECE_H

#include "caditem.h"
#include "cad_basic_duct.h"

class CAD_air_ductYpiece : public CADitem
{
public:
    CAD_air_ductYpiece();
    virtual ~CAD_air_ductYpiece();
    virtual QList<CADitemTypes::ItemType> flangable_items(int flangeIndex);
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual void paint(GLWidget* glwidget);
    virtual QMatrix4x4 rotationOfFlange(quint8 num);

    qreal a; //height 1
    qreal b; //width 1
    qreal a2; //height 2
    qreal b2; //width 2
    qreal e; //offset y 1
    qreal f; //offset z 1
    qreal b3; //width 3
    qreal l; //length;
    qreal m; //offset y 2
    qreal u; //endcap
    qreal s;
    qreal ff, fe;

    CAD_basic_duct *endcap_1;
    CAD_basic_duct *endcap_2;
    CAD_basic_duct *endcap_3;
    CAD_basic_duct *flange_1;
    CAD_basic_duct *flange_2;
    CAD_basic_duct *flange_3;

    QVector3D splitPoint[4];


    QOpenGLBuffer arrayBufVertices;
    QOpenGLBuffer indexBufFaces;
    QOpenGLBuffer indexBufLines;

};

#endif // CAD_AIR_DUCTYPIECE_H
