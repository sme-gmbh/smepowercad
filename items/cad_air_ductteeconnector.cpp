#include "cad_air_ductteeconnector.h"

CAD_air_ductTeeConnector::CAD_air_ductTeeConnector() : CADitem(CADitem::Air_DuctTeeConnector)
{
    this->description = "Air|Duct T-Connector";

}

QList<CADitem::ItemType> CAD_air_ductTeeConnector::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_air_ductTeeConnector::wizardImage()
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

void CAD_air_ductTeeConnector::calculate()
{

}

void CAD_air_ductTeeConnector::processWizardInput()
{

}
