#include "cad_air_pipeturn.h"

CAD_air_pipeTurn::CAD_air_pipeTurn() : CADitem(CADitem::Air_PipeTurn)
{
    this->description = "Air|Pipe turn";

}

QList<CADitem::ItemType> CAD_air_pipeTurn::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_pipeTurn::wizardImage()
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

void CAD_air_pipeTurn::calculate()
{

}

void CAD_air_pipeTurn::processWizardInput()
{

}
