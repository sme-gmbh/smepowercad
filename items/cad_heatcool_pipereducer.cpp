#include "cad_heatcool_pipereducer.h"

CAD_heatcool_pipeReducer::CAD_heatcool_pipeReducer() : CADitem(CADitem::HeatCool_PipeReducer)
{
    this->description = "Heat/Cool|Pipe reducer";

}

QList<CADitem::ItemType> CAD_heatcool_pipeReducer::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_pipeReducer::wizardImage()
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

void CAD_heatcool_pipeReducer::calculate()
{

}

void CAD_heatcool_pipeReducer::processWizardInput()
{

}
