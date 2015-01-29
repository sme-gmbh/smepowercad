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

#include "m3dbox.h"

M3dBox::M3dBox()
{
}

M3dBox::M3dBox(const M3dBox &box)
{
    this->p0 = box.p0;
    this->a1 = box.a1;
    this->a2 = box.a2;
    this->a3 = box.a3;
}

M3dBox::M3dBox(QVector3D p0, QVector3D a1, QVector3D a2, QVector3D a3)
{
    this->p0 = p0;
    this->a1 = a1;
    this->a2 = a2;
    this->a3 = a3;
}

qreal M3dBox::volume()
{
    return qAbs(QVector3D::dotProduct(QVector3D::crossProduct(a1, a2), a3));
}

QVector3D M3dBox::center()
{
    return ((a1 * 0.5 + a2 * 0.5 + a3 * 0.5) + p0);
}

void M3dBox::translate(QVector3D delta)
{
    p0 += delta;
}
