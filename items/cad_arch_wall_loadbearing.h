#ifndef CAD_ARCH_WALL_LOADBEARING_H
#define CAD_ARCH_WALL_LOADBEARING_H

#include "caditem.h"

class CAD_arch_wall_loadBearing : public CADitem
{
public:
    CAD_arch_wall_loadBearing();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_ARCH_WALL_LOADBEARING_H
