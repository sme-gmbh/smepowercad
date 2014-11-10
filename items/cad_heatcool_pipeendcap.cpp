#include "cad_heatcool_pipeendcap.h"

CAD_heatcool_pipeEndCap::CAD_heatcool_pipeEndCap() : CADitem(CADitem::HeatCool_PipeEndCap)
{
    this->description = "Heat/Cool|Pipe endcap";

}

QList<CADitem::ItemType> CAD_heatcool_pipeEndCap::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_pipeEndCap::wizardImage()
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

void CAD_heatcool_pipeEndCap::calculate()
{

}

void CAD_heatcool_pipeEndCap::processWizardInput()
{

}
