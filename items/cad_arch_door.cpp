#include "cad_arch_door.h"

CAD_arch_door::CAD_arch_door() : CADitem(CADitem::Arch_Door)
{
    this->description = "Architecture|Door";

}

QList<CADitem::ItemType> CAD_arch_door::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_arch_door::wizardImage()
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

void CAD_arch_door::calculate()
{

}

void CAD_arch_door::processWizardInput()
{

}
