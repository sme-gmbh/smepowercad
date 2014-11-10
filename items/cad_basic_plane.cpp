#include "cad_basic_plane.h"

CAD_basic_plane::CAD_basic_plane() : CADitem(CADitem::Basic_Plane)
{
    this->description = "Basic|Plane";

}

QList<CADitem::ItemType> CAD_basic_plane::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_basic_plane::wizardImage()
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

void CAD_basic_plane::calculate()
{

}

void CAD_basic_plane::processWizardInput()
{

}
