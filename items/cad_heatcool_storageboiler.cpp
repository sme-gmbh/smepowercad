#include "cad_heatcool_storageboiler.h"

CAD_heatcool_storageBoiler::CAD_heatcool_storageBoiler() : CADitem(CADitem::HeatCool_StorageBoiler)
{
    this->description = "Heat/Cool|Storage boiler";

}

QList<CADitem::ItemType> CAD_heatcool_storageBoiler::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_storageBoiler::wizardImage()
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

void CAD_heatcool_storageBoiler::calculate()
{

}

void CAD_heatcool_storageBoiler::processWizardInput()
{

}
