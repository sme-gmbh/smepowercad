#ifndef CAD_AIR_DUCTRECT_H
#define CAD_AIR_DUCTRECT_H

#include "caditem.h"
#include "cad_basic_duct.h"

class CAD_air_duct : public CADitem
{
public:
    CAD_air_duct();
    virtual ~CAD_air_duct();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

    qreal length;
    qreal width;
    qreal height;
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
    qreal flange_size;

    CAD_basic_duct *main_duct, *flange_duct_left, *flange_duct_right;
};

#endif // CAD_AIR_DUCTRECT_H
