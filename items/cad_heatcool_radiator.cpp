#include "cad_heatcool_radiator.h"

CAD_heatcool_radiator::CAD_heatcool_radiator() : CADitem(CADitem::HeatCool_Radiator)
{
    this->description = "Heat/Cool|Radiator";

}

QList<CADitem::ItemType> CAD_heatcool_radiator::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_radiator::wizardImage()
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

void CAD_heatcool_radiator::calculate()
{

}

void CAD_heatcool_radiator::processWizardInput()
{

}
