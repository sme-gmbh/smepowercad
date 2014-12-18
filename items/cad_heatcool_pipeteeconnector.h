#ifndef CAD_HEATCOOL_PIPETEECONNECTOR_H
#define CAD_HEATCOOL_PIPETEECONNECTOR_H

#include "caditem.h"

class CAD_heatcool_pipeTeeConnector : public CADitem
{
public:
    CAD_heatcool_pipeTeeConnector();
    virtual ~CAD_heatcool_pipeTeeConnector();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_PIPETEECONNECTOR_H
