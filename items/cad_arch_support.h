#ifndef CAD_ARCH_SUPPORT_H
#define CAD_ARCH_SUPPORT_H

#include "caditem.h"

class CAD_arch_support : public CADitem
{
public:
    CAD_arch_support();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_ARCH_SUPPORT_H
