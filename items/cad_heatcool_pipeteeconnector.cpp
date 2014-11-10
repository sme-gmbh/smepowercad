#include "cad_heatcool_pipeteeconnector.h"

CAD_heatcool_pipeTeeConnector::CAD_heatcool_pipeTeeConnector() : CADitem(CADitem::HeatCool_PipeTeeConnector)
{
    this->description = "Heat/Cool|Pipe-T-Connector";

}

QList<CADitem::ItemType> CAD_heatcool_pipeTeeConnector::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_pipeTeeConnector::wizardImage()
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

void CAD_heatcool_pipeTeeConnector::calculate()
{

}

void CAD_heatcool_pipeTeeConnector::processWizardInput()
{

}
