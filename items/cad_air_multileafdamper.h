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

#ifndef CAD_AIR_MULTILEAFDAMPER_H
#define CAD_AIR_MULTILEAFDAMPER_H

#include "caditem.h"
#include "items/cad_basic_box.h"
#include "items/cad_basic_duct.h"

class CAD_air_multiLeafDamper : public CADitem
{
public:
    CAD_air_multiLeafDamper();
    virtual ~CAD_air_multiLeafDamper();
    virtual QList<CADitemTypes::ItemType> flangable_items(int flangeIndex);
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual QMatrix4x4 rotationOfFlange(quint8 num);

    qreal a, b, l, fe, ff, s;
    quint8 n;

    CAD_basic_duct *main_duct, *flange_duct_left, *flange_duct_right;
    CAD_basic_box *function;
};

#endif // CAD_AIR_MULTILEAFDAMPER_H
