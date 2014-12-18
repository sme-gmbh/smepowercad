#ifndef CAD_AIR_PIPEFIREDAMPER_H
#define CAD_AIR_PIPEFIREDAMPER_H

#include "caditem.h"
#include "cad_basic_pipe.h"
#include "cad_basic_box.h"

class CAD_air_pipeFireDamper : public CADitem
{
public:
    CAD_air_pipeFireDamper();
    virtual ~CAD_air_pipeFireDamper();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

    qreal s, ff;
    qreal d, l, l2;

    CAD_basic_pipe *main_pipe;
    CAD_basic_box *function;
};

#endif // CAD_AIR_PIPEFIREDAMPER_H
