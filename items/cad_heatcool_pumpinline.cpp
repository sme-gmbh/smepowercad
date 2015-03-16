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

#include "cad_heatcool_pumpinline.h"
#include "itemdb.h"

CAD_HeatCool_PumpInline::CAD_HeatCool_PumpInline() : CADitem(CADitemTypes::HeatCool_PumpInline)
{
    pipe = new CAD_basic_pipe;
    flange_left = new CAD_basic_pipe;
    flange_right = new CAD_basic_pipe;
    motor = new CAD_basic_pipe;
    housing = new CAD_basic_pipe;
    this->subItems.append(pipe);
    this->subItems.append(flange_left);
    this->subItems.append(flange_right);
    this->subItems.append(motor);
    this->subItems.append(housing);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 200.0);
    wizardParams.insert("d", 150.0);
    wizardParams.insert("e", 500.0);
    wizardParams.insert("f", 300.0);
    wizardParams.insert("fe", 10.0);
    wizardParams.insert("ff", 10.0);
    wizardParams.insert("l", 500.0);
    wizardParams.insert("l1", 400.0);
    wizardParams.insert("s", 10.0);

    processWizardInput();
    calculate();
}

CAD_HeatCool_PumpInline::~CAD_HeatCool_PumpInline()
{

}

QList<CADitemTypes::ItemType> CAD_HeatCool_PumpInline::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::HeatCool_ButterflyValveBolted);
    flangable_items.append(CADitemTypes::HeatCool_ButterflyValveClamped);
    flangable_items.append(CADitemTypes::HeatCool_DirtArrester);
    flangable_items.append(CADitemTypes::HeatCool_Filter);
    flangable_items.append(CADitemTypes::HeatCool_HeatexchangerBolted);
    flangable_items.append(CADitemTypes::HeatCool_HeatexchangerSoldered);
    flangable_items.append(CADitemTypes::HeatCool_NonReturnFlap);
    flangable_items.append(CADitemTypes::HeatCool_NonReturnValve);
    flangable_items.append(CADitemTypes::HeatCool_Pipe);
    flangable_items.append(CADitemTypes::HeatCool_PipeEndCap);
    flangable_items.append(CADitemTypes::HeatCool_PipeReducer);
    flangable_items.append(CADitemTypes::HeatCool_PipeTeeConnector);
    flangable_items.append(CADitemTypes::HeatCool_PipeTurn);
    flangable_items.append(CADitemTypes::HeatCool_PumpInline);
    flangable_items.append(CADitemTypes::HeatCool_PumpNorm);
    flangable_items.append(CADitemTypes::HeatCool_SafetyValve);
    flangable_items.append(CADitemTypes::HeatCool_Valve);
    flangable_items.append(CADitemTypes::HeatCool_Valve90Degree);
    return flangable_items;
}

QImage CAD_HeatCool_PumpInline::wizardImage()
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

QString CAD_HeatCool_PumpInline::iconPath()
{
    return ":/icons/cad_heatcool/cad_heatcool_pumpinline.svg";
}

QString CAD_HeatCool_PumpInline::domain()
{
    return "HeatCool";
}

QString CAD_HeatCool_PumpInline::description()
{
    return "Heat/Cool|Pump Inline";
}

void CAD_HeatCool_PumpInline::calculate()
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

    pipe->wizardParams.insert("Position x", position.x());
    pipe->wizardParams.insert("Position y", position.y());
    pipe->wizardParams.insert("Position z", position.z());
    pipe->wizardParams.insert("Angle x", angle_x);
    pipe->wizardParams.insert("Angle y", angle_y);
    pipe->wizardParams.insert("Angle z", angle_z);
    pipe->wizardParams.insert("l", l);
    pipe->wizardParams.insert("d", d);
    pipe->wizardParams.insert("s",  s);
    pipe->layer = this->layer;
    pipe->processWizardInput();
    pipe->calculate();

    flange_left->wizardParams.insert("Position x", position.x());
    flange_left->wizardParams.insert("Position y", position.y());
    flange_left->wizardParams.insert("Position z", position.z());
    flange_left->wizardParams.insert("Angle x", angle_x);
    flange_left->wizardParams.insert("Angle y", angle_y);
    flange_left->wizardParams.insert("Angle z", angle_z);
    flange_left->wizardParams.insert("l", fe);
    flange_left->wizardParams.insert("d", d + 2 * ff);
    flange_left->wizardParams.insert("s",  ff);
    flange_left->layer = this->layer;
    flange_left->processWizardInput();
    flange_left->calculate();

    QVector3D position_flange = position + matrix_rotation * QVector3D(l - fe, 0.0, 0.0);
    flange_right->wizardParams.insert("Position x", position_flange.x());
    flange_right->wizardParams.insert("Position y", position_flange.y());
    flange_right->wizardParams.insert("Position z", position_flange.z());
    flange_right->wizardParams.insert("Angle x", angle_x);
    flange_right->wizardParams.insert("Angle y", angle_y);
    flange_right->wizardParams.insert("Angle z", angle_z);
    flange_right->wizardParams.insert("l", fe);
    flange_right->wizardParams.insert("d", d + 2 * ff);
    flange_right->wizardParams.insert("s",  ff);
    flange_right->layer = this->layer;
    flange_right->processWizardInput();
    flange_right->calculate();

    QVector3D position_housing = position + matrix_rotation * QVector3D(l/2, 0.0, a/2);
    housing->wizardParams.insert("Position x", position_housing.x());
    housing->wizardParams.insert("Position y", position_housing.y());
    housing->wizardParams.insert("Position z", position_housing.z());
    housing->wizardParams.insert("Angle x", angle_x);
    housing->wizardParams.insert("Angle y", angle_y);
    housing->wizardParams.insert("Angle z", angle_z);
    housing->wizardParams.insert("l", a);
    housing->wizardParams.insert("d", l1);
    housing->wizardParams.insert("s",  l1/2);
    housing->layer = this->layer;
    housing->processWizardInput();
    housing->rotateAroundAxis(90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    housing->calculate();

    QVector3D position_motor = position + matrix_rotation * QVector3D(l/2, 0.0, a/2 + e);
    motor->wizardParams.insert("Position x", position_motor.x());
    motor->wizardParams.insert("Position y", position_motor.y());
    motor->wizardParams.insert("Position z", position_motor.z());
    motor->wizardParams.insert("Angle x", angle_x);
    motor->wizardParams.insert("Angle y", angle_y);
    motor->wizardParams.insert("Angle z", angle_z);
    motor->wizardParams.insert("l", e);
    motor->wizardParams.insert("d", f);
    motor->wizardParams.insert("s",  f/2);
    motor->layer = this->layer;
    motor->processWizardInput();
    motor->rotateAroundAxis(90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    motor->calculate();

    this->boundingBox = flange_left->boundingBox;
    this->boundingBox.enterVertices(flange_right->boundingBox.getVertices());
    this->boundingBox.enterVertices(motor->boundingBox.getVertices());
    this->boundingBox.enterVertices(housing->boundingBox.getVertices());

    this->snap_flanges = pipe->snap_flanges;
}

void CAD_HeatCool_PumpInline::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();


    a = wizardParams.value("a").toDouble();
    d = wizardParams.value("d").toDouble();
    e = wizardParams.value("e").toDouble();
    f = wizardParams.value("f").toDouble();
    fe = wizardParams.value("fe").toDouble();
    ff = wizardParams.value("ff").toDouble();
    l = wizardParams.value("l").toDouble();
    l1  =wizardParams.value("l1").toDouble();
    s  =wizardParams.value("s").toDouble();

}
