#include "cad_heatcool_waterheater.h"

CAD_heatcool_waterHeater::CAD_heatcool_waterHeater() : CADitem(CADitem::HeatCool_WaterHeater)
{
    this->description = "Heat/Cool|Water heater";

}

QList<CADitem::ItemType> CAD_heatcool_waterHeater::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_waterHeater::wizardImage()
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

void CAD_heatcool_waterHeater::calculate()
{

}

void CAD_heatcool_waterHeater::processWizardInput()
{

}
