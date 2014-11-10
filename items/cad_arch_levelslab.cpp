#include "cad_arch_levelslab.h"

CAD_arch_levelSlab::CAD_arch_levelSlab() : CADitem(CADitem::Arch_LevelSlab)
{
    this->description = "Architecture|Level slab";

}

QList<CADitem::ItemType> CAD_arch_levelSlab::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_arch_levelSlab::wizardImage()
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

void CAD_arch_levelSlab::calculate()
{

}

void CAD_arch_levelSlab::processWizardInput()
{

}
