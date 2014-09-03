#ifndef CAD3DFACE_H
#define CAD3DFACE_H

#include "caditem.h"

class CAD3Dface : public CADitem
{
public:
    CAD3Dface();
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

#endif // CAD3DFACE_H
