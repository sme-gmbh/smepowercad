#ifndef CAD_SANITARY_PIPE_H
#define CAD_SANITARY_PIPE_H

#include "caditem.h"

class CAD_sanitary_pipe : public CADitem
{
public:
    CAD_sanitary_pipe();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SANITARY_PIPE_H
