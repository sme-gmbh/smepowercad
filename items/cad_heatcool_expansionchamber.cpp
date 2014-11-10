#include "cad_heatcool_expansionchamber.h"

CAD_heatcool_expansionChamber::CAD_heatcool_expansionChamber() : CADitem(CADitem::HeatCool_ExpansionChamber)
{
    this->description = "Heat/Cool|Expansion chamber";

}

QList<CADitem::ItemType> CAD_heatcool_expansionChamber::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_expansionChamber::wizardImage()
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

void CAD_heatcool_expansionChamber::calculate()
{

}

void CAD_heatcool_expansionChamber::processWizardInput()
{

}
