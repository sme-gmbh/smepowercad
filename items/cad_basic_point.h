#ifndef CAD_BASIC_POINT_H
#define CAD_BASIC_POINT_H

#include "caditem.h"

class CAD_basic_point : public CADitem
{
public:
    CAD_basic_point();
    virtual ~CAD_basic_point();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
    virtual void paint(GLWidget* glwidget);
};

#endif // CAD_BASIC_POINT_H
