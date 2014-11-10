#include "cad_arch_support.h"

CAD_arch_support::CAD_arch_support() : CADitem(CADitem::Arch_Support)
{
    this->description = "Architecture|Support";

}

QList<CADitem::ItemType> CAD_arch_support::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_arch_support::wizardImage()
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

void CAD_arch_support::calculate()
{

}

void CAD_arch_support::processWizardInput()
{

}
