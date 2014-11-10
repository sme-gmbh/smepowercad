#ifndef CAD_BASIC_POINT_H
#define CAD_BASIC_POINT_H

#include "caditem.h"

class CAD_basic_point : public CADitem
{
public:
    CAD_basic_point();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_BASIC_POINT_H
