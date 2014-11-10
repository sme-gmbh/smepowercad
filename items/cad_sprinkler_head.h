#ifndef CAD_SPRINKLER_HEAD_H
#define CAD_SPRINKLER_HEAD_H

#include "caditem.h"

class CAD_sprinkler_head : public CADitem
{
public:
    CAD_sprinkler_head();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SPRINKLER_HEAD_H
