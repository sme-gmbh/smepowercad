#ifndef CAD_AIR_CANVASFLANGE_H
#define CAD_AIR_CANVASFLANGE_H

#include "caditem.h"
#include "cad_basic_duct.h"

class CAD_air_canvasFlange : public CADitem
{
public:
    CAD_air_canvasFlange();
    virtual ~CAD_air_canvasFlange();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

    qreal a, b, l;
    qreal fe, ff;
    qreal s;

    CAD_basic_duct* flange_left;
    CAD_basic_duct* flange_right;
    CAD_basic_duct* canvas1;
    CAD_basic_duct* canvas2;
    CAD_basic_duct* canvas3;
};

#endif // CAD_AIR_CANVASFLANGE_H
