#include "cad_heatcool_controlvalve.h"

CAD_heatcool_controlvalve::CAD_heatcool_controlvalve() : CADitem(CADitem::HeatCool_Controlvalve)
{
    this->description = "Heat/Cool|Controlvalve";

}

QList<CADitem::ItemType> CAD_heatcool_controlvalve::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_controlvalve::wizardImage()
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

void CAD_heatcool_controlvalve::calculate()
{

}

void CAD_heatcool_controlvalve::processWizardInput()
{

}
