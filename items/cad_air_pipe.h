#ifndef CAD_AIR_DUCTROUND_H
#define CAD_AIR_DUCTROUND_H

#include "caditem.h"

class CAD_air_pipe : public CADitem
{
public:
    CAD_air_pipe();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_DUCTROUND_H
