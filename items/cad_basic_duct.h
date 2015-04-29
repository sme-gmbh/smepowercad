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

#ifndef CAD_BASIC_DUCT_H
#define CAD_BASIC_DUCT_H

#include "caditem.h"

class CAD_basic_duct : public CADitem
{
public:
    CAD_basic_duct();
    virtual ~CAD_basic_duct();
    virtual QList<CADitemTypes::ItemType> flangable_items(int flangeIndex);
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual void paint(GLWidget* glwidget);
    virtual QMatrix4x4 rotationOfFlange(quint8 num);

    QVector3D pos_bot_1;
    QVector3D pos_bot_2;
    QVector3D pos_bot_3;
    QVector3D pos_bot_4;
    QVector3D pos_top_1;
    QVector3D pos_top_2;
    QVector3D pos_top_3;
    QVector3D pos_top_4;

    QVector3D inner_pos_bot_1;
    QVector3D inner_pos_bot_2;
    QVector3D inner_pos_bot_3;
    QVector3D inner_pos_bot_4;
    QVector3D inner_pos_top_1;
    QVector3D inner_pos_top_2;
    QVector3D inner_pos_top_3;
    QVector3D inner_pos_top_4;

    QVector3D size;

    qreal s;

    QOpenGLBuffer arrayBufVertices;
    QOpenGLBuffer indexBufFaces;
    QOpenGLBuffer indexBufLines;

};

#endif // CAD_BASIC_DUCT_H
