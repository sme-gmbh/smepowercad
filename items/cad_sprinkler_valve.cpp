#include "cad_sprinkler_valve.h"

CAD_sprinkler_valve::CAD_sprinkler_valve() : CADitem(CADitem::Sprinkler_Valve)
{
    this->description = "Sprinkler|Valve";

}

QList<CADitem::ItemType> CAD_sprinkler_valve::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sprinkler_valve::wizardImage()
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

void CAD_sprinkler_valve::calculate()
{

}

void CAD_sprinkler_valve::processWizardInput()
{

}
