#ifndef CAD_AIR_DUCTRECT_H
#define CAD_AIR_DUCTRECT_H

#include "caditem.h"
#include "cad_basic_duct.h"

class CAD_air_duct : public CADitem
{
public:
    CAD_air_duct();
    virtual ~CAD_air_duct();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

    qreal l;
    qreal b;
    qreal a;
    QVector3D pos_bot_1;
    QVector3D pos_bot_2;
    QVector3D pos_bot_3;
    QVector3D pos_bot_4;
    QVector3D pos_top_1;
    QVector3D pos_top_2;
    QVector3D pos_top_3;
    QVector3D pos_top_4;

//    QVector3D size;

    qreal s;
    qreal ff, fe;

    CAD_basic_duct *main_duct, *flange_duct_left, *flange_duct_right;
};

#endif // CAD_AIR_DUCTRECT_H
