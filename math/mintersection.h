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

#ifndef MINTERSECTION_H
#define MINTERSECTION_H
#define TOL 10E-8
#define TOL_INEXACT 10E-1

#include <QVector3D>
#include <QVector2D>
#include <math/mtriangle.h>
#include <math/m3dboundingbox.h>

class MIntersection
{
public:
    MIntersection();
    ~MIntersection();

     bool trianglesIntersect(QVector3D v0, QVector3D v1, QVector3D v2, QVector3D w0, QVector3D w1, QVector3D w2);
     bool trianglesIntersect(MTriangle t1, MTriangle t2);

private:
    void interval(float VV0, float VV1, float VV2, float D0, float D1, float D2, float D0D1, float D0D2, float* A, float* B, float* C, float* X0, float* X1);
    bool edgeAgainstEdge(QVector2D v0, QVector2D v1, QVector2D w0, QVector2D w1);
    bool pointOnLineSegment(QVector2D p, QVector2D v0, QVector2D v1);
    bool vertexInTriangle(QVector2D v0, QVector2D w0, QVector2D w1, QVector2D w2);
    bool trianglesOnlyTouch(float dist_v0, float dist_v1, float dist_v2, float dist_w0, float dist_w1, float dist_w2);
    bool verticesOnSameSide(QVector2D p1,QVector2D p2, QVector2D a, QVector2D b);
    QVector3D randomVector();
};

#endif // MINTERSECTION_H
