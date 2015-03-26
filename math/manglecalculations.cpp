#include "manglecalculations.h"

MAngleCalculations::MAngleCalculations()
{

}

MAngleCalculations::~MAngleCalculations()
{

}

//calculates a set of kardan angles, which describe the same rotation as given by matrix_rotation
QVector3D MAngleCalculations::anglesFromMatrix(QMatrix4x4 matrix_rotation)
{
    qreal r11 = matrix_rotation.row(0).x();
    qreal r21 = matrix_rotation.row(1).x();
    qreal r31 = matrix_rotation.row(2).x();
    qreal r12 = matrix_rotation.row(0).y();
    qreal r13 = matrix_rotation.row(0).z();
    qreal r23 = matrix_rotation.row(1).z();
    qreal r33 = matrix_rotation.row(2).z();

    qreal phi, theta, psi;

    if(qAbs(r13 - 1) < EPS || qAbs(r13 + 1) < EPS)
    {
        phi = 0;
        qreal delta = atan2(r21, r31);
        if(qAbs(r13 + 1) < EPS)
        {
            theta = PI / 2;
            psi = phi + delta;
        }
        else
        {
            theta = -PI / 2;
            psi = - phi + delta;
        }
    }
    else
    {
        theta =  asin(r13);
        //qreal theta2 = -PI + theta1;
        psi = -atan2(r23 / cos(theta), r33 / cos(theta));
        //qreal psi2 = -atan2(r23 / cos(theta2), r33 / cos(theta2));
        phi = -atan2(r12 / cos(theta), r11 / cos(theta));
        //qreal phi2 = -atan2(r12 / cos(theta2), r11 / cos(theta2));
//        QMatrix4x4 m1;
//        m1.setToIdentity();
//        m1.rotate(psi1 / PI * 180.0,   1.0, 0.0, 0.0);
//        m1.rotate(theta1 / PI * 180.0, 0.0, 1.0, 0.0);
//        m1.rotate(phi1 / PI * 180.0,   0.0, 0.0, 1.0);
//        QMatrix4x4 m2;
//        m2.setToIdentity();
//        m2.rotate(psi2 / PI * 180.0,   1.0, 0.0, 0.0);
//        m2.rotate(theta2 / PI * 180.0, 0.0, 1.0, 0.0);
//        m2.rotate(phi2 / PI * 180.0,   0.0, 0.0, 1.0);
//        qDebug() << "m1: " << m1;
//        qDebug() << "original: " << matrix_rotation;
//        if(matrixNorm(matrix_rotation - m1) < matrixNorm(matrix_rotation - m2))
//        {
//            psi = psi1;
//            theta = theta1;
//            phi = phi1;
//        }
//        else
//        {
//            psi = psi2;
//            theta = theta2;
//            phi = phi2;
//        }
    }
    return QVector3D(psi / PI * 180.0, theta / PI * 180.0, phi / PI * 180.0);
}

//calculates the frobenius norm of the given matrix
qreal MAngleCalculations::matrixNorm(QMatrix4x4 matrix)
{
    qreal s = 0;
    s += matrix.row(0).x() * matrix.row(0).x();
    s += matrix.row(0).y() * matrix.row(0).y();
    s += matrix.row(0).w() * matrix.row(0).z();
    s += matrix.row(0).z() * matrix.row(0).w();
    s += matrix.row(1).x() * matrix.row(1).x();
    s += matrix.row(1).y() * matrix.row(1).y();
    s += matrix.row(1).w() * matrix.row(1).z();
    s += matrix.row(1).z() * matrix.row(1).w();
    s += matrix.row(2).x() * matrix.row(2).x();
    s += matrix.row(2).y() * matrix.row(2).y();
    s += matrix.row(2).w() * matrix.row(2).z();
    s += matrix.row(2).z() * matrix.row(2).w();
    s += matrix.row(3).x() * matrix.row(3).x();
    s += matrix.row(3).y() * matrix.row(3).y();
    s += matrix.row(3).w() * matrix.row(3).z();
    s += matrix.row(3).z() * matrix.row(3).w();
    return sqrt(s);
}


