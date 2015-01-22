#ifndef CAD_SANITARY_PIPE_H
#define CAD_SANITARY_PIPE_H

#include "caditem.h"

class CAD_sanitary_pipe : public CADitem
{
public:
    CAD_sanitary_pipe();
    virtual ~CAD_sanitary_pipe();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SANITARY_PIPE_H
