#ifndef CAD_ARCH_GRATING_H
#define CAD_ARCH_GRATING_H

#include "caditem.h"

class CAD_arch_grating : public CADitem
{
public:
    CAD_arch_grating();
    virtual ~CAD_arch_grating();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_ARCH_GRATING_H
