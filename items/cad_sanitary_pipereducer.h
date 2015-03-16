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

#ifndef CAD_SANITARY_PIPEREDUCER_H
#define CAD_SANITARY_PIPEREDUCER_H

#include "caditem.h"
#include "items/cad_basic_pipe.h"
#include "items/cad_basic_pipereducer.h"

class CAD_sanitary_pipeReducer : public CADitem
{
public:
    CAD_sanitary_pipeReducer();
    virtual ~CAD_sanitary_pipeReducer();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

    qreal d1, d2, l, l1, l2, e, s, iso1, iso2;
    CAD_basic_pipe *left, *right;
    CAD_Basic_PipeReducer *reducer;


};

#endif // CAD_SANITARY_PIPEREDUCER_H
