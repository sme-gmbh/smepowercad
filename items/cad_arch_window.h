#ifndef CAD_ARCH_WINDOW_H
#define CAD_ARCH_WINDOW_H

#include "caditem.h"

class CAD_arch_window : public CADitem
{
public:
    CAD_arch_window();
    virtual ~CAD_arch_window();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_ARCH_WINDOW_H
