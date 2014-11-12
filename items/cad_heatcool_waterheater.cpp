#include "cad_heatcool_waterheater.h"

CAD_heatcool_waterHeater::CAD_heatcool_waterHeater() : CADitem(CADitem::HeatCool_WaterHeater)
{
    this->description = "Heat/Cool|Water heater";

}

QList<CADitem::ItemType> CAD_heatcool_waterHeater::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;
    flangable_items.append(CADitem::HeatCool_Adjustvalve);
    flangable_items.append(CADitem::HeatCool_BallValve);
    flangable_items.append(CADitem::HeatCool_Boiler);
    flangable_items.append(CADitem::HeatCool_ButterflyValve);
    flangable_items.append(CADitem::HeatCool_Chiller);
    flangable_items.append(CADitem::HeatCool_Controlvalve);
    flangable_items.append(CADitem::HeatCool_CoolingTower);
    flangable_items.append(CADitem::HeatCool_ExpansionChamber);
    flangable_items.append(CADitem::HeatCool_Filter);
    flangable_items.append(CADitem::HeatCool_Flange);
    flangable_items.append(CADitem::HeatCool_Flowmeter);
    flangable_items.append(CADitem::HeatCool_HeatExchanger);
    flangable_items.append(CADitem::HeatCool_Pipe);
    flangable_items.append(CADitem::HeatCool_PipeEndCap);
    flangable_items.append(CADitem::HeatCool_PipeReducer);
    flangable_items.append(CADitem::HeatCool_PipeTeeConnector);
    flangable_items.append(CADitem::HeatCool_PipeTurn);
    flangable_items.append(CADitem::HeatCool_Pump);
    flangable_items.append(CADitem::HeatCool_Radiator);
    flangable_items.append(CADitem::HeatCool_SafetyValve);
    flangable_items.append(CADitem::HeatCool_Sensor);
    flangable_items.append(CADitem::HeatCool_StorageBoiler);
    flangable_items.append(CADitem::HeatCool_WaterHeater);
    return flangable_items;
}

QImage CAD_heatcool_waterHeater::wizardImage()
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

void CAD_heatcool_waterHeater::calculate()
{

}

void CAD_heatcool_waterHeater::processWizardInput()
{

}
