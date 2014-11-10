#include "cad_heatcool_heatexchanger.h"

CAD_heatcool_heatExchanger::CAD_heatcool_heatExchanger() : CADitem(CADitem::HeatCool_HeatExchanger)
{
    this->description = "Heat/Cool|Heatexchanger";

}

QList<CADitem::ItemType> CAD_heatcool_heatExchanger::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_heatExchanger::wizardImage()
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

void CAD_heatcool_heatExchanger::calculate()
{

}

void CAD_heatcool_heatExchanger::processWizardInput()
{

}
