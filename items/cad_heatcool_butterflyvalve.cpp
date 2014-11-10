#include "cad_heatcool_butterflyvalve.h"

CAD_heatcool_butterflyValve::CAD_heatcool_butterflyValve() : CADitem(CADitem::HeatCool_ButterflyValve)
{
    this->description = "Heat/Cool|Butterflyvalve";

}

QList<CADitem::ItemType> CAD_heatcool_butterflyValve::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_butterflyValve::wizardImage()
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

void CAD_heatcool_butterflyValve::calculate()
{

}

void CAD_heatcool_butterflyValve::processWizardInput()
{

}
