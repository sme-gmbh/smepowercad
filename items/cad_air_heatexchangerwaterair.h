#ifndef CAD_AIR_HEATEXCHANGERWATERAIR_H
#define CAD_AIR_HEATEXCHANGERWATERAIR_H

#include "caditem.h"
#include "cad_air_duct.h"
#include "cad_basic_box.h"
#include "cad_basic_pipe.h"

class CAD_air_heatExchangerWaterAir : public CADitem
{
public:
    CAD_air_heatExchangerWaterAir();
    virtual ~CAD_air_heatExchangerWaterAir();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

    qreal s, ff, fe;
    qreal l, l3, l4;
    qreal d, j;
    qreal m3, m4;
    qreal a, b;

    CAD_air_duct *duct;
    CAD_basic_pipe *pipe3, *pipe4;
    CAD_basic_box *exchanger;
};

#endif // CAD_AIR_HEATEXCHANGERWATERAIR_H
