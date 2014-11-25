#ifndef CAD_AIR_DUCTYPIECE_H
#define CAD_AIR_DUCTYPIECE_H

#include "caditem.h"
#include "cad_basic_duct.h"

class CAD_air_ductYpiece : public CADitem
{
public:
    CAD_air_ductYpiece();
    virtual ~CAD_air_ductYpiece();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

    qreal a; //height 1
    qreal b; //width 1
    qreal c; //height 2
    qreal d; //width 2
    qreal e; //offset y 1
    qreal f; //offset z 1
    qreal h; //width 3
    qreal l; //length;
    qreal m; //offset y 2
    qreal u; //endcap
    qreal wall_thickness;
    qreal flange_size;

    CAD_basic_duct *endcap_1;
    CAD_basic_duct *endcap_2;
    CAD_basic_duct *endcap_3;
    CAD_basic_duct *flange_1;
    CAD_basic_duct *flange_2;
    CAD_basic_duct *flange_3;

    QVector3D splitPoint[2];

};

#endif // CAD_AIR_DUCTYPIECE_H
