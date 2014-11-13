#ifndef CAD_SANITARY_SINK_H
#define CAD_SANITARY_SINK_H

#include "caditem.h"

class CAD_sanitary_sink : public CADitem
{
public:
    CAD_sanitary_sink();
    virtual ~CAD_sanitary_sink();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SANITARY_SINK_H
