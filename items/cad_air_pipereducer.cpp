#include "cad_air_pipereducer.h"

CAD_air_pipeReducer::CAD_air_pipeReducer() : CADitem(CADitem::Air_PipeReducer)
{
    this->description = "Air|Pipe reducer";

}

QList<CADitem::ItemType> CAD_air_pipeReducer::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Air_Pipe);
    flangable_items.append(CADitem::Air_PipeEndCap);
    flangable_items.append(CADitem::Air_PipeFireDamper);
    flangable_items.append(CADitem::Air_PipeReducer);
    flangable_items.append(CADitem::Air_PipeSilencer);
    flangable_items.append(CADitem::Air_PipeTeeConnector);
    flangable_items.append(CADitem::Air_PipeTurn);
    flangable_items.append(CADitem::Air_PipeVolumetricFlowController);
    return flangable_items;
}

QImage CAD_air_pipeReducer::wizardImage()
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

void CAD_air_pipeReducer::calculate()
{

}

void CAD_air_pipeReducer::processWizardInput()
{

}
