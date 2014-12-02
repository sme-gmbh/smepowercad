#ifndef CAD_BASIC_DUCT_H
#define CAD_BASIC_DUCT_H

#include "caditem.h"

class CAD_basic_duct : public CADitem
{
public:
    CAD_basic_duct();
    virtual ~CAD_basic_duct();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

    QVector3D pos_bot_1;
    QVector3D pos_bot_2;
    QVector3D pos_bot_3;
    QVector3D pos_bot_4;
    QVector3D pos_top_1;
    QVector3D pos_top_2;
    QVector3D pos_top_3;
    QVector3D pos_top_4;

    QVector3D inner_pos_bot_1;
    QVector3D inner_pos_bot_2;
    QVector3D inner_pos_bot_3;
    QVector3D inner_pos_bot_4;
    QVector3D inner_pos_top_1;
    QVector3D inner_pos_top_2;
    QVector3D inner_pos_top_3;
    QVector3D inner_pos_top_4;

    QVector3D size;

    qreal s;
};

#endif // CAD_BASIC_DUCT_H
