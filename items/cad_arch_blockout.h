#ifndef CAD_ARCH_BLOCKOUT_H
#define CAD_ARCH_BLOCKOUT_H

#include "caditem.h"
#include "cad_basic_box.h"

class CAD_arch_blockOut : public CADitem
{
public:
    CAD_arch_blockOut();
    virtual ~CAD_arch_blockOut();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

    qreal g, h, b;
    CAD_basic_box *blockout;
};

#endif // CAD_ARCH_BLOCKOUT_H
