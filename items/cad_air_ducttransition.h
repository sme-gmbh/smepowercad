#ifndef CAD_AIR_DUCTTRANSITION_H
#define CAD_AIR_DUCTTRANSITION_H

#include "caditem.h"
#include "cad_basic_duct.h"

class CAD_air_ductTransition : public CADitem
{
public:
    CAD_air_ductTransition();
    virtual ~CAD_air_ductTransition();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

private:
    qreal s;
    qreal endcap;
    qreal e, f; //offset dy and dz
    qreal ff, fe;
    qreal a, b, a2, b2, l;

    CAD_basic_duct *transition_duct;
    CAD_basic_duct *endcap_left_duct;
    CAD_basic_duct *endcap_right_duct;
    CAD_basic_duct *flange_left_duct;
    CAD_basic_duct *flange_right_duct;

};

#endif // CAD_AIR_DUCTTRANSITION_H
