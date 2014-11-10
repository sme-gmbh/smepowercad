#ifndef CAD_AIR_DUCTTURN_H
#define CAD_AIR_DUCTTURN_H

#include "caditem.h"

class CAD_air_ductTurn : public CADitem
{
public:
    CAD_air_ductTurn();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

    qreal wall_thickness;
    qreal flange_size;

    qreal radius;
    qreal width_1;
    qreal width_2;
    qreal height;
    qreal angle;
    qreal endcap_1;
    qreal endcap_2;

};

#endif // CAD_AIR_DUCTTURN_H
