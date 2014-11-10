#include "cad_sanitary_electricwaterheater.h"

CAD_sanitary_electricWaterHeater::CAD_sanitary_electricWaterHeater() : CADitem(CADitem::Sanitary_ElectricWaterHeater)
{
    this->description = "Sanitary|Electric Water Heater";

}

QList<CADitem::ItemType> CAD_sanitary_electricWaterHeater::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sanitary_electricWaterHeater::wizardImage()
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

void CAD_sanitary_electricWaterHeater::calculate()
{

}

void CAD_sanitary_electricWaterHeater::processWizardInput()
{

}
