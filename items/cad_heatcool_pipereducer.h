#ifndef CAD_HEATCOOL_PIPEREDUCER_H
#define CAD_HEATCOOL_PIPEREDUCER_H

#include "caditem.h"

class CAD_heatcool_pipeReducer : public CADitem
{
public:
    CAD_heatcool_pipeReducer();
    virtual void calculate();
};

#endif // CAD_HEATCOOL_PIPEREDUCER_H
