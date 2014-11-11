#include "cad_arch_grating.h"

CAD_arch_grating::CAD_arch_grating() : CADitem(CADitem::Arch_Grating)
{
    this->description = "Architecture|Grating";

}

QList<CADitem::ItemType> CAD_arch_grating::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_arch_grating::wizardImage()
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

void CAD_arch_grating::calculate()
{

}

void CAD_arch_grating::processWizardInput()
{

}
