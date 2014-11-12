#include "cad_air_pipevolumetricflowcontroller.h"

CAD_air_pipeVolumetricFlowController::CAD_air_pipeVolumetricFlowController() : CADitem(CADitem::Air_PipeVolumetricFlowController)
{
    this->description = "Air|Pipe volumetric flow controller";

}

QList<CADitem::ItemType> CAD_air_pipeVolumetricFlowController::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Air_Pipe);
    flangable_items.append(CADitem::Air_PipeEndCap);
    flangable_items.append(CADitem::Air_PipeFireDamper);
    flangable_items.append(CADitem::Air_PipeReducer);
    flangable_items.append(CADitem::Air_PipeSilencer);
    flangable_items.append(CADitem::Air_PipeTeeConnector);
    flangable_items.append(CADitem::Air_PipeTurn);
    return flangable_items;
}

QImage CAD_air_pipeVolumetricFlowController::wizardImage()
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

void CAD_air_pipeVolumetricFlowController::calculate()
{

}

void CAD_air_pipeVolumetricFlowController::processWizardInput()
{

}
