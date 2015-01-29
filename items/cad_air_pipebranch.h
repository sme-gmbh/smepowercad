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

#ifndef CAD_AIR_PIPEBRANCH_H
#define CAD_AIR_PIPEBRANCH_H

#include "caditem.h"
#include "cad_basic_pipe.h"

class CAD_air_pipeBranch : public CADitem
{
public:
    CAD_air_pipeBranch();
    virtual ~CAD_air_pipeBranch();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

    qreal d, d2;
    qreal s;
    qreal l, l1 , l2;
    qreal alpha;

    CAD_basic_pipe *pipe, *branch;

};

#endif // CAD_AIR_PIPEBRANCH_H
