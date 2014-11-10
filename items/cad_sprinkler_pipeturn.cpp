#include "cad_sprinkler_pipeturn.h"

CAD_sprinkler_pipeTurn::CAD_sprinkler_pipeTurn() : CADitem(CADitem::Sprinkler_PipeTurn)
{
    this->description = "Sprinkler|Pipe Turn";

}

QList<CADitem::ItemType> CAD_sprinkler_pipeTurn::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sprinkler_pipeTurn::wizardImage()
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

void CAD_sprinkler_pipeTurn::calculate()
{

}

void CAD_sprinkler_pipeTurn::processWizardInput()
{

}
