#ifndef CAD_ELECTRICAL_CABLETRAY_H
#define CAD_ELECTRICAL_CABLETRAY_H

#include "caditem.h"

class CAD_electrical_cableTray : public CADitem
{
public:
    CAD_electrical_cableTray();
    virtual void calculate();
};

#endif // CAD_ELECTRICAL_CABLETRAY_H
