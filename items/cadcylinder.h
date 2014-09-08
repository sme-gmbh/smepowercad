#ifndef CADCYLINDER_H
#define CADCYLINDER_H

#include "caditem.h"

class CADcylinder : public CADitem
{
public:
    CADcylinder();
    virtual void calculate();
};

#endif // CADCYLINDER_H
