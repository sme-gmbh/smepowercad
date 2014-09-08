#ifndef CADCIRCLE_H
#define CADCIRCLE_H

#include "caditem.h"

class CADcircle : public CADitem
{
public:
    CADcircle();
    virtual void calculate();
};

#endif // CADCIRCLE_H
