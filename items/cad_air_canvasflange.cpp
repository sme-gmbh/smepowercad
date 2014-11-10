#include "cad_air_canvasflange.h"

CAD_air_canvasFlange::CAD_air_canvasFlange() : CADitem(CADitem::Air_CanvasFlange)
{
    this->description = "Air|Canvas flange";
}

QList<CADitem::ItemType> CAD_air_canvasFlange::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_canvasFlange::wizardImage()
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

void CAD_air_canvasFlange::calculate()
{

}

void CAD_air_canvasFlange::processWizardInput()
{

}
