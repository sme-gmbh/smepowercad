#ifndef CAD_HEATCOOL_PIPE_H
#define CAD_HEATCOOL_PIPE_H

#include "caditem.h"

class CAD_heatcool_pipe : public CADitem
{
public:
    CAD_heatcool_pipe();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_PIPE_H
