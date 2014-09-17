#ifndef CAD_BASIC_BOX_H
#define CAD_BASIC_BOX_H

#include "caditem.h"

class CAD_basic_box : public CADitem
{
public:
    CAD_basic_box();
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
};

#endif // CAD_BASIC_BOX_H
