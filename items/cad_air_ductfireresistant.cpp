#include "cad_air_ductfireresistant.h"

CAD_air_ductFireResistant::CAD_air_ductFireResistant() : CADitem(CADitem::Air_DuctFireResistant)
{
    this->description = "Air|Duct (fire resistant)";

}

QList<CADitem::ItemType> CAD_air_ductFireResistant::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_ductFireResistant::wizardImage()
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

void CAD_air_ductFireResistant::calculate()
{

}

void CAD_air_ductFireResistant::processWizardInput()
{

}
