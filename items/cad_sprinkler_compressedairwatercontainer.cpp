#include "cad_sprinkler_compressedairwatercontainer.h"

CAD_sprinkler_compressedAirWaterContainer::CAD_sprinkler_compressedAirWaterContainer() : CADitem(CADitem::Sprinkler_CompressedAirWaterContainer)
{
    this->description = "Sprinkler|Compressed air water container";

}

QList<CADitem::ItemType> CAD_sprinkler_compressedAirWaterContainer::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sprinkler_compressedAirWaterContainer::wizardImage()
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

void CAD_sprinkler_compressedAirWaterContainer::calculate()
{

}

void CAD_sprinkler_compressedAirWaterContainer::processWizardInput()
{

}
