#ifndef CAD_AIR_HEATEXCHANGERAIRAIR_H
#define CAD_AIR_HEATEXCHANGERAIRAIR_H

#include "caditem.h"
#include "cad_air_duct.h"
#include "cad_basic_plane.h"

class CAD_air_heatExchangerAirAir : public CADitem
{
public:
    CAD_air_heatExchangerAirAir();
    virtual ~CAD_air_heatExchangerAirAir();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual void calculate();
    virtual void processWizardInput();
    virtual void paint(GLWidget* glwidget);

    qreal ff, fe, s;
    qreal l, a, b, i;

    CAD_air_duct *duct;
    CAD_basic_plane *seperator_1, *seperator_2;
    QVector3D points[2][4];
};

#endif // CAD_AIR_HEATEXCHANGERAIRAIR_H
