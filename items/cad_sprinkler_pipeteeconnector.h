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

#ifndef CAD_SPRINKLER_PIPETEECONNECTOR_H
#define CAD_SPRINKLER_PIPETEECONNECTOR_H

#include "caditem.h"
#include "items/cad_basic_pipe.h"

class CAD_sprinkler_pipeTeeConnector : public CADitem
{
public:
    CAD_sprinkler_pipeTeeConnector();
    virtual ~CAD_sprinkler_pipeTeeConnector();
    virtual QList<CADitemTypes::ItemType> flangable_items(int flangeIndex);
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual QMatrix4x4 rotationOfFlange(quint8 num);

    qreal d, d3;
    qreal s;
    qreal l, l2 , l3;
    qreal alpha;

    CAD_basic_pipe *pipe, *branch;
};

#endif // CAD_SPRINKLER_PIPETEECONNECTOR_H
