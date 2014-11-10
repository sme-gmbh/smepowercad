#include "cad_air_emptycabinet.h"

CAD_air_emptyCabinet::CAD_air_emptyCabinet() : CADitem(CADitem::Air_EmptyCabinet)
{
    this->description = "Air|Empty cabinet";

}

QList<CADitem::ItemType> CAD_air_emptyCabinet::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_emptyCabinet::wizardImage()
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

void CAD_air_emptyCabinet::calculate()
{

}

void CAD_air_emptyCabinet::processWizardInput()
{

}
