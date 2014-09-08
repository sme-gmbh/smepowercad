#ifndef CAD_BASIC_CIRCLE_H
#define CAD_BASIC_CIRCLE_H

#include "caditem.h"

class CAD_basic_circle : public CADitem
{
public:
    CAD_basic_circle();
    virtual void calculate();

    QVector3D pos;
    qreal radius;
};

#endif // CAD_BASIC_CIRCLE_H
