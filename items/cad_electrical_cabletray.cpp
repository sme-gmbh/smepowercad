#include "cad_electrical_cabletray.h"

CAD_electrical_cableTray::CAD_electrical_cableTray() : CADitem(CADitem::Electrical_CableTray)
{
    this->description = "Electrical|Cabletray";

}

QList<CADitem::ItemType> CAD_electrical_cableTray::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Electrical_CableTray);
    return flangable_items;
}

QImage CAD_electrical_cableTray::wizardImage()
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

void CAD_electrical_cableTray::calculate()
{

}

void CAD_electrical_cableTray::processWizardInput()
{

}
