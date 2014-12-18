#ifndef CAD_AIR_DUCTTEECONNECTOR_H
#define CAD_AIR_DUCTTEECONNECTOR_H

#include "caditem.h"
#include "cad_basic_duct.h"

class CAD_air_ductTeeConnector : public CADitem
{
public:
    CAD_air_ductTeeConnector();
    virtual ~CAD_air_ductTeeConnector();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

    qreal a;    //height of duct
    qreal b;    //width of first outlet
    qreal b2;    //width of second outlet
    qreal e;    //offset of second outlet in y
    qreal b3;    //width of third outlet
    qreal l;    //length from first outlet to second outlet
    qreal m;    //offset of second outlet to third outlet in y
    qreal n;    //offset of third outlet to first outlet in x
    qreal r1;   //radius of "left" turn
    qreal r2;   //radius of "right" turn
    qreal u;    //length of endcap
    qreal s;
    qreal ff,fe;

    CAD_basic_duct *endcap_1;
    CAD_basic_duct *endcap_2;
    CAD_basic_duct *endcap_3;

    CAD_basic_duct *flange_1;
    CAD_basic_duct *flange_2;
    CAD_basic_duct *flange_3;

    QVector3D vertices_turn1[2][12][2];
    QVector3D vertices_turn2[2][12][2];

    QVector3D vertices_backside[4];
};

#endif // CAD_AIR_DUCTTEECONNECTOR_H
