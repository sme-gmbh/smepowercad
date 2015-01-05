#ifndef CAD_AIR_PIPEREDUCER_H
#define CAD_AIR_PIPEREDUCER_H

#include "caditem.h"
#include "cad_basic_pipe.h"

class CAD_air_pipeReducer : public CADitem
{
public:
    CAD_air_pipeReducer();
    virtual ~CAD_air_pipeReducer();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual void calculate();
    virtual void processWizardInput();

    qreal d, d2;
    qreal s, l;

    CAD_basic_pipe *reducer;
};

#endif // CAD_AIR_PIPEREDUCER_H
