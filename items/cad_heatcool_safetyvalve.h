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

#ifndef CAD_HEATCOOL_SAFETYVALVE_H
#define CAD_HEATCOOL_SAFETYVALVE_H

#include "caditem.h"
#include "items/cad_basic_pipe.h"
#include "items/cad_basic_box.h"

class CAD_heatcool_safetyValve : public CADitem
{
public:
    CAD_heatcool_safetyValve();
    virtual ~CAD_heatcool_safetyValve();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

    qreal a, a2, d, e, f, fe, ff, l1, l2, s;
    CAD_basic_pipe *pipe_left, *flange_left, *pipe_lower, *flange_lower, *valve;
    CAD_basic_box *handle_1, *handle_2;
};

#endif // CAD_HEATCOOL_SAFETYVALVE_H
