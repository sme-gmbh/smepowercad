#ifndef CAD_ARCH_BLOCKOUT_H
#define CAD_ARCH_BLOCKOUT_H

#include "caditem.h"
#include "cad_basic_box.h"

class CAD_arch_blockOut : public CADitem
{
public:
    CAD_arch_blockOut();
    virtual ~CAD_arch_blockOut();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

    qreal g, h, b;
    CAD_basic_box *blockout;
};

#endif // CAD_ARCH_BLOCKOUT_H
