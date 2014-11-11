#include "cad_arch_foundation.h"

CAD_arch_foundation::CAD_arch_foundation() : CADitem(CADitem::Arch_Foundation)
{
    this->description = "Architecture|Foundation";

}

QList<CADitem::ItemType> CAD_arch_foundation::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_arch_foundation::wizardImage()
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

void CAD_arch_foundation::calculate()
{

}

void CAD_arch_foundation::processWizardInput()
{

}
