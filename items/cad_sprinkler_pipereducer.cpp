#include "cad_sprinkler_pipereducer.h"

CAD_sprinkler_pipeReducer::CAD_sprinkler_pipeReducer() : CADitem(CADitem::Sprinkler_PipeReducer)
{
    this->description = "Sprinkler|Pipe Reducer";

}

QList<CADitem::ItemType> CAD_sprinkler_pipeReducer::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::Sprinkler_CompressedAirWaterContainer);
    flangable_items.append(CADitem::Sprinkler_Distribution);
    flangable_items.append(CADitem::Sprinkler_Head);
    flangable_items.append(CADitem::Sprinkler_Pipe);
    flangable_items.append(CADitem::Sprinkler_PipeEndCap);
    flangable_items.append(CADitem::Sprinkler_PipeReducer);
    flangable_items.append(CADitem::Sprinkler_PipeTurn);
    flangable_items.append(CADitem::Sprinkler_Pump);
    flangable_items.append(CADitem::Sprinkler_TeeConnector);
    flangable_items.append(CADitem::Sprinkler_Valve);
    flangable_items.append(CADitem::Sprinkler_WetAlarmValve);
    flangable_items.append(CADitem::Sprinkler_ZoneCheck);

    return flangable_items;
}

QImage CAD_sprinkler_pipeReducer::wizardImage()
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

void CAD_sprinkler_pipeReducer::calculate()
{

}

void CAD_sprinkler_pipeReducer::processWizardInput()
{

}
