#include "m3dbox.h"

M3dBox::M3dBox()
{
}

M3dBox::M3dBox(const M3dBox &box)
{
    this->p0 = box.p0;
    this->a1 = box.a1;
    this->a2 = box.a2;
    this->a3 = box.a3;
}

M3dBox::M3dBox(QVector3D p0, QVector3D a1, QVector3D a2, QVector3D a3)
{
    this->p0 = p0;
    this->a1 = a1;
    this->a2 = a2;
    this->a3 = a3;
}

qreal M3dBox::volume()
{
    return qAbs(QVector3D::dotProduct(QVector3D::crossProduct(a1, a2), a3));
}

QVector3D M3dBox::center()
{
    return ((a1 * 0.5 + a2 * 0.5 + a3 * 0.5) + p0);
}

void M3dBox::translate(QVector3D delta)
{
    p0 += delta;
}
