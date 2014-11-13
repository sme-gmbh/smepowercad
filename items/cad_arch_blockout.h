#ifndef CAD_ARCH_BLOCKOUT_H
#define CAD_ARCH_BLOCKOUT_H

#include "caditem.h"

class CAD_arch_blockOut : public CADitem
{
public:
    CAD_arch_blockOut();
    virtual ~CAD_arch_blockOut();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_ARCH_BLOCKOUT_H
