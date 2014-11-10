#include "cad_arch_wall_nonloadbearing.h"

CAD_arch_wall_nonLoadBearing::CAD_arch_wall_nonLoadBearing() : CADitem(CADitem::Arch_Wall_nonLoadBearing)
{
    this->description = "Architecture|Wall non load bearing";

}

QList<CADitem::ItemType> CAD_arch_wall_nonLoadBearing::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_arch_wall_nonLoadBearing::wizardImage()
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

void CAD_arch_wall_nonLoadBearing::calculate()
{

}

void CAD_arch_wall_nonLoadBearing::processWizardInput()
{

}
