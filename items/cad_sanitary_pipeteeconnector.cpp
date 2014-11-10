#include "cad_sanitary_pipeteeconnector.h"

CAD_sanitary_pipeTeeConnector::CAD_sanitary_pipeTeeConnector() : CADitem(CADitem::Sanitary_PipeTeeConnector)
{
    this->description = "Sanitary|Pipe T-Connector";

}

QList<CADitem::ItemType> CAD_sanitary_pipeTeeConnector::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sanitary_pipeTeeConnector::wizardImage()
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

void CAD_sanitary_pipeTeeConnector::calculate()
{

}

void CAD_sanitary_pipeTeeConnector::processWizardInput()
{

}
