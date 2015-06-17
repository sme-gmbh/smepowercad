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
