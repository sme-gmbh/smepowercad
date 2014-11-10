#include "cad_air_multileafdamper.h"

CAD_air_multiLeafDamper::CAD_air_multiLeafDamper() : CADitem(CADitem::Air_MultiLeafDamper)
{
    this->description = "Air|Mulit leaf damper";

}

QList<CADitem::ItemType> CAD_air_multiLeafDamper::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_multiLeafDamper::wizardImage()
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

void CAD_air_multiLeafDamper::calculate()
{

}

void CAD_air_multiLeafDamper::processWizardInput()
{

}
