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

#ifndef CAD_AIR_DUCTTEECONNECTOR_H
#define CAD_AIR_DUCTTEECONNECTOR_H

#include "caditem.h"
#include "cad_basic_duct.h"

class CAD_air_ductTeeConnector : public CADitem
{
public:
    CAD_air_ductTeeConnector();
    virtual ~CAD_air_ductTeeConnector();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual void paint(GLWidget* glwidget);

    qreal a;    //height of duct
    qreal b;    //width of first outlet
    qreal b2;    //width of second outlet
    qreal e;    //offset of second outlet in y
    qreal b3;    //width of third outlet
    qreal l;    //length from first outlet to second outlet
    qreal m;    //offset of second outlet to third outlet in y
    qreal n;    //offset of third outlet to first outlet in x
    qreal r1;   //radius of "left" turn
    qreal r2;   //radius of "right" turn
    qreal u;    //length of endcap
    qreal s;
    qreal ff,fe;

    CAD_basic_duct *endcap_1;
    CAD_basic_duct *endcap_2;
    CAD_basic_duct *endcap_3;

    CAD_basic_duct *flange_1;
    CAD_basic_duct *flange_2;
    CAD_basic_duct *flange_3;

    QVector3D vertices_turn1[2][12][2];
    QVector3D vertices_turn2[2][12][2];

    QVector3D vertices_backside[4];

    QOpenGLBuffer arrayBufVertices;
    QOpenGLBuffer indexBufFaces;
    QOpenGLBuffer indexBufLines;
};

#endif // CAD_AIR_DUCTTEECONNECTOR_H
