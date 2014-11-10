#ifndef CAD_SANITARY_FLANGE_H
#define CAD_SANITARY_FLANGE_H

#include "caditem.h"

class CAD_sanitary_flange : public CADitem
{
public:
    CAD_sanitary_flange();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SANITARY_FLANGE_H
