#ifndef CAD_BASIC_CYLINDER_H
#define CAD_BASIC_CYLINDER_H

#include "caditem.h"

class CAD_basic_cylinder : public CADitem
{
public:
    CAD_basic_cylinder();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

    qreal radius;
    QVector3D center_base;
    qreal height;
};

#endif // CADCYLINDER_H
