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

#ifndef CAD_HEATCOOL_FLANGE_H
#define CAD_HEATCOOL_FLANGE_H

#include "caditem.h"
#include "items/cad_basic_pipe.h"

class CAD_heatcool_flange : public CADitem
{
public:
    CAD_heatcool_flange();
    virtual ~CAD_heatcool_flange();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual QMatrix4x4 rotationOfFlange(quint8 num);


    qreal d,ff, l, fe, s;
    CAD_basic_pipe *pipe, *flange;
};

#endif // CAD_HEATCOOL_FLANGE_H
