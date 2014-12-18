#ifndef CAD_SPRINKLER_PIPE_H
#define CAD_SPRINKLER_PIPE_H

#include "caditem.h"

class CAD_sprinkler_pipe : public CADitem
{
public:
    CAD_sprinkler_pipe();
    virtual ~CAD_sprinkler_pipe();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SPRINKLER_PIPE_H
