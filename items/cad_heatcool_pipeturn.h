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

#ifndef CAD_HEATCOOL_PIPETURN_H
#define CAD_HEATCOOL_PIPETURN_H

#include "caditem.h"
#include "cad_basic_pipe.h"
#include "cad_basic_turn.h"

class CAD_heatcool_pipeTurn : public CADitem
{
public:
    CAD_heatcool_pipeTurn();
    virtual ~CAD_heatcool_pipeTurn();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual QMatrix4x4 rotationOfFlange(quint8 num);

    qreal l1, l2, r, alpha, d, iso, s;
    CAD_basic_pipe *left, *right;
    CAD_basic_turn *turn;

};

#endif // CAD_HEATCOOL_PIPETURN_H
