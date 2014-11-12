#include "cad_arch_wall_loadbearing.h"

CAD_arch_wall_loadBearing::CAD_arch_wall_loadBearing() : CADitem(CADitem::Arch_Wall_loadBearing)
{
    this->description = "Architecture|Wall load bearing";

}

QList<CADitem::ItemType> CAD_arch_wall_loadBearing::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Arch_Wall_loadBearing);
    flangable_items.append(CADitem::Arch_Wall_nonLoadBearing);
    return flangable_items;
}

QImage CAD_arch_wall_loadBearing::wizardImage()
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

void CAD_arch_wall_loadBearing::calculate()
{

}

void CAD_arch_wall_loadBearing::processWizardInput()
{

}
