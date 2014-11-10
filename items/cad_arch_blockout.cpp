#include "cad_arch_blockout.h"

CAD_arch_blockOut::CAD_arch_blockOut() : CADitem(CADitem::Arch_BlockOut)
{
    this->description = "Architecture|Blockout";

}

QList<CADitem::ItemType> CAD_arch_blockOut::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_arch_blockOut::wizardImage()
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

void CAD_arch_blockOut::calculate()
{

}

void CAD_arch_blockOut::processWizardInput()
{

}
