#include "cad_air_humidifier.h"

CAD_air_humidifier::CAD_air_humidifier() : CADitem(CADitem::Air_Humidifier)
{
    this->description = "Air|Humidifier";

}

QList<CADitem::ItemType> CAD_air_humidifier::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_humidifier::wizardImage()
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

void CAD_air_humidifier::calculate()
{

}

void CAD_air_humidifier::processWizardInput()
{

}
