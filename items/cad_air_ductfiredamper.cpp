#include "cad_air_ductfiredamper.h"

CAD_air_ductFireDamper::CAD_air_ductFireDamper() : CADitem(CADitem::Air_DuctFireDamper)
{
    this->description = "Air|Duct fire damper";

}

QList<CADitem::ItemType> CAD_air_ductFireDamper::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_ductFireDamper::wizardImage()
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

void CAD_air_ductFireDamper::calculate()
{

}

void CAD_air_ductFireDamper::processWizardInput()
{

}
