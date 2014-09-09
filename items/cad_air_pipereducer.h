#ifndef CAD_AIR_PIPEREDUCER_H
#define CAD_AIR_PIPEREDUCER_H

#include "caditem.h"

class CAD_air_pipeReducer : public CADitem
{
public:
    CAD_air_pipeReducer();
    virtual void calculate();
};

#endif // CAD_AIR_PIPEREDUCER_H
