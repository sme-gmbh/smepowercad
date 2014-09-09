#ifndef CAD_HEATCOOL_PIPE_H
#define CAD_HEATCOOL_PIPE_H

#include "caditem.h"

class CAD_heatcool_pipe : public CADitem
{
public:
    CAD_heatcool_pipe();
    virtual void calculate();
};

#endif // CAD_HEATCOOL_PIPE_H
