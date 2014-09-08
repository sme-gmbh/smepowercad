#ifndef CADARC_H
#define CADARC_H

#include "caditem.h"

class CADarc : public CADitem
{
public:
    CADarc();
    virtual void calculate();
};

#endif // CADARC_H
