#ifndef CAD_AIR_DUCTBAFFLESILENCER_H
#define CAD_AIR_DUCTBAFFLESILENCER_H

#include "caditem.h"
#include "cad_basic_duct.h"
#include "cad_basic_box.h"
#include <QList>

class CAD_air_ductBaffleSilencer : public CADitem
{
public:
    CAD_air_ductBaffleSilencer();
    virtual ~CAD_air_ductBaffleSilencer();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();

    qreal a, b, l;
    qreal d, n;
    qreal ff, fe;
    qreal s;

    CAD_basic_duct* flange_duct_left;
    CAD_basic_duct* flange_duct_right;
    CAD_basic_duct* main_duct;

    QList<CAD_basic_box*> silencers;



};

#endif // CAD_AIR_DUCTBAFFLESILENCER_H
