#ifndef CAD_HEATCOOL_PIPE_H
#define CAD_HEATCOOL_PIPE_H

#include "caditem.h"

class CAD_heatcool_pipe : public CADitem
{
public:
    CAD_heatcool_pipe();
    virtual ~CAD_heatcool_pipe();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_PIPE_H
