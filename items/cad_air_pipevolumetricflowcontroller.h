#ifndef CAD_AIR_PIPEVOLUMETRICFLOWCONTROLLER_H
#define CAD_AIR_PIPEVOLUMETRICFLOWCONTROLLER_H

#include "caditem.h"
#include "cad_basic_pipe.h"
#include "cad_basic_box.h"

class CAD_air_pipeVolumetricFlowController : public CADitem
{
public:
    CAD_air_pipeVolumetricFlowController();
    virtual ~CAD_air_pipeVolumetricFlowController();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

    qreal l, s, d;
    CAD_basic_pipe* mainPipe, flangePipe_left, flangePipe_right;
    CAD_basic_box* function;
    CAD_basic_box* handle;
    CAD_basic_pipe* controller;



};

#endif // CAD_AIR_PIPEVOLUMETRICFLOWCONTROLLER_H
