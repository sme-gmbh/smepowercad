#include "cad_arch_beam.h"

CAD_arch_beam::CAD_arch_beam() : CADitem(CADitem::Arch_Beam)
{
    this->description = "Architecture|Beam";

}

QList<CADitem::ItemType> CAD_arch_beam::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_arch_beam::wizardImage()
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

void CAD_arch_beam::calculate()
{

}

void CAD_arch_beam::processWizardInput()
{

}
