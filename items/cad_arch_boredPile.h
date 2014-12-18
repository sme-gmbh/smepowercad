#ifndef CAD_ARCH_BOREDPILE_H
#define CAD_ARCH_BOREDPILE_H

#include "caditem.h"
#include "cad_basic_pipe.h"

class CAD_arch_boredPile : public CADitem
{
public:
    CAD_arch_boredPile();
    virtual ~CAD_arch_boredPile();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

    qreal a, d;
    CAD_basic_pipe *pile;
};

#endif // CAD_ARCH_BOREDPILE_H
