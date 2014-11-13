#ifndef CAD_ARCH_WINDOW_H
#define CAD_ARCH_WINDOW_H

#include "caditem.h"

class CAD_arch_window : public CADitem
{
public:
    CAD_arch_window();
    virtual ~CAD_arch_window();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_ARCH_WINDOW_H
