#ifndef CAD_SPRINKLER_PIPE_H
#define CAD_SPRINKLER_PIPE_H

#include "caditem.h"

class CAD_sprinkler_pipe : public CADitem
{
public:
    CAD_sprinkler_pipe();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SPRINKLER_PIPE_H
