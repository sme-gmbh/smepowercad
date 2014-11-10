#include "cad_heatcool_boiler.h"

CAD_heatcool_boiler::CAD_heatcool_boiler() : CADitem(CADitem::HeatCool_Boiler)
{
    this->description = "Heat/Cool|Boiler";

}

QList<CADitem::ItemType> CAD_heatcool_boiler::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_boiler::wizardImage()
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

void CAD_heatcool_boiler::calculate()
{

}

void CAD_heatcool_boiler::processWizardInput()
{

}
