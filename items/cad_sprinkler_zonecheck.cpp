#include "cad_sprinkler_zonecheck.h"

CAD_sprinkler_zoneCheck::CAD_sprinkler_zoneCheck() : CADitem(CADitem::Sprinkler_ZoneCheck)
{
    this->description = "Sprinkler|ZoneCheck";

}

QList<CADitem::ItemType> CAD_sprinkler_zoneCheck::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sprinkler_zoneCheck::wizardImage()
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

void CAD_sprinkler_zoneCheck::calculate()
{

}

void CAD_sprinkler_zoneCheck::processWizardInput()
{

}
