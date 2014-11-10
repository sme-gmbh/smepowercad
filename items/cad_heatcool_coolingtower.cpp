#include "cad_heatcool_coolingtower.h"

CAD_heatcool_coolingTower::CAD_heatcool_coolingTower() : CADitem(CADitem::HeatCool_CoolingTower)
{
    this->description = "Heat/Cool|Coolingtower";
}

QList<CADitem::ItemType> CAD_heatcool_coolingTower::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_coolingTower::wizardImage()
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

void CAD_heatcool_coolingTower::calculate()
{

}

void CAD_heatcool_coolingTower::processWizardInput()
{

}
