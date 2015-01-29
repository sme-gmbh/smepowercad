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

#ifndef CAD_AIR_HEATEXCHANGERWATERAIR_H
#define CAD_AIR_HEATEXCHANGERWATERAIR_H

#include "caditem.h"
#include "cad_air_duct.h"
#include "cad_basic_box.h"
#include "cad_basic_pipe.h"

class CAD_air_heatExchangerWaterAir : public CADitem
{
public:
    CAD_air_heatExchangerWaterAir();
    virtual ~CAD_air_heatExchangerWaterAir();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

    qreal s, ff, fe;
    qreal l, l3, l4;
    qreal d, j;
    qreal m3, m4;
    qreal a, b;

    CAD_air_duct *duct;
    CAD_basic_pipe *pipe3, *pipe4;
    CAD_basic_box *exchanger;
};

#endif // CAD_AIR_HEATEXCHANGERWATERAIR_H
