#ifndef CAD_HEATCOOL_PIPEREDUCER_H
#define CAD_HEATCOOL_PIPEREDUCER_H

#include "caditem.h"

class CAD_heatcool_pipeReducer : public CADitem
{
public:
    CAD_heatcool_pipeReducer();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_PIPEREDUCER_H
