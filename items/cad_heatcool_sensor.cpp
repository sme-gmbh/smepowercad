#include "cad_heatcool_sensor.h"

CAD_heatcool_sensor::CAD_heatcool_sensor() : CADitem(CADitem::HeatCool_Sensor)
{
    this->description = "Heat/Cool|Sensor";

}

QList<CADitem::ItemType> CAD_heatcool_sensor::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_sensor::wizardImage()
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

void CAD_heatcool_sensor::calculate()
{

}

void CAD_heatcool_sensor::processWizardInput()
{

}
