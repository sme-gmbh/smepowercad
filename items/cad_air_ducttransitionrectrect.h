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

#ifndef CAD_AIR_DUCTTRANSITIONRECTRECT_H
#define CAD_AIR_DUCTTRANSITIONRECTRECT_H

#include "caditem.h"
#include "cad_basic_duct.h"

class CAD_air_ductTransitionRectRect : public CADitem
{
public:
    CAD_air_ductTransitionRectRect();
    virtual ~CAD_air_ductTransitionRectRect();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

private:
    qreal s;
    qreal endcap;
    qreal e, f; //offset dy and dz
    qreal ff, fe;
    qreal a, b, a2, b2, l;

    CAD_basic_duct *transition_duct;
    CAD_basic_duct *endcap_left_duct;
    CAD_basic_duct *endcap_right_duct;
    CAD_basic_duct *flange_left_duct;
    CAD_basic_duct *flange_right_duct;

};

#endif // CAD_AIR_DUCTTRANSITIONRECTRECT_H
