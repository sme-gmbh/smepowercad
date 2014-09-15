#ifndef CAD_BASIC_CYLINDER_H
#define CAD_BASIC_CYLINDER_H

#include "caditem.h"

class CAD_basic_cylinder : public CADitem
{
public:
    CAD_basic_cylinder();
    virtual void calculate();
    virtual void processWizardInput();

    qreal radius;
    QVector3D center_base;
    qreal height;
    qreal angle_x;
    qreal angle_y;
    qreal angle_z;
};

#endif // CADCYLINDER_H
