#ifndef CAD_AIR_PIPESILENCER_H
#define CAD_AIR_PIPESILENCER_H

#include "caditem.h"
#include "cad_basic_pipe.h"

class CAD_air_pipeSilencer : public CADitem
{
public:
    CAD_air_pipeSilencer();
    virtual ~CAD_air_pipeSilencer();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

    qreal d;
    qreal D;
    qreal s;
    qreal l;

    CAD_basic_pipe *pipe;

};

#endif // CAD_AIR_PIPESILENCER_H
