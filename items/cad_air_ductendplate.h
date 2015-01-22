#ifndef CAD_AIR_DUCTENDPLATE_H
#define CAD_AIR_DUCTENDPLATE_H

#include "caditem.h"
#include "cad_basic_duct.h"
#include "cad_basic_box.h"

class CAD_air_ductEndPlate : public CADitem
{
public:
    CAD_air_ductEndPlate();
    virtual ~CAD_air_ductEndPlate();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

    qreal a; //height
    qreal b; //width
    qreal l; //length
    qreal ff, fe;
    qreal s;

    CAD_basic_duct *duct;
    CAD_basic_duct *flange;
    CAD_basic_box *plate;

};

#endif // CAD_AIR_DUCTENDPLATE_H
