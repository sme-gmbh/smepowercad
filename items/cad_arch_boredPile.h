#ifndef CAD_ARCH_BOREDPILE_H
#define CAD_ARCH_BOREDPILE_H

#include "caditem.h"

class CAD_arch_boredPile : public CADitem
{
public:
    CAD_arch_boredPile();
    virtual ~CAD_arch_boredPile();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_ARCH_BOREDPILE_H
