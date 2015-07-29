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

#ifndef MANGLECALCULATIONS_H
#define MANGLECALCULATIONS_H

#define EPS 10E-8
#define PI 3.1415926535897

#include "qmath.h"
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>

class MAngleCalculations
{
public:
    MAngleCalculations();
    ~MAngleCalculations();
    QVector3D anglesFromMatrix(QMatrix4x4 matrix_rotation);
    qreal matrixNorm(QMatrix4x4 matrix);
    QVector3D anglesFromVector(QVector3D vector);
    QMatrix4x4 rotateAroundAxis(qreal angle, QVector3D axis);
    QQuaternion matrixToQuaternion(QMatrix4x4 mat);
    QMatrix4x4 quaternionToMatrix(QQuaternion quat);

private:
    qreal difference(QVector3D vec, qreal alpha, qreal beta, qreal gamma);
    qreal alpha(quint32 i);
};

#endif // MANGLECALCULATIONS_H
