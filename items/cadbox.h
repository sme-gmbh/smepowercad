#ifndef CADBOX_H
#define CADBOX_H

#include "caditem.h"

class CADbox : public CADitem
{
public:
    CADbox();
    virtual void calculate();
};

#endif // CADBOX_H
