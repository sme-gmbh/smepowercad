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

#ifndef MTRIANGLE_H
#define MTRIANGLE_H

#include <QVector3D>
#include <QDataStream>
#include <QByteArray>


class MTriangle
{
public:
    MTriangle();
    MTriangle(const MTriangle &triangle);
    MTriangle(QVector3D v0, QVector3D v1, QVector3D v2);
    ~MTriangle();
    QVector3D getV0();
    QVector3D getV1();
    QVector3D getV2();
    bool operator ==(const MTriangle &t);


private:
    QVector3D v0;
    QVector3D v1;
    QVector3D v2;
};

#endif // MTRIANGLE_H
