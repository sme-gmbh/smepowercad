#ifndef CAD_HEATCOOL_PIPETEECONNECTOR_H
#define CAD_HEATCOOL_PIPETEECONNECTOR_H

#include "caditem.h"

class CAD_heatcool_pipeTeeConnector : public CADitem
{
public:
    CAD_heatcool_pipeTeeConnector();
    virtual void calculate();
};

#endif // CAD_HEATCOOL_PIPETEECONNECTOR_H
