#include "cad_air_ductvolumetricflowcontroller.h"

CAD_air_ductVolumetricFlowController::CAD_air_ductVolumetricFlowController() : CADitem(CADitem::Air_DuctVolumetricFlowController)
{
    this->description = "Air|Duct volumetric flow controller";

}

QList<CADitem::ItemType> CAD_air_ductVolumetricFlowController::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_ductVolumetricFlowController::wizardImage()
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

void CAD_air_ductVolumetricFlowController::calculate()
{

}

void CAD_air_ductVolumetricFlowController::processWizardInput()
{

}
