#include "cad_heatcool_chiller.h"

CAD_heatcool_chiller::CAD_heatcool_chiller() : CADitem(CADitem::HeatCool_Chiller)
{
    this->description = "Heat/Cool|Chiller";

}

QList<CADitem::ItemType> CAD_heatcool_chiller::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_chiller::wizardImage()
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

void CAD_heatcool_chiller::calculate()
{

}

void CAD_heatcool_chiller::processWizardInput()
{

}
