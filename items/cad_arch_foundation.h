#ifndef CAD_ARCH_FOUNDATION_H
#define CAD_ARCH_FOUNDATION_H

#include "caditem.h"

class CAD_arch_foundation : public CADitem
{
public:
    CAD_arch_foundation();
    virtual ~CAD_arch_foundation();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_ARCH_FOUNDATION_H
