#ifndef CAD_ARCH_BEAM_H
#define CAD_ARCH_BEAM_H

#include "caditem.h"
#include "cad_basic_box.h"

class CAD_arch_beam : public CADitem
{
public:
    CAD_arch_beam();
    virtual ~CAD_arch_beam();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

    qreal a,b,l;

    CAD_basic_box *beam;
};

#endif // CAD_ARCH_BEAM_H
