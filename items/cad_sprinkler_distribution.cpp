#include "cad_sprinkler_distribution.h"

CAD_sprinkler_distribution::CAD_sprinkler_distribution() : CADitem(CADitem::Sprinkler_Distribution)
{
    this->description = "Sprinkler|Distribution";

}

QList<CADitem::ItemType> CAD_sprinkler_distribution::flangable_items()
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

QImage CAD_sprinkler_distribution::wizardImage()
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

void CAD_sprinkler_distribution::calculate()
{

}

void CAD_sprinkler_distribution::processWizardInput()
{

}
