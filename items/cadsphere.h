#ifndef CADSPHERE_H
#define CADSPHERE_H

#include "caditem.h"

class CADsphere : public CADitem
{
public:
    CADsphere();
    virtual void calculate();
};

#endif // CADSPHERE_H
