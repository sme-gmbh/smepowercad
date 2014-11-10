#include "cad_air_ductbafflesilencer.h"

CAD_air_ductBaffleSilencer::CAD_air_ductBaffleSilencer() : CADitem(CADitem::Air_DuctBaffleSilencer)
{
    this->description = "Air|Duct baffle silencer";

}

QList<CADitem::ItemType> CAD_air_ductBaffleSilencer::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_ductBaffleSilencer::wizardImage()
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

void CAD_air_ductBaffleSilencer::calculate()
{

}

void CAD_air_ductBaffleSilencer::processWizardInput()
{

}
