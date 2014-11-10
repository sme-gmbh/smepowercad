#include "cad_heatcool_flowmeter.h"

CAD_heatcool_flowmeter::CAD_heatcool_flowmeter() : CADitem(CADitem::HeatCool_Flowmeter)
{
    this->description = "Heat/Cool|Flowmeter";

}

QList<CADitem::ItemType> CAD_heatcool_flowmeter::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_flowmeter::wizardImage()
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

void CAD_heatcool_flowmeter::calculate()
{

}

void CAD_heatcool_flowmeter::processWizardInput()
{

}
