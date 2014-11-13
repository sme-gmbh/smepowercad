#ifndef CAD_ARCH_WALL_NONLOADBEARING_H
#define CAD_ARCH_WALL_NONLOADBEARING_H

#include "caditem.h"

class CAD_arch_wall_nonLoadBearing : public CADitem
{
public:
    CAD_arch_wall_nonLoadBearing();
    virtual ~CAD_arch_wall_nonLoadBearing();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_ARCH_WALL_NONLOADBEARING_H
