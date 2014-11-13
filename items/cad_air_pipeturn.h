#ifndef CAD_AIR_PIPETURN_H
#define CAD_AIR_PIPETURN_H

#include "caditem.h"

class CAD_air_pipeTurn : public CADitem
{
public:
    CAD_air_pipeTurn();
    virtual ~CAD_air_pipeTurn();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_PIPETURN_H
