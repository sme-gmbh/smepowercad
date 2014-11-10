#include "cad_heatcool_adjustvalve.h"

CAD_heatcool_adjustvalve::CAD_heatcool_adjustvalve() : CADitem(CADitem::HeatCool_Adjustvalve)
{
    this->description = "Heat/Cool|Adjustvalve";

}

QList<CADitem::ItemType> CAD_heatcool_adjustvalve::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_adjustvalve::wizardImage()
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

void CAD_heatcool_adjustvalve::calculate()
{

}

void CAD_heatcool_adjustvalve::processWizardInput()
{

}
