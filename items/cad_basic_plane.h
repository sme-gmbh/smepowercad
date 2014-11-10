#ifndef CAD_BASIC_PLANE_H
#define CAD_BASIC_PLANE_H

#include "caditem.h"

class CAD_basic_plane : public CADitem
{
public:
    CAD_basic_plane();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_BASIC_PLANE_H
