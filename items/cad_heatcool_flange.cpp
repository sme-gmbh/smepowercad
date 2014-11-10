#include "cad_heatcool_flange.h"

CAD_heatcool_flange::CAD_heatcool_flange() : CADitem(CADitem::HeatCool_Flange)
{
    this->description = "Heat/Cool|Flange";

}

QList<CADitem::ItemType> CAD_heatcool_flange::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_flange::wizardImage()
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

void CAD_heatcool_flange::calculate()
{

}

void CAD_heatcool_flange::processWizardInput()
{

}
