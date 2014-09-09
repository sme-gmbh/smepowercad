#ifndef CAD_HEATCOOL_STORAGEBOILER_H
#define CAD_HEATCOOL_STORAGEBOILER_H

#include "caditem.h"

class CAD_heatcool_storageBoiler : public CADitem
{
public:
    CAD_heatcool_storageBoiler();
    virtual void calculate();
};

#endif // CAD_HEATCOOL_STORAGEBOILER_H
