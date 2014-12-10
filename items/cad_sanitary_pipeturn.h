#ifndef CAD_SANITARY_PIPETURN_H
#define CAD_SANITARY_PIPETURN_H

#include "caditem.h"

class CAD_sanitary_pipeTurn : public CADitem
{
public:
    CAD_sanitary_pipeTurn();
    virtual ~CAD_sanitary_pipeTurn();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SANITARY_PIPETURN_H