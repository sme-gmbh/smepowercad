#include "cad_air_heatexchangerairair.h"

CAD_air_heatExchangerAirAir::CAD_air_heatExchangerAirAir() : CADitem(CADitem::Air_HeatExchangerAirAir)
{
    this->description = "Air|Heat exchanger air/air";

}

QList<CADitem::ItemType> CAD_air_heatExchangerAirAir::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_heatExchangerAirAir::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    qDebug() << imageFileName;

    image.load(imageFileName, "PNG");

    return image;
}

void CAD_air_heatExchangerAirAir::calculate()
{

}

void CAD_air_heatExchangerAirAir::processWizardInput()
{

}
