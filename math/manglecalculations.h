#ifndef MANGLECALCULATIONS_H
#define MANGLECALCULATIONS_H

#define EPS 10E-8
#define PI 3.1415926535897

#include "qmath.h"
#include <QMatrix4x4>
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

private:
    qreal difference(QVector3D vec, qreal alpha, qreal beta, qreal gamma);
    qreal alpha(quint32 i);
};

#endif // MANGLECALCULATIONS_H
