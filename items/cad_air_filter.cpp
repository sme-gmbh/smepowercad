#include "cad_air_filter.h"

CAD_air_filter::CAD_air_filter() : CADitem(CADitem::Air_Filter)
{
    this->description = "Air|Filter";

}

QList<CADitem::ItemType> CAD_air_filter::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_filter::wizardImage()
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

void CAD_air_filter::calculate()
{

}

void CAD_air_filter::processWizardInput()
{

}
