#include "cad_air_fan.h"

CAD_air_fan::CAD_air_fan() : CADitem(CADitem::Air_Fan)
{
    this->description = "Air|Fan";

}

QList<CADitem::ItemType> CAD_air_fan::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Air_CanvasFlange);
    return flangable_items;
}

QImage CAD_air_fan::wizardImage()
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

void CAD_air_fan::calculate()
{

}

void CAD_air_fan::processWizardInput()
{

}
