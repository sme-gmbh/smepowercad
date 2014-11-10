#include "cad_air_equipmentframe.h"

CAD_air_equipmentFrame::CAD_air_equipmentFrame() : CADitem(CADitem::Air_EquipmentFrame)
{
    this->description = "Air|Equipment frame";

}

QList<CADitem::ItemType> CAD_air_equipmentFrame::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_equipmentFrame::wizardImage()
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

void CAD_air_equipmentFrame::calculate()
{

}

void CAD_air_equipmentFrame::processWizardInput()
{

}
