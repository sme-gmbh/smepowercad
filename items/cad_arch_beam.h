#ifndef CAD_ARCH_BEAM_H
#define CAD_ARCH_BEAM_H

#include "caditem.h"

class CAD_arch_beam : public CADitem
{
public:
    CAD_arch_beam();
    virtual ~CAD_arch_beam();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_ARCH_BEAM_H
