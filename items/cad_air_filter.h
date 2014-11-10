#ifndef CAD_AIR_FILTER_H
#define CAD_AIR_FILTER_H

#include "caditem.h"

class CAD_air_filter : public CADitem
{
public:
    CAD_air_filter();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_FILTER_H
