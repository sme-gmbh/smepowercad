#ifndef CAD_ARCH_WALL_LOADBEARING_H
#define CAD_ARCH_WALL_LOADBEARING_H

#include "caditem.h"
#include "cad_basic_box.h"

class CAD_arch_wall_loadBearing : public CADitem
{
public:
    CAD_arch_wall_loadBearing();
    virtual ~CAD_arch_wall_loadBearing();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

private:
    qreal b;
    qreal l;
    qreal a;

    CAD_basic_box* basic_box;
};

#endif // CAD_ARCH_WALL_LOADBEARING_H
