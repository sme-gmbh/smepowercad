#include "cad_sprinkler_pump.h"

CAD_sprinkler_pump::CAD_sprinkler_pump() : CADitem(CADitem::Sprinkler_Pump)
{
    this->description = "Sprinkler|Pump";

}

QList<CADitem::ItemType> CAD_sprinkler_pump::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sprinkler_pump::wizardImage()
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

void CAD_sprinkler_pump::calculate()
{

}

void CAD_sprinkler_pump::processWizardInput()
{

}
