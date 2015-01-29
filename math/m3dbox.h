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

#ifndef M3DBOX_H
#define M3DBOX_H

#include <QVector3D>

class M3dBox
{
public:
    M3dBox();
    M3dBox(const M3dBox &box);
    M3dBox(QVector3D p0, QVector3D a1, QVector3D a2, QVector3D a3);

    QVector3D p0;    // Insertion point
    QVector3D a1;   // Edge 1
    QVector3D a2;   // Edge 2
    QVector3D a3;   // Edge 3

    qreal volume();
    QVector3D center();

    void translate(QVector3D delta);



};

#endif // M3DBOX_H
