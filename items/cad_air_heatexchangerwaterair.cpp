#include "cad_air_heatexchangerwaterair.h"

CAD_air_heatExchangerWaterAir::CAD_air_heatExchangerWaterAir() : CADitem(CADitem::Air_HeatExchangerWaterAir)
{
    this->description = "Air|Heat exchanger water/air";

}

QList<CADitem::ItemType> CAD_air_heatExchangerWaterAir::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_heatExchangerWaterAir::wizardImage()
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

void CAD_air_heatExchangerWaterAir::calculate()
{

}

void CAD_air_heatExchangerWaterAir::processWizardInput()
{

}
