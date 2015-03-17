/**********************************************************************
** smepowercad
** Copyright (C) 2015 Smart Micro Engineering GmbH
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/

#include "cad_heatcool_pipereducer.h"
#include "itemdb.h"

CAD_heatcool_pipeReducer::CAD_heatcool_pipeReducer() : CADitem(CADitemTypes::HeatCool_PipeReducer)
{
    left = new CAD_basic_pipe;
    right = new CAD_basic_pipe;
    reducer = new CAD_Basic_PipeReducer;
    this->subItems.append(left);
    this->subItems.append(right);
    this->subItems.append(reducer);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("d1", 150.0);
    wizardParams.insert("d2", 200.0);
    wizardParams.insert("l", 200.0);
    wizardParams.insert("l1", 50.0);
    wizardParams.insert("l2", 70.0);
    wizardParams.insert("e", 10.0);
    wizardParams.insert("iso1", 10.0);
    wizardParams.insert("iso2", 10.0);
    wizardParams.insert("s", 10.0);

    processWizardInput();
    calculate();
}

CAD_heatcool_pipeReducer::~CAD_heatcool_pipeReducer()
{

}

QList<CADitemTypes::ItemType> CAD_heatcool_pipeReducer::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::HeatCool_Adjustvalve);
    flangable_items.append(CADitemTypes::HeatCool_BallValve);
    flangable_items.append(CADitemTypes::HeatCool_Boiler);
    flangable_items.append(CADitemTypes::HeatCool_ButterflyValveBolted);
    flangable_items.append(CADitemTypes::HeatCool_ButterflyValveClamped);
    flangable_items.append(CADitemTypes::HeatCool_Chiller);
    flangable_items.append(CADitemTypes::HeatCool_Controlvalve);
    flangable_items.append(CADitemTypes::HeatCool_CoolingTower);
    flangable_items.append(CADitemTypes::HeatCool_DirtArrester);
    flangable_items.append(CADitemTypes::HeatCool_ExpansionChamber);
    flangable_items.append(CADitemTypes::HeatCool_Filter);
    flangable_items.append(CADitemTypes::HeatCool_Flange);
    flangable_items.append(CADitemTypes::HeatCool_Flowmeter);
    flangable_items.append(CADitemTypes::HeatCool_Gauge);
    flangable_items.append(CADitemTypes::HeatCool_Gauge90Degree);
    flangable_items.append(CADitemTypes::HeatCool_HeatexchangerSoldered);
    flangable_items.append(CADitemTypes::HeatCool_HeatexchangerBolted);
    flangable_items.append(CADitemTypes::HeatCool_NonReturnFlap);
    flangable_items.append(CADitemTypes::HeatCool_NonReturnValve);
    flangable_items.append(CADitemTypes::HeatCool_Pipe);
    flangable_items.append(CADitemTypes::HeatCool_PipeEndCap);
    flangable_items.append(CADitemTypes::HeatCool_PipeReducer);
    flangable_items.append(CADitemTypes::HeatCool_PipeTeeConnector);
    flangable_items.append(CADitemTypes::HeatCool_PipeTurn);
    flangable_items.append(CADitemTypes::HeatCool_PumpInline);
    flangable_items.append(CADitemTypes::HeatCool_PumpNorm);
    flangable_items.append(CADitemTypes::HeatCool_Radiator);
    flangable_items.append(CADitemTypes::HeatCool_RadiatorCompact);
    flangable_items.append(CADitemTypes::HeatCool_RadiatorFlange);
    flangable_items.append(CADitemTypes::HeatCool_RadiatorFlangeBent);
    flangable_items.append(CADitemTypes::HeatCool_RadiatorValve);
    flangable_items.append(CADitemTypes::HeatCool_SafetyValve);
    flangable_items.append(CADitemTypes::HeatCool_Sensor);
    flangable_items.append(CADitemTypes::HeatCool_StorageBoiler);
    flangable_items.append(CADitemTypes::HeatCool_Valve);
    flangable_items.append(CADitemTypes::HeatCool_Valve90Degree);
    flangable_items.append(CADitemTypes::HeatCool_ValveHandwheel);
    flangable_items.append(CADitemTypes::HeatCool_ValveHandwheelGear);
    flangable_items.append(CADitemTypes::HeatCool_ValveLever);
    flangable_items.append(CADitemTypes::HeatCool_ValveMotorRect);
    flangable_items.append(CADitemTypes::HeatCool_ValveMotorRound);
    flangable_items.append(CADitemTypes::HeatCool_WaterHeater);
    return flangable_items;
}

QImage CAD_heatcool_pipeReducer::wizardImage()
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

QString CAD_heatcool_pipeReducer::iconPath()
{
    return ":/icons/cad_heatcool/cad_heatcool_pipereducer.svg";
}

QString CAD_heatcool_pipeReducer::domain()
{
    return "HeatCool";
}

QString CAD_heatcool_pipeReducer::description()
{
    return "Heat/Cool|Pipe reducer";
}

void CAD_heatcool_pipeReducer::calculate()
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

    left->wizardParams.insert("Position x", (position.x()));
    left->wizardParams.insert("Position y", (position.y()));
    left->wizardParams.insert("Position z", (position.z()));
    left->wizardParams.insert("Angle x", (angle_x));
    left->wizardParams.insert("Angle y", (angle_y));
    left->wizardParams.insert("Angle z", (angle_z));
    left->wizardParams.insert("l", (l1));
    left->wizardParams.insert("d", (d1+2*iso1+2*s));
    left->wizardParams.insert("s", (iso1+s));
    left->layer = this->layer;
    left->processWizardInput();
    left->calculate();

    QVector3D position_rgt = position + matrix_rotation * QVector3D(l - l2, 0.0, -e);
    right->wizardParams.insert("Position x", (position_rgt.x()));
    right->wizardParams.insert("Position y", (position_rgt.y()));
    right->wizardParams.insert("Position z", (position_rgt.z()));
    right->wizardParams.insert("Angle x", (angle_x));
    right->wizardParams.insert("Angle y", (angle_y));
    right->wizardParams.insert("Angle z", (angle_z));
    right->wizardParams.insert("l", (l2));
    right->wizardParams.insert("d", (d2 + 2 * iso2 + 2 * s));
    right->wizardParams.insert("s", (iso2 + s));
    right->layer = this->layer;
    right->processWizardInput();
    right->calculate();


    QVector3D position_red = position + matrix_rotation * QVector3D(l1, 0.0, 0.0);
    reducer->wizardParams.insert("Position x", (position_red.x()));
    reducer->wizardParams.insert("Position y", (position_red.y()));
    reducer->wizardParams.insert("Position z", (position_red.z()));
    reducer->wizardParams.insert("Angle x", (angle_x));
    reducer->wizardParams.insert("Angle y", (angle_y));
    reducer->wizardParams.insert("Angle z", (angle_z));
    reducer->wizardParams.insert("l", (l - l1 - l2));
    reducer->wizardParams.insert("d1", (d1 + 2 * iso1 + 2 * s));
    reducer->wizardParams.insert("d2", (d2 + 2 * iso2 + 2 * s));
    reducer->wizardParams.insert("e", (e));
    reducer->wizardParams.insert("s", (s));
    reducer->layer = this->layer;
    reducer->processWizardInput();
    reducer->calculate();



    this->boundingBox.enterVertices(left->boundingBox.getVertices());
    this->boundingBox.enterVertices(right->boundingBox.getVertices());
    this->boundingBox.enterVertices(reducer->boundingBox.getVertices());
    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, -e));
}

void CAD_heatcool_pipeReducer::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    d1 = wizardParams.value("d1").toDouble();
    d2 = wizardParams.value("d2").toDouble();
    l = wizardParams.value("l").toDouble();
    l1 = wizardParams.value("l1").toDouble();
    l2 = wizardParams.value("l2").toDouble();
    e = wizardParams.value("e").toDouble();
    s = wizardParams.value("s").toDouble();
    iso1 = wizardParams.value("iso1").toDouble();
    iso2 = wizardParams.value("iso1").toDouble();
    s = wizardParams.value("s").toDouble();
}
