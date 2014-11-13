#ifndef CAD_SPRINKLER_PIPEREDUCER_H
#define CAD_SPRINKLER_PIPEREDUCER_H

#include "caditem.h"

class CAD_sprinkler_pipeReducer : public CADitem
{
public:
    CAD_sprinkler_pipeReducer();
    virtual ~CAD_sprinkler_pipeReducer();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SPRINKLER_PIPEREDUCER_H
