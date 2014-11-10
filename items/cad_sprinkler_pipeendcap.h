#ifndef CAD_SPRINKLER_PIPEENDCAP_H
#define CAD_SPRINKLER_PIPEENDCAP_H

#include "caditem.h"

class CAD_sprinkler_pipeEndCap : public CADitem
{
public:
    CAD_sprinkler_pipeEndCap();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SPRINKLER_PIPEENDCAP_H
