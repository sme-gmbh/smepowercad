#ifndef CAD_AIR_PIPETEECONNECTOR_H
#define CAD_AIR_PIPETEECONNECTOR_H

#include "caditem.h"

class CAD_air_pipeTeeConnector : public CADitem
{
public:
    CAD_air_pipeTeeConnector();
    virtual ~CAD_air_pipeTeeConnector();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_PIPETEECONNECTOR_H
