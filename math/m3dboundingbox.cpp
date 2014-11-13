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
