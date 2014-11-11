#ifndef CAD_AIR_DUCTTRANSITION_H
#define CAD_AIR_DUCTTRANSITION_H

#include "caditem.h"
#include "cad_basic_duct.h"

class CAD_air_ductTransition : public CADitem
{
public:
    CAD_air_ductTransition();
    virtual ~CAD_air_ductTransition();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

private:
    QVector3D pos_bot_1;
    QVector3D pos_bot_2;
    QVector3D pos_bot_3;
    QVector3D pos_bot_4;
    QVector3D pos_top_1;
    QVector3D pos_top_2;
    QVector3D pos_top_3;
    QVector3D pos_top_4;

    qreal wall_thickness;
    qreal endcap;
    qreal e, f; //offset dy and dz
    qreal flange_size;
    qreal a, b, c, d, l;

    CAD_basic_duct *transition_duct;
    CAD_basic_duct *endcap_left_duct;
    CAD_basic_duct *endcap_right_duct;
    CAD_basic_duct *flange_left_duct;
    CAD_basic_duct *flange_right_duct;

};

#endif // CAD_AIR_DUCTTRANSITION_H
