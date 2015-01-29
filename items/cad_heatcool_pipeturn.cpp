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

#include "cad_heatcool_pipeturn.h"
#include "itemdb.h"

CAD_heatcool_pipeTurn::CAD_heatcool_pipeTurn() : CADitem(CADitemTypes::HeatCool_PipeTurn)
{
    turn = new CAD_basic_turn();
    endcap_1 = new CAD_basic_pipe();
    endcap_2 = new CAD_basic_pipe();
    this->subItems.append(turn);
    this->subItems.append(endcap_1);
    this->subItems.append(endcap_2);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("d",  50.0);
    wizardParams.insert("do", 60.0);
    wizardParams.insert("r", 100.0);
    wizardParams.insert("alpha", 90.0);
    wizardParams.insert("b", 100.0);
    wizardParams.insert("l", 100.0);
    wizardParams.insert("s",   5.0);

    processWizardInput();
    calculate();
}

CAD_heatcool_pipeTurn::~CAD_heatcool_pipeTurn()
{

}

QList<CADitemTypes::ItemType> CAD_heatcool_pipeTurn::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::HeatCool_Adjustvalve);
    flangable_items.append(CADitemTypes::HeatCool_BallValve);
    flangable_items.append(CADitemTypes::HeatCool_Boiler);
    flangable_items.append(CADitemTypes::HeatCool_ButterflyValve);
    flangable_items.append(CADitemTypes::HeatCool_Chiller);
    flangable_items.append(CADitemTypes::HeatCool_Controlvalve);
    flangable_items.append(CADitemTypes::HeatCool_CoolingTower);
    flangable_items.append(CADitemTypes::HeatCool_ExpansionChamber);
    flangable_items.append(CADitemTypes::HeatCool_Filter);
    flangable_items.append(CADitemTypes::HeatCool_Flange);
    flangable_items.append(CADitemTypes::HeatCool_Flowmeter);
    flangable_items.append(CADitemTypes::HeatCool_HeatExchanger);
    flangable_items.append(CADitemTypes::HeatCool_Pipe);
    flangable_items.append(CADitemTypes::HeatCool_PipeEndCap);
    flangable_items.append(CADitemTypes::HeatCool_PipeReducer);
    flangable_items.append(CADitemTypes::HeatCool_PipeTeeConnector);
    flangable_items.append(CADitemTypes::HeatCool_PipeTurn);
    flangable_items.append(CADitemTypes::HeatCool_Pump);
    flangable_items.append(CADitemTypes::HeatCool_Radiator);
    flangable_items.append(CADitemTypes::HeatCool_SafetyValve);
    flangable_items.append(CADitemTypes::HeatCool_Sensor);
    flangable_items.append(CADitemTypes::HeatCool_StorageBoiler);
    flangable_items.append(CADitemTypes::HeatCool_WaterHeater);
    return flangable_items;
}

QImage CAD_heatcool_pipeTurn::wizardImage()
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

QString CAD_heatcool_pipeTurn::iconPath()
{
    return ":/icons/cad_heatcool/cad_heatcool_pipeturn.svg";
}

QString CAD_heatcool_pipeTurn::domain()
{
    return "HeatCool";
}

QString CAD_heatcool_pipeTurn::description()
{
    return "Heat/Cool|Pipe turn";
}

void CAD_heatcool_pipeTurn::calculate()
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

    QVector3D position_turn = position + matrix_rotation * QVector3D(l - r, 0.0, 0.0);
    turn->wizardParams.insert("Position x", position_turn.x());
    turn->wizardParams.insert("Position y", position_turn.y());
    turn->wizardParams.insert("Position z", position_turn.z());
    turn->wizardParams.insert("Angle x", angle_x);
    turn->wizardParams.insert("Angle y", angle_y);
    turn->wizardParams.insert("Angle z", angle_z);
    turn->wizardParams.insert("s",              (d_o - d)/2 + s);
    turn->wizardParams.insert("Turn radius",    r);
    turn->wizardParams.insert("Turn angle",     alpha);
    turn->wizardParams.insert("Outer diameter", d_o);
    turn->layer = this->layer;
    turn->processWizardInput();
    turn->calculate();

    endcap_1->wizardParams.insert("Position x", position.x());
    endcap_1->wizardParams.insert("Position y", position.y());
    endcap_1->wizardParams.insert("Position z", position.z());
    endcap_1->wizardParams.insert("Angle x", angle_x);
    endcap_1->wizardParams.insert("Angle y", angle_y);
    endcap_1->wizardParams.insert("Angle z", angle_z);
    endcap_1->wizardParams.insert("l", l-r);
    endcap_1->wizardParams.insert("d", d_o);
    endcap_1->wizardParams.insert("s",  (d_o - d)/2 + s);
    endcap_1->layer = this->layer;
    endcap_1->processWizardInput();
    endcap_1->calculate();

    QVector3D position_end2 = turn->snap_flanges.at(1);
    endcap_2->wizardParams.insert("Position x", position_end2.x());
    endcap_2->wizardParams.insert("Position y", position_end2.y());
    endcap_2->wizardParams.insert("Position z", position_end2.z());
    endcap_2->wizardParams.insert("Angle x", angle_x);
    endcap_2->wizardParams.insert("Angle y", angle_y);
    endcap_2->wizardParams.insert("Angle z", angle_z - alpha);
    endcap_2->wizardParams.insert("l", b-r);
    endcap_2->wizardParams.insert("d", d_o);
    endcap_2->wizardParams.insert("s",  (d_o - d)/2 + s);
    endcap_2->layer = this->layer;
    endcap_2->processWizardInput();
    endcap_2->calculate();

    this->snap_flanges.append(position);
    this->snap_flanges.append(endcap_2->snap_flanges.at(1));

    this->boundingBox.enterVertices(turn->boundingBox.getVertices());
    this->boundingBox.enterVertices(endcap_1->boundingBox.getVertices());
    this->boundingBox.enterVertices(endcap_2->boundingBox.getVertices());

}

void CAD_heatcool_pipeTurn::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
    d = wizardParams.value("d").toDouble();
    d_o = wizardParams.value("do").toDouble();
    s = wizardParams.value("s").toDouble();
    l = wizardParams.value("l").toDouble();
    b = wizardParams.value("b").toDouble();
    alpha = wizardParams.value("alpha").toDouble();
    r = wizardParams.value("r").toDouble();



}
