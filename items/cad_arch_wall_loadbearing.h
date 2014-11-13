#ifndef CAD_ARCH_WALL_LOADBEARING_H
#define CAD_ARCH_WALL_LOADBEARING_H

#include "caditem.h"
#include "cad_basic_box.h"

class CAD_arch_wall_loadBearing : public CADitem
{
public:
    CAD_arch_wall_loadBearing();
    virtual ~CAD_arch_wall_loadBearing();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

private:
    qreal wall_thickness;
    qreal length;
    qreal height;

    CAD_basic_box* basic_box;
};

#endif // CAD_ARCH_WALL_LOADBEARING_H
