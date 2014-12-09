#ifndef CAD_AIR_PIPEBRANCH_H
#define CAD_AIR_PIPEBRANCH_H

#include "caditem.h"
#include "cad_basic_pipe.h"

class CAD_air_pipeBranch : public CADitem
{
public:
    CAD_air_pipeBranch();
    virtual ~CAD_air_pipeBranch();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

    qreal d, d2;
    qreal s;
    qreal l, l1 , l2;
    qreal alpha;

    CAD_basic_pipe *pipe, *branch;

};

#endif // CAD_AIR_PIPEBRANCH_H
