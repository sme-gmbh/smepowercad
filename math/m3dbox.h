#ifndef M3DBOX_H
#define M3DBOX_H

#include <QVector3D>

class M3dBox
{
public:
    M3dBox();
    M3dBox(const M3dBox &box);
    M3dBox(QVector3D p0, QVector3D a1, QVector3D a2, QVector3D a3);

    QVector3D p0;    // Insertion point
    QVector3D a1;   // Edge 1
    QVector3D a2;   // Edge 2
    QVector3D a3;   // Edge 3

    qreal volume();
    QVector3D center();

    void translate(QVector3D delta);



};

#endif // M3DBOX_H
