#ifndef CAD_BASIC_POLYLINE_H
#define CAD_BASICPOLYLINE_H

#include "caditem.h"

class CAD_basic_polyline : public CADitem
{
public:
    CAD_basic_polyline();
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

#endif // CAD_BASIC_POLYLINE_H
