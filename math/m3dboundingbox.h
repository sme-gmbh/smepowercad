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

#ifndef M3DBOUNDINGBOX_H
#define M3DBOUNDINGBOX_H

#include <QVector3D>
#include <QList>

class M3dBoundingBox
{
public:
    M3dBoundingBox();
    M3dBoundingBox(M3dBoundingBox &box);

    qreal x_min, x_max;
    qreal y_min, y_max;
    qreal z_min, z_max;

    void reset();
    void enterVertex(QVector3D vertex);
    void enterVertices(QList<QVector3D> vertices);
    bool intersectsWith(M3dBoundingBox &box);

    QList<QVector3D> getVertices();

    QVector3D p(int num);
    QVector3D p1();
    QVector3D p2();
    QVector3D p3();
    QVector3D p4();
    QVector3D p5();
    QVector3D p6();
    QVector3D p7();
    QVector3D p8();
};

#endif // M3DBOUNDINGBOX_H
