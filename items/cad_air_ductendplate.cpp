#include "cad_air_ductendplate.h"

CAD_air_ductEndPlate::CAD_air_ductEndPlate() : CADitem(CADitem::Air_DuctEndPlate)
{
    this->description = "Air|Duct end plate";

}

QList<CADitem::ItemType> CAD_air_ductEndPlate::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_ductEndPlate::wizardImage()
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

void CAD_air_ductEndPlate::calculate()
{

}

void CAD_air_ductEndPlate::processWizardInput()
{

}
