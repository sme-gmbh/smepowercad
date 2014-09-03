#ifndef CADPOLYLINE_H
#define CADPOLYLINE_H

#include "caditem.h"

class CADpolyline : public CADitem
{
public:
    CADpolyline();
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

#endif // CADPOLYLINE_H
