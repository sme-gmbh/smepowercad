#ifndef CAD_HEATCOOL_FILTER_H
#define CAD_HEATCOOL_FILTER_H

#include "caditem.h"

class CAD_heatcool_filter : public CADitem
{
public:
    CAD_heatcool_filter();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_FILTER_H
