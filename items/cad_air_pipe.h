#ifndef CAD_AIR_DUCTROUND_H
#define CAD_AIR_DUCTROUND_H

#include "caditem.h"
#include "cad_basic_pipe.h"

class CAD_air_pipe : public CADitem
{
public:
    CAD_air_pipe();
    virtual ~CAD_air_pipe();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
    qreal d;
    qreal s;
    qreal l;

    CAD_basic_pipe *pipe;
};

#endif // CAD_AIR_DUCTROUND_H
