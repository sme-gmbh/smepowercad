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
    //compare to: http://staff.city.ac.uk/~sbbh653/publications/euler.pdf
//    qDebug() << "original: " << matrix_rotation;
    qreal r11 = matrix_rotation.row(0).x();
    qreal r21 = matrix_rotation.row(1).x();
    qreal r22 = matrix_rotation.row(1).y();
    qreal r31 = matrix_rotation.row(2).x();
    qreal r12 = matrix_rotation.row(0).y();
    qreal r13 = matrix_rotation.row(0).z();
    qreal r23 = matrix_rotation.row(1).z();
    qreal r33 = matrix_rotation.row(2).z();

    qreal phi, theta, psi;

    if(qAbs(r13 - 1) < EPS || qAbs(r13 + 1) < EPS)
    {
        if(qAbs(r13 + 1) < EPS)
        {
//            qDebug() << "anglesFromMatrix case 1";
            phi = 0.0;
            qreal delta = atan2(r21, r31);
            theta = -PI / 2.0;
            psi = - phi - delta;
            QMatrix4x4 m1;
            m1.setToIdentity();
            m1.rotate(psi / PI * 180.0,   1.0, 0.0, 0.0);
            m1.rotate(theta / PI * 180.0, 0.0, 1.0, 0.0);
            m1.rotate(phi / PI * 180.0,   0.0, 0.0, 1.0);
//            qDebug() << "m1: " << m1;
        }
        if(qAbs(r13 - 1) < EPS)
        {
//            qDebug() << "anglesFromMatrix case 2";
//            qDebug() << "Theta = 90 oder Theta = -270";
            phi = 0.0;
            qreal theta2 = PI / 2.0;
            qreal psi2 = atan2(r21 , r22);
            QMatrix4x4 m2;
            m2.setToIdentity();
            m2.rotate(psi2 / PI * 180.0,   1.0, 0.0, 0.0);
            m2.rotate(theta2 / PI * 180.0, 0.0, 1.0, 0.0);
            m2.rotate(phi / PI * 180.0,   0.0, 0.0, 1.0);
//            qDebug() << "m2: " << m2;
            theta = theta2;
            psi = psi2;
        }
    }
    else
    {
//        qDebug() << "anglesFromMatrix case 3";

        qreal theta1 = asin(r13);
        qreal theta2 = PI - theta1;
        qreal psi1 = -atan2(r23 / cos(theta1), r33 / cos(theta1));
        qreal psi2 = -atan2(r23 / cos(theta2), r33 / cos(theta2));
        qreal phi1 = -atan2(r12 / cos(theta1), r11 / cos(theta1));
        qreal phi2 = -atan2(r12 / cos(theta2), r11 / cos(theta2));
        QMatrix4x4 m1;
        m1.setToIdentity();
        m1.rotate(psi1 / PI * 180.0,   1.0, 0.0, 0.0);
        m1.rotate(theta1 / PI * 180.0, 0.0, 1.0, 0.0);
        m1.rotate(phi1 / PI * 180.0,   0.0, 0.0, 1.0);
        QMatrix4x4 m2;
        m2.setToIdentity();
        m2.rotate(psi2 / PI * 180.0,   1.0, 0.0, 0.0);
        m2.rotate(theta2 / PI * 180.0, 0.0, 1.0, 0.0);
        m2.rotate(phi2 / PI * 180.0,   0.0, 0.0, 1.0);
//        qDebug() << "m1: " << m1;
//        qDebug() << "m2: " << m2;
        if(matrixNorm(matrix_rotation - m1) < matrixNorm(matrix_rotation - m2))
        {
            psi = psi1;
            theta = theta1;
            phi = phi1;
        }
        else
        {
            psi = psi2;
            theta = theta2;
            phi = phi2;
        }
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

QVector3D MAngleCalculations::anglesFromVector(QVector3D vector)
{
    qreal angle_x = 0.0;
    qreal angle_y = 0.0;
    qreal angle_z = 0.0;

    //optimize until difference angle ist smaller than 10E-6°
    quint32 i = 1;
    //tbd: we need an algorithm with faster convergence rate
    //at this moment we just terminate after 10000 steps
    while(difference(vector, angle_x, angle_y, angle_z) > 10E-6 && i < 10000)
    {
        //optimize angle_x
        if( difference(vector, angle_x + alpha(i), angle_y, angle_z) < difference(vector, angle_x, angle_y, angle_z))
            angle_x = angle_x + alpha(i);
        else if( difference(vector, angle_x - alpha(i), angle_y, angle_z) < difference(vector, angle_x, angle_y, angle_z))
            angle_x = angle_x - alpha(i);
        //optimize angle_y
        if( difference(vector, angle_x, angle_y + alpha(i), angle_z) < difference(vector, angle_x, angle_y, angle_z))
            angle_y = angle_y + alpha(i);
        else if( difference(vector, angle_x, angle_y - alpha(i), angle_z) < difference(vector, angle_x, angle_y, angle_z))
            angle_y = angle_y - alpha(i);
        //optimize angle_z
        if( difference(vector, angle_x, angle_y, angle_z + alpha(i)) < difference(vector, angle_x, angle_y, angle_z))
            angle_z = angle_z + alpha(i);
        else if( difference(vector, angle_x, angle_y, angle_z - alpha(i)) < difference(vector, angle_x, angle_y, angle_z))
            angle_z = angle_z - alpha(i);
        i++;
    }
    return QVector3D(angle_x, angle_y, angle_z);
}

qreal MAngleCalculations::difference(QVector3D vec, qreal angle_x, qreal angle_y, qreal angle_z)
{
    QMatrix4x4 matrix_difference;
    matrix_difference.setToIdentity();
    matrix_difference.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_difference.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_difference.rotate(angle_z, 0.0, 0.0, 1.0);
    QVector3D a = matrix_difference * QVector3D(1.0, 0.0, 0.0);
    qreal phi = acos(a.dotProduct(vec, a) / (vec.length() * a.length())) / PI * 180.0;
    return phi;
}

qreal MAngleCalculations::alpha(quint32 i)
{
    return 45.0 / i;
}


