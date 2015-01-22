#ifndef CAD_AIR_PIPETURN_H
#define CAD_AIR_PIPETURN_H

#include "caditem.h"
#include "cad_basic_turn.h"

class CAD_air_pipeTurn : public CADitem
{
public:
    CAD_air_pipeTurn();
    virtual ~CAD_air_pipeTurn();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

    qreal alpha, r;
    qreal d, s;
    CAD_basic_turn* turn;
};

#endif // CAD_AIR_PIPETURN_H
