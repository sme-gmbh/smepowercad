#ifndef CAD_SPRINKLER_HEAD_H
#define CAD_SPRINKLER_HEAD_H

#include "caditem.h"

class CAD_sprinkler_head : public CADitem
{
public:
    CAD_sprinkler_head();
    virtual void calculate();
};

#endif // CAD_SPRINKLER_HEAD_H
