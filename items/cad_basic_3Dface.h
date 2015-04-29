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

#ifndef CAD_BASIC_3DFACE_H
#define CAD_BASIC_3DFACE_H

#include "caditem.h"

class CAD_basic_3Dface : public CADitem
{
public:
    CAD_basic_3Dface();
    virtual ~CAD_basic_3Dface();
    virtual QList<CADitemTypes::ItemType> flangable_items(int flangeIndex);
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual void paint(GLWidget* glwidget);
    virtual QMatrix4x4 rotationOfFlange(quint8 num);

    class Vertex{
    public:
        QVector3D pos;
        qreal bulge;
        qreal widthStart;
        qreal widthEnd;
    };

    QList<Vertex> vertices;

private:

};

#endif // CAD_BASIC_3DFACE_H
