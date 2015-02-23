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

#ifndef CAD_ELECTRICAL_CABINET_H
#define CAD_ELECTRICAL_CABINET_H

#include "caditem.h"
#include "items/cad_basic_duct.h"
#include "items/cad_basic_box.h"

class CAD_electrical_cabinet : public CADitem
{
public:
    CAD_electrical_cabinet();
    virtual ~CAD_electrical_cabinet();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

    qreal a, a1, b, l, i, alpha, s;
    CAD_basic_duct *cabinet;
    CAD_basic_box *back, *door, *socket;
};

#endif // CAD_ELECTRICAL_CABINET_H
