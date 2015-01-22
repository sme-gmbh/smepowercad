#ifndef CAD_ARCH_LEVELSLAB_H
#define CAD_ARCH_LEVELSLAB_H

#include "caditem.h"
#include "cad_basic_box.h"

class CAD_arch_levelSlab : public CADitem
{
public:
    CAD_arch_levelSlab();
    virtual ~CAD_arch_levelSlab();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

    qreal a,b,l;
    CAD_basic_box *slab;
};

#endif // CAD_ARCH_LEVELSLAB_H
