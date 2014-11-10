#include "cad_heatcool_filter.h"

CAD_heatcool_filter::CAD_heatcool_filter() : CADitem(CADitem::HeatCool_Filter)
{
    this->description = "Heat/Cool|Filter";

}

QList<CADitem::ItemType> CAD_heatcool_filter::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_filter::wizardImage()
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

void CAD_heatcool_filter::calculate()
{

}

void CAD_heatcool_filter::processWizardInput()
{

}
