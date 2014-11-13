#include "cad_heatcool_pipeteeconnector.h"

CAD_heatcool_pipeTeeConnector::CAD_heatcool_pipeTeeConnector() : CADitem(CADitem::HeatCool_PipeTeeConnector)
{
    this->description = "Heat/Cool|Pipe-T-Connector";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));

    processWizardInput();
    calculate();
}

CAD_heatcool_pipeTeeConnector::~CAD_heatcool_pipeTeeConnector()
{

}

QList<CADitem::ItemType> CAD_heatcool_pipeTeeConnector::flangable_items()
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
    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);

    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);
}

void CAD_heatcool_pipeTeeConnector::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

}
