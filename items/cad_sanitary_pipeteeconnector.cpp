#include "cad_sanitary_pipeteeconnector.h"

CAD_sanitary_pipeTeeConnector::CAD_sanitary_pipeTeeConnector() : CADitem(CADitem::Sanitary_PipeTeeConnector)
{
    this->description = "Sanitary|Pipe T-Connector";

}

QList<CADitem::ItemType> CAD_sanitary_pipeTeeConnector::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Sanitary_ElectricWaterHeater);
    flangable_items.append(CADitem::Sanitary_EmergencyEyeShower);
    flangable_items.append(CADitem::Sanitary_EmergencyShower);
    flangable_items.append(CADitem::Sanitary_Flange);
    flangable_items.append(CADitem::Sanitary_LiftingUnit);
    flangable_items.append(CADitem::Sanitary_Pipe);
    flangable_items.append(CADitem::Sanitary_PipeEndCap);
    flangable_items.append(CADitem::Sanitary_PipeReducer);
    flangable_items.append(CADitem::Sanitary_PipeTeeConnector);
    flangable_items.append(CADitem::Sanitary_PipeTurn);
    flangable_items.append(CADitem::Sanitary_Shower);
    flangable_items.append(CADitem::Sanitary_Sink);
    flangable_items.append(CADitem::Sanitary_WashBasin);

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
