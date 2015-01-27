#ifndef CAD_HEATCOOL_PIPETURN_H
#define CAD_HEATCOOL_PIPETURN_H

#include "caditem.h"
#include "cad_basic_pipe.h"
#include "cad_basic_turn.h"

class CAD_heatcool_pipeTurn : public CADitem
{
public:
    CAD_heatcool_pipeTurn();
    virtual ~CAD_heatcool_pipeTurn();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

    qreal d, d_o, s, l, b, alpha, r;
    CAD_basic_pipe* endcap_1;
    CAD_basic_pipe* endcap_2;
    CAD_basic_turn* turn;

};

#endif // CAD_HEATCOOL_PIPETURN_H
