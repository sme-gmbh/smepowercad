#ifndef CAD_HEATCOOL_FILTER_H
#define CAD_HEATCOOL_FILTER_H

#include "caditem.h"

class CAD_heatcool_filter : public CADitem
{
public:
    CAD_heatcool_filter();
    virtual void calculate();
};

#endif // CAD_HEATCOOL_FILTER_H
