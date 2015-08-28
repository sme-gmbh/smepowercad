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

#ifndef CAD_ARCH_WINDOW_H
#define CAD_ARCH_WINDOW_H

#include "caditem.h"
#include "items/cad_basic_box.h"
#include "items/cad_basic_arc.h"
#include "items/cad_basic_line.h"

class CAD_arch_window : public CADitem
{
public:
    CAD_arch_window();
    virtual ~CAD_arch_window();
    virtual QList<CADitemTypes::ItemType> flangable_items(int flangeIndex);
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
    virtual QMatrix4x4 rotationOfFlange(quint8 num);

    qreal a, b, l, s1, s2, alpha;
    CAD_basic_box *box_left, *box_up, *box_right, *box_low, *window;
    CAD_basic_arc *arc;
    CAD_basic_line *tilt_arrow_1, *tilt_arrow_2;
    CAD_basic_line *swing_arrow_1, *swing_arrow_2;
};

#endif // CAD_ARCH_WINDOW_H
