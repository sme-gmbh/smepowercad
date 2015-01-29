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

#ifndef CAD_AIR_DUCTBAFFLESILENCER_H
#define CAD_AIR_DUCTBAFFLESILENCER_H

#include "caditem.h"
#include "cad_basic_duct.h"
#include "cad_basic_box.h"
#include <QList>

class CAD_air_ductBaffleSilencer : public CADitem
{
public:
    CAD_air_ductBaffleSilencer();
    virtual ~CAD_air_ductBaffleSilencer();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

    qreal a, b, l;
    qreal d, n;
    qreal ff, fe;
    qreal s;

    CAD_basic_duct* flange_duct_left;
    CAD_basic_duct* flange_duct_right;
    CAD_basic_duct* main_duct;

    QList<CAD_basic_box*> silencers;



};

#endif // CAD_AIR_DUCTBAFFLESILENCER_H
