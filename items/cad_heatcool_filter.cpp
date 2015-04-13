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

#include "cad_heatcool_filter.h"
#include "itemdb.h"

CAD_heatcool_filter::CAD_heatcool_filter() : CADitem(CADitemTypes::HeatCool_Filter)
{
    filter = new CAD_basic_pipe;
    flange_left = new CAD_basic_pipe;
    flange_right = new CAD_basic_pipe;
    pipe_left = new CAD_basic_pipe;
    pipe_right = new CAD_basic_pipe;
    this->subItems.append(filter);
    this->subItems.append(flange_left);
    this->subItems.append(flange_right);
    this->subItems.append(pipe_left);
    this->subItems.append(pipe_right);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",  500.0);
    wizardParams.insert("a2", 350.0);
    wizardParams.insert("a3", 400.0);
    wizardParams.insert("e",  200.0);
    wizardParams.insert("fe",  10.0);
    wizardParams.insert("ff", 10.0);
    wizardParams.insert("l",  500.0);
    wizardParams.insert("d",  150.0);
    wizardParams.insert("s",   10.0);

    processWizardInput();
    calculate();
}

CAD_heatcool_filter::~CAD_heatcool_filter()
{

}

QList<CADitemTypes::ItemType> CAD_heatcool_filter::flangable_items()
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

QImage CAD_heatcool_filter::wizardImage()
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

QString CAD_heatcool_filter::iconPath()
{
    return ":/icons/cad_heatcool/cad_heatcool_filter.svg";
}

QString CAD_heatcool_filter::domain()
{
    return "HeatCool";
}

QString CAD_heatcool_filter::description()
{
    return "HeatCool|Filter";
}

void CAD_heatcool_filter::calculate()
{
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    pipe_left->wizardParams.insert("Position x", position.x());
    pipe_left->wizardParams.insert("Position y", position.y());
    pipe_left->wizardParams.insert("Position z", position.z());
    pipe_left->wizardParams.insert("Angle x", angle_x);
    pipe_left->wizardParams.insert("Angle y", angle_y);
    pipe_left->wizardParams.insert("Angle z", angle_z);
    pipe_left->wizardParams.insert("l", (l - e) / 2 * 1.2);
    pipe_left->wizardParams.insert("d", d);
    pipe_left->wizardParams.insert("s",  s);
    pipe_left->layer = this->layer;
    pipe_left->processWizardInput();
    pipe_left->calculate();

    flange_left->wizardParams.insert("Position x", position.x());
    flange_left->wizardParams.insert("Position y", position.y());
    flange_left->wizardParams.insert("Position z", position.z());
    flange_left->wizardParams.insert("Angle x", angle_x);
    flange_left->wizardParams.insert("Angle y", angle_y);
    flange_left->wizardParams.insert("Angle z", angle_z);
    flange_left->wizardParams.insert("l", fe);
    flange_left->wizardParams.insert("d", d + 2 * ff);
    flange_left->wizardParams.insert("s", ff);
    flange_left->layer = this->layer;
    flange_left->processWizardInput();
    flange_left->calculate();

    QVector3D position_pr = position + matrix_rotation * QVector3D(0.4*l + 0.6*e, 0.0, -a2 + a3);
    pipe_right->wizardParams.insert("Position x", position_pr.x());
    pipe_right->wizardParams.insert("Position y", position_pr.y());
    pipe_right->wizardParams.insert("Position z", position_pr.z());
    pipe_right->wizardParams.insert("Angle x", angle_x);
    pipe_right->wizardParams.insert("Angle y", angle_y);
    pipe_right->wizardParams.insert("Angle z", angle_z);
    pipe_right->wizardParams.insert("l", (l - e) / 2 * 1.2);
    pipe_right->wizardParams.insert("d", d);
    pipe_right->wizardParams.insert("s",  s);
    pipe_right->layer = this->layer;
    pipe_right->processWizardInput();
    pipe_right->calculate();

    QVector3D position_fr = position + matrix_rotation * QVector3D(l - fe, 0.0, -a2 + a3);
    flange_right->wizardParams.insert("Position x", position_fr.x());
    flange_right->wizardParams.insert("Position y", position_fr.y());
    flange_right->wizardParams.insert("Position z", position_fr.z());
    flange_right->wizardParams.insert("Angle x", angle_x);
    flange_right->wizardParams.insert("Angle y", angle_y);
    flange_right->wizardParams.insert("Angle z", angle_z);
    flange_right->wizardParams.insert("l", fe);
    flange_right->wizardParams.insert("d", d + 2 * ff);
    flange_right->wizardParams.insert("s", ff);
    flange_right->layer = this->layer;
    flange_right->processWizardInput();
    flange_right->calculate();

    QVector3D position_filter = position + matrix_rotation * QVector3D(l/2, 0.0, -a2 + a);
    filter->wizardParams.insert("Position x", position_filter.x());
    filter->wizardParams.insert("Position y", position_filter.y());
    filter->wizardParams.insert("Position z", position_filter.z());
    filter->wizardParams.insert("Angle x", angle_x);
    filter->wizardParams.insert("Angle y", angle_y);
    filter->wizardParams.insert("Angle z", angle_z);
    filter->wizardParams.insert("l", a);
    filter->wizardParams.insert("d", e);
    filter->wizardParams.insert("s", e/2);
    filter->layer = this->layer;
    filter->processWizardInput();
    filter->rotateAroundAxis(90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    filter->calculate();

    this->boundingBox = pipe_left->boundingBox;
    this->boundingBox.enterVertices(pipe_right->boundingBox.getVertices());
    this->boundingBox.enterVertices(flange_left->boundingBox.getVertices());
    this->boundingBox.enterVertices(flange_right->boundingBox.getVertices());
    this->boundingBox.enterVertices(filter->boundingBox.getVertices());

    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, -a2 + a3));

}

void CAD_heatcool_filter::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a = wizardParams.value("a").toDouble();
    a2 = wizardParams.value("a2").toDouble();
    a3 = wizardParams.value("a3").toDouble();
    e = wizardParams.value("e").toDouble();
    fe = wizardParams.value("fe").toDouble();
    ff = wizardParams.value("ff").toDouble();
    l = wizardParams.value("l").toDouble();
    d = wizardParams.value("d").toDouble();
    s = wizardParams.value("s").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

QMatrix4x4 CAD_heatcool_filter::rotationOfFlange(quint8 num)
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
        return matrix_rotation;
    }
    else
        return matrix_rotation;
}
