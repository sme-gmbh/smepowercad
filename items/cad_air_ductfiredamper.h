#ifndef CAD_AIR_DUCTFIREDAMPER_H
#define CAD_AIR_DUCTFIREDAMPER_H

#include "caditem.h"
#include "cad_basic_duct.h"
#include "cad_basic_box.h"

class CAD_air_ductFireDamper : public CADitem
{
public:
    CAD_air_ductFireDamper();
    virtual ~CAD_air_ductFireDamper();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

    qreal a, h, b, g;
    qreal ff, fe;
    qreal l, l2, l3;
    qreal s;

    CAD_basic_duct *main_duct, *flange_duct_left, *flange_duct_right;
    CAD_basic_box *function, *flange_duct_middle;
};

#endif // CAD_AIR_DUCTFIREDAMPER_H
