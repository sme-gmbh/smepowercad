#include "cad_sprinkler_teeconnector.h"

CAD_sprinkler_teeConnector::CAD_sprinkler_teeConnector() : CADitem(CADitem::Sprinkler_TeeConnector)
{
    this->description = "Sprinkler|T-Connector";

}

QList<CADitem::ItemType> CAD_sprinkler_teeConnector::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_sprinkler_teeConnector::wizardImage()
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

void CAD_sprinkler_teeConnector::calculate()
{

}

void CAD_sprinkler_teeConnector::processWizardInput()
{

}
