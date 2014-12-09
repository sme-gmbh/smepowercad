#ifndef CAD_AIR_DUCTTRANSITIONRECTROUND_H
#define CAD_AIR_DUCTTRANSITIONRECTROUND_H

#include "caditem.h"
#include "cad_basic_duct.h"
#include "cad_basic_pipe.h"

class CAD_air_ductTransitionRectRound : public CADitem
{
public:
    CAD_air_ductTransitionRectRound();
    virtual ~CAD_air_ductTransitionRectRound();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

    qreal a; //height of rect side
    qreal b; //width of rect side
    qreal d; //diameter of round size
    qreal e; //offset y
    qreal f; //offset z
    qreal l; //length of transition
    qreal u; //endcap length

    qreal ff, fe;
    qreal s;

    QVector3D roundside[2][2][32]; //front <-> back, inner <-> outer, discrete circle
    QVector3D rectside[2][2][4]; //front <-> back, inner <-> outer, "discrete circle"

    CAD_basic_pipe *flange_round;
    CAD_basic_duct *flange_rect;
};

#endif // CAD_AIR_DUCTTRANSITIONRECTROUND_H
