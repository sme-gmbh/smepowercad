#include "cad_arch_window.h"

CAD_arch_window::CAD_arch_window() : CADitem(CADitem::Arch_Window)
{
    this->description = "Architecture|Window";

}

QList<CADitem::ItemType> CAD_arch_window::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_arch_window::wizardImage()
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

void CAD_arch_window::calculate()
{

}

void CAD_arch_window::processWizardInput()
{

}
