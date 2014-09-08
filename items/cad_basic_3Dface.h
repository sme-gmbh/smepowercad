#ifndef CAD_BASIC_3DFACE_H
#define CAD_BASIC_3DFACE_H

#include "caditem.h"

class CAD_basic_3Dface : public CADitem
{
public:
    CAD_basic_3Dface();
    virtual void calculate();

    class Vertex{
    public:
        QVector3D pos;
        qreal bulge;
        qreal widthStart;
        qreal widthEnd;
    };

    QList<Vertex> vertices;

private:

};

#endif // CAD_BASIC_3DFACE_H
