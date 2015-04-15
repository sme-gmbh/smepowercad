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
    turn = new CAD_basic_turn;
    left = new CAD_basic_pipe;
    right = new CAD_basic_pipe;
    this->subItems.append(turn);
    this->subItems.append(left);
    this->subItems.append(right);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("r", 500.0);
    wizardParams.insert("alpha", 90.0);
    wizardParams.insert("d", 150.0);
    wizardParams.insert("iso", 15.0);
    wizardParams.insert("s", 10.0);
    wizardParams.insert("l1", 100.0);
    wizardParams.insert("l2", 100.0);

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
    return "HeatCool|Pipe turn";
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

    left->wizardParams.insert("Position x", position.x());
    left->wizardParams.insert("Position y", position.y());
    left->wizardParams.insert("Position z", position.z());
    left->wizardParams.insert("Angle x", angle_x);
    left->wizardParams.insert("Angle y", angle_y);
    left->wizardParams.insert("Angle z", angle_z);
    left->wizardParams.insert("l", l1);
    left->wizardParams.insert("d", d+2*iso);
    left->wizardParams.insert("s",  iso+s);
    left->layer = this->layer;
    left->processWizardInput();
    left->calculate();

    QVector3D position_right = position + matrix_rotation * QVector3D(l1 + sin(alpha / 180 * PI) * r, 0.0, -(1-cos(alpha / 180 * PI)) * r);
    QVector3D angles_right = MAngleCalculations().anglesFromVector(matrix_rotation * QVector3D(cos(alpha / 180 * PI), 0.0, -sin(alpha / 180 * PI)));
    right->wizardParams.insert("Position x", position_right.x());
    right->wizardParams.insert("Position y", position_right.y());
    right->wizardParams.insert("Position z", position_right.z());
    right->wizardParams.insert("Angle x", angles_right.x());
    right->wizardParams.insert("Angle y", angles_right.y());
    right->wizardParams.insert("Angle z", angles_right.z());
    right->wizardParams.insert("l", l2);
    right->wizardParams.insert("d", d+2*iso);
    right->wizardParams.insert("s",  iso+s);
    right->layer = this->layer;
    right->processWizardInput();
    right->calculate();

    QVector3D position_turn = position + matrix_rotation * QVector3D(l1, 0.0, 0.0);
    turn->wizardParams.insert("Position x", position_turn.x());
    turn->wizardParams.insert("Position y", position_turn.y());
    turn->wizardParams.insert("Position z", position_turn.z());
    turn->wizardParams.insert("Angle x", 0.0);
    turn->wizardParams.insert("Angle y", 0.0);
    turn->wizardParams.insert("Angle z", 0.0);
    turn->wizardParams.insert("d", d + 2*iso);
    turn->wizardParams.insert("r", r);
    turn->wizardParams.insert("alpha", alpha);
    turn->wizardParams.insert("s", s + iso);
    turn->layer = this->layer;
    turn->processWizardInput();
    turn->rotateAroundAxis(90, QVector3D(1.0, 0.0, 0.0), angle_x, angle_y, angle_z);
    turn->calculate();

    this->boundingBox = left->boundingBox;
    this->boundingBox.enterVertices(right->boundingBox.getVertices());
    this->boundingBox.enterVertices(turn->boundingBox.getVertices());
    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l1 + sin(alpha / 180 * PI) * r + cos(alpha / 180 * PI) * l2,
                                                                     0.0,
                                                                     -(1-cos(alpha / 180 * PI)) * r - sin(alpha / 180.0 * PI) * l2));
}

void CAD_heatcool_pipeTurn::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    l1 = wizardParams.value("l1").toDouble();
    l2 = wizardParams.value("l2").toDouble();
    r = wizardParams.value("r").toDouble();
    alpha = wizardParams.value("alpha").toDouble();
    d = wizardParams.value("d").toDouble();
    iso = wizardParams.value("iso").toDouble();
    s = wizardParams.value("s").toDouble();
}

QMatrix4x4 CAD_heatcool_pipeTurn::rotationOfFlange(quint8 num)
{
    if(num == 1)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(180.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else if(num == 2)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(alpha, 0.0, 1.0, 0.0);
        return matrix_rotation * m;
    }
    else
        return matrix_rotation;
}
