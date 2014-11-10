#include "cad_heatcool_pump.h"

CAD_heatcool_pump::CAD_heatcool_pump() : CADitem(CADitem::HeatCool_Pump)
{
    this->description = "Heat/Cool|Pump";

}

QList<CADitem::ItemType> CAD_heatcool_pump::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_pump::wizardImage()
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

void CAD_heatcool_pump::calculate()
{

}

void CAD_heatcool_pump::processWizardInput()
{

}
