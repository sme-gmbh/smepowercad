#include "cad_heatcool_heatexchanger.h"

CAD_heatcool_heatExchanger::CAD_heatcool_heatExchanger() : CADitem(CADitem::HeatCool_HeatExchanger)
{
    this->description = "Heat/Cool|Heatexchanger";
    wizardParams.insert("Position x", QVariant::fromValue(0.0));
    wizardParams.insert("Position y", QVariant::fromValue(0.0));
    wizardParams.insert("Position z", QVariant::fromValue(0.0));
    wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    wizardParams.insert("Angle z", QVariant::fromValue(0.0));

    processWizardInput();
    calculate();
}

CAD_heatcool_heatExchanger::~CAD_heatcool_heatExchanger()
{

}

QList<CADitem::ItemType> CAD_heatcool_heatExchanger::flangable_items()
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

QImage CAD_heatcool_heatExchanger::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    ;

    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_heatcool_heatExchanger::iconPath()
{
    return ":/icons/cad_heatcool/cad_heatcool_heatexchanger.svg";
}

QString CAD_heatcool_heatExchanger::domain()
{
    return "HeatCool";
}

void CAD_heatcool_heatExchanger::calculate()
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

void CAD_heatcool_heatExchanger::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

}
