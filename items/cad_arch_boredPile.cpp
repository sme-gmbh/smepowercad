#include "cad_arch_boredPile.h"

CAD_arch_boredPile::CAD_arch_boredPile() : CADitem(CADitem::Arch_BoredPile)
{
    this->description = "Architecture|BoredPile";

}

QList<CADitem::ItemType> CAD_arch_boredPile::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_arch_boredPile::wizardImage()
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

void CAD_arch_boredPile::calculate()
{

}

void CAD_arch_boredPile::processWizardInput()
{

}
