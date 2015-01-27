#ifndef M3DBOUNDINGBOX_H
#define M3DBOUNDINGBOX_H

#include <QVector3D>
#include <QList>

class M3dBoundingBox
{
public:
    M3dBoundingBox();
    M3dBoundingBox(M3dBoundingBox &box);

    qreal x_min, x_max;
    qreal y_min, y_max;
    qreal z_min, z_max;

    void reset();
    void enterVertex(QVector3D vertex);
    void enterVertices(QList<QVector3D> vertices);
    bool intersectsWith(M3dBoundingBox &box);

    QList<QVector3D> getVertices();

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
