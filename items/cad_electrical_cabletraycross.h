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

#ifndef CAD_ELECTRICAL_CABLETRAYCROSS_H
#define CAD_ELECTRICAL_CABLETRAYCROSS_H

#include "caditem.h"
#include "items/cad_basic_box.h"
#include "items/cad_electrical_cabletray.h"

class CAD_Electrical_CabletrayCross : public CADitem
{
public:
    CAD_Electrical_CabletrayCross();
    virtual ~CAD_Electrical_CabletrayCross();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
//    virtual void paint(GLWidget* glwidget);

//    QOpenGLBuffer arrayBufVertices;
//    QOpenGLBuffer indexBufFaces;
//    QOpenGLBuffer indexBufLines;
    qreal a, b, b1, l, l1, s;
    CAD_basic_box *cross;
    CAD_electrical_cabletray *ct_1, *ct_2, *ct_3, *ct_4;
};

#endif // CAD_ELECTRICAL_CABLETRAYCROSS_H
