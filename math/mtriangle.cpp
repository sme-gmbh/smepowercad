#include "mtriangle.h"


MTriangle::MTriangle()
{
    this->v0 = QVector3D();
    this->v1 = QVector3D();
    this->v2 = QVector3D();
}

MTriangle::MTriangle(const MTriangle &triangle)
{
    this->v0 = triangle.v0;
    this->v1 = triangle.v1;
    this->v2 = triangle.v2;
}

MTriangle::MTriangle(QVector3D v0, QVector3D v1, QVector3D v2)
{
    this->v0 = v0;
    this->v1 = v1;
    this->v2 = v2;
}

MTriangle::~MTriangle()
{

}

QVector3D MTriangle::getV0()
{
    return this->v0;
}

QVector3D MTriangle::getV1()
{
    return this->v1;
}

QVector3D MTriangle::getV2()
{
    return this->v2;
}

bool MTriangle::operator ==(const MTriangle &t)
{
    return ((v0 == t.v0)
            && (v1 == t.v1)
            && (v2 == t.v2));
}
