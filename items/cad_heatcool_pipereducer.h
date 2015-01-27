#ifndef CAD_HEATCOOL_PIPEREDUCER_H
#define CAD_HEATCOOL_PIPEREDUCER_H

#include "caditem.h"
#include "cad_basic_pipe.h"

class CAD_heatcool_pipeReducer : public CADitem
{
public:
    CAD_heatcool_pipeReducer();
    virtual ~CAD_heatcool_pipeReducer();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

    CAD_basic_pipe* reducer;
    CAD_basic_pipe* endcap_bottom;
    CAD_basic_pipe* endcap_top;
    qreal l, l1, l2;
    qreal d,d_o, d2,d_o2;
    qreal f, s;

};

#endif // CAD_HEATCOOL_PIPEREDUCER_H
