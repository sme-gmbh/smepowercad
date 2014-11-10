#include "cad_air_ductypiece.h"

CAD_air_ductYpiece::CAD_air_ductYpiece() : CADitem(CADitem::Air_DuctYpiece)
{
    this->description = "Air|Duct Y-Piece";

}

QList<CADitem::ItemType> CAD_air_ductYpiece::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_ductYpiece::wizardImage()
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

void CAD_air_ductYpiece::calculate()
{

}

void CAD_air_ductYpiece::processWizardInput()
{

}
