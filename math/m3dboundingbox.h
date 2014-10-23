#ifndef M3DBOUNDINGBOX_H
#define M3DBOUNDINGBOX_H

#include <QVector3D>

class M3dBoundingBox
{
public:
    M3dBoundingBox();

    qreal x_min, x_max;
    qreal y_min, y_max;
    qreal z_min, z_max;

    void reset();
    void enterVertex(QVector3D vertex);

    QVector3D p(int num);
    QVector3D p1();
    QVector3D p2();
    QVector3D p3();
    QVector3D p4();
    QVector3D p5();
    QVector3D p6();
    QVector3D p7();
    QVector3D p8();
};

#endif // M3DBOUNDINGBOX_H
