#ifndef CAD_AIR_DUCTFIRERESISTANT_H
#define CAD_AIR_DUCTFIRERESISTANT_H

#include "caditem.h"
#include "cad_air_duct.h"

class CAD_air_ductFireResistant : public CADitem
{
public:
    CAD_air_ductFireResistant();
    virtual ~CAD_air_ductFireResistant();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

    qreal a,b,l, fe, ff, s, iso;
    CAD_air_duct* duct;


};

#endif // CAD_AIR_DUCTFIRERESISTANT_H
