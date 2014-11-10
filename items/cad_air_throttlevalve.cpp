#include "cad_air_throttlevalve.h"

CAD_air_throttleValve::CAD_air_throttleValve() : CADitem(CADitem::Air_ThrottleValve)
{
    this->description = "Air|Throttle valve";

}

QList<CADitem::ItemType> CAD_air_throttleValve::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_throttleValve::wizardImage()
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

void CAD_air_throttleValve::calculate()
{

}

void CAD_air_throttleValve::processWizardInput()
{

}
