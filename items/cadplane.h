#ifndef CADPLANE_H
#define CADPLANE_H

#include "caditem.h"

class CADplane : public CADitem
{
public:
    CADplane();
    virtual void calculate();
};

#endif // CADPLANE_H
