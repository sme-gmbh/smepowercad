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

#include "m3dboundingbox.h"

M3dBoundingBox::M3dBoundingBox()
{
    reset();
}

M3dBoundingBox::M3dBoundingBox(M3dBoundingBox &box)
{
    this->x_min = box.x_min;
    this->x_max = box.x_max;
    this->y_min = box.y_min;
    this->y_max = box.y_max;
    this->z_min = box.z_min;
    this->z_max = box.z_max;
}

void M3dBoundingBox::reset()
{
    x_min =  10e10;
    x_max = -10e10;
    y_min =  10e10;
    y_max = -10e10;
    z_min =  10e10;
    z_max = -10e10;
}

void M3dBoundingBox::enterVertex(QVector3D vertex)
{
    qreal x = vertex.x();
    qreal y = vertex.y();
    qreal z = vertex.z();

    if (x < x_min)  x_min = x;
    if (y < y_min)  y_min = y;
    if (z < z_min)  z_min = z;

    if (x > x_max)  x_max = x;
    if (y > y_max)  y_max = y;
    if (z > z_max)  z_max = z;
}

void M3dBoundingBox::enterVertices(QList<QVector3D> vertices)
{
    foreach (QVector3D vertex, vertices)
    {
        enterVertex(vertex);
    }
}

bool M3dBoundingBox::intersectsWith(M3dBoundingBox &box)
{
    if (x_min > box.x_max)  return false;
    if (x_max < box.x_min)  return false;
    if (y_min > box.y_max)  return false;
    if (y_max < box.y_min)  return false;
    if (z_min > box.z_max)  return false;
    if (z_max < box.z_min)  return false;

    return true;
}

bool M3dBoundingBox::isEmpty()
{
    bool empty = false;

    if (x_min > x_max) empty = true;
    if (y_min > y_max) empty = true;
    if (z_min > z_max) empty = true;

    return empty;
}

// returns a given point of the box
QVector3D M3dBoundingBox::p(int num)
{
    switch (num)
    {
    case 0:
        return QVector3D(x_min, y_min, z_min);
        break;
    case 1:
        return QVector3D(x_max, y_min, z_min);
        break;
    case 2:
        return QVector3D(x_min, y_max, z_min);
        break;
    case 3:
        return QVector3D(x_max, y_max, z_min);
        break;
    case 4:
        return QVector3D(x_min, y_min, z_max);
        break;
    case 5:
        return QVector3D(x_max, y_min, z_max);
        break;
    case 6:
        return QVector3D(x_min, y_max, z_max);
        break;
    case 7:
        return QVector3D(x_max, y_max, z_max);
        break;
    }
    return QVector3D(); // happens if an invalid vertex is given
}

QVector3D M3dBoundingBox::p1()
{
    return QVector3D(x_min, y_min, z_min);
}

QVector3D M3dBoundingBox::p2()
{
    return QVector3D(x_max, y_min, z_min);
}

QVector3D M3dBoundingBox::p3()
{
    return QVector3D(x_min, y_max, z_min);
}

QVector3D M3dBoundingBox::p4()
{
    return QVector3D(x_max, y_max, z_min);
}

QVector3D M3dBoundingBox::p5()
{
    return QVector3D(x_min, y_min, z_max);
}

QVector3D M3dBoundingBox::p6()
{
    return QVector3D(x_max, y_min, z_max);
}

QVector3D M3dBoundingBox::p7()
{
    return QVector3D(x_min, y_max, z_max);
}

QVector3D M3dBoundingBox::p8()
{
    return QVector3D(x_max, y_max, z_max);
}

QList<QVector3D> M3dBoundingBox::getVertices()
{
    QList<QVector3D> ret = QList<QVector3D>();
    ret.append(this->p1());
    ret.append(this->p2());
    ret.append(this->p3());
    ret.append(this->p4());
    ret.append(this->p5());
    ret.append(this->p6());
    ret.append(this->p7());
    ret.append(this->p8());
    return ret;
}
