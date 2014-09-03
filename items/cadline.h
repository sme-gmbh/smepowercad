#ifndef CADLINE_H
#define CADLINE_H

#include "caditem.h"

class CADline : public CADitem
{
public:
    CADline();
    virtual void calculate();

    QVector3D p1;
    QVector3D p2;
    qreal width;
    bool widthByLayer;
    bool widthByBlock;

private:

};

#endif // CADLINE_H
