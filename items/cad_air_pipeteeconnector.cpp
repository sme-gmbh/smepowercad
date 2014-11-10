#include "cad_air_pipeteeconnector.h"

CAD_air_pipeTeeConnector::CAD_air_pipeTeeConnector() : CADitem(CADitem::Air_PipeTeeConnector)
{
    this->description = "Air|Pipe T-Connector";

}

QList<CADitem::ItemType> CAD_air_pipeTeeConnector::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_pipeTeeConnector::wizardImage()
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

void CAD_air_pipeTeeConnector::calculate()
{

}

void CAD_air_pipeTeeConnector::processWizardInput()
{

}
