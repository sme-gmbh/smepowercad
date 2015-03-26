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

#ifndef CAD_SANITARY_PIPETEECONNECTOR_H
#define CAD_SANITARY_PIPETEECONNECTOR_H

#include "caditem.h"
#include "items/cad_basic_pipe.h"

class CAD_sanitary_pipeTeeConnector : public CADitem
{
public:
    CAD_sanitary_pipeTeeConnector();
    virtual ~CAD_sanitary_pipeTeeConnector();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual QMatrix4x4 rotationOfFlange(quint8 num);

    qreal l, l2, l3,  d, d3, iso, iso3, alpha, s;
    CAD_basic_pipe *pipe, *branch;
};

#endif // CAD_SANITARY_PIPETEECONNECTOR_H
