#include "cad_air_pressurereliefdamper.h"

CAD_air_pressureReliefDamper::CAD_air_pressureReliefDamper() : CADitem(CADitem::Air_PressureReliefDamper)
{
    this->description = "Air|Pressure relief damper";

}

QList<CADitem::ItemType> CAD_air_pressureReliefDamper::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_pressureReliefDamper::wizardImage()
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

void CAD_air_pressureReliefDamper::calculate()
{

}

void CAD_air_pressureReliefDamper::processWizardInput()
{

}
