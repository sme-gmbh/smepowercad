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

#include "cad_heatcool_safetyvalve.h"
#include "itemdb.h"

CAD_heatcool_safetyValve::CAD_heatcool_safetyValve() : CADitem(CADitemTypes::HeatCool_SafetyValve)
{
    pipe_left = new CAD_basic_pipe;
    pipe_lower = new CAD_basic_pipe;
    flange_left = new CAD_basic_pipe;
    flange_lower = new CAD_basic_pipe;
    valve =new CAD_basic_pipe;
    handle_1 = new CAD_basic_box;
    handle_2 = new CAD_basic_box;
    this->subItems.append(pipe_left);
    this->subItems.append(pipe_lower);
    this->subItems.append(flange_left);
    this->subItems.append(flange_lower);
    this->subItems.append(valve);
    this->subItems.append(handle_1);
    this->subItems.append(handle_2);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 600.0);
    wizardParams.insert("a2", 500.0);
    wizardParams.insert("d", 150.0);
    wizardParams.insert("e", 200.0);
    wizardParams.insert("f", 200.0);
    wizardParams.insert("fe", 10.0);
    wizardParams.insert("ff", 10.0);
    wizardParams.insert("l1", 200.0);
    wizardParams.insert("l2", 200.0);
    wizardParams.insert("s", 10.0);

    processWizardInput();
    calculate();
}

CAD_heatcool_safetyValve::~CAD_heatcool_safetyValve()
{

}

QList<CADitemTypes::ItemType> CAD_heatcool_safetyValve::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
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
    flangable_items.append(CADitemTypes::HeatCool_WaterHeater);
    return flangable_items;
}

QImage CAD_heatcool_safetyValve::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_heatcool_safetyValve::iconPath()
{
    return ":/icons/cad_heatcool/cad_heatcool_safetyvalve.svg";
}

QString CAD_heatcool_safetyValve::domain()
{
    return "HeatCool";
}

QString CAD_heatcool_safetyValve::description()
{
    return "HeatCool|Safety valve";
}

void CAD_heatcool_safetyValve::calculate()
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
    pipe_left->wizardParams.insert("l", l1);
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
    flange_left->wizardParams.insert("s",  ff);
    flange_left->layer = this->layer;
    flange_left->processWizardInput();
    flange_left->calculate();

    QVector3D position_lower = position + matrix_rotation * QVector3D(l1, 0.0, -l2);
    pipe_lower->wizardParams.insert("Position x", position_lower.x());
    pipe_lower->wizardParams.insert("Position y", position_lower.y());
    pipe_lower->wizardParams.insert("Position z", position_lower.z());
    pipe_lower->wizardParams.insert("Angle x", angle_x);
    pipe_lower->wizardParams.insert("Angle y", angle_y);
    pipe_lower->wizardParams.insert("Angle z", angle_z);
    pipe_lower->wizardParams.insert("l", l1);
    pipe_lower->wizardParams.insert("d", d);
    pipe_lower->wizardParams.insert("s",  s);
    pipe_lower->layer = this->layer;
    pipe_lower->processWizardInput();
    pipe_lower->rotateAroundAxis(-90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    pipe_lower->calculate();

    flange_lower->wizardParams.insert("Position x", position_lower.x());
    flange_lower->wizardParams.insert("Position y", position_lower.y());
    flange_lower->wizardParams.insert("Position z", position_lower.z());
    flange_lower->wizardParams.insert("Angle x", angle_x);
    flange_lower->wizardParams.insert("Angle y", angle_y);
    flange_lower->wizardParams.insert("Angle z", angle_z);
    flange_lower->wizardParams.insert("l", fe);
    flange_lower->wizardParams.insert("d", d + 2 * ff);
    flange_lower->wizardParams.insert("s",  ff);
    flange_lower->layer = this->layer;
    flange_lower->processWizardInput();
    flange_lower->rotateAroundAxis(-90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    flange_lower->calculate();

    QVector3D position_valve = position + matrix_rotation * QVector3D(l1, 0.0, a2);
    valve->wizardParams.insert("Position x", position_valve.x());
    valve->wizardParams.insert("Position y", position_valve.y());
    valve->wizardParams.insert("Position z", position_valve.z());
    valve->wizardParams.insert("Angle x", angle_x);
    valve->wizardParams.insert("Angle y", angle_y);
    valve->wizardParams.insert("Angle z", angle_z);
    valve->wizardParams.insert("l", a);
    valve->wizardParams.insert("d", f);
    valve->wizardParams.insert("s",  f/2);
    valve->layer = this->layer;
    valve->processWizardInput();
    valve->rotateAroundAxis(90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    valve->calculate();

    QVector3D position_h1 = position + matrix_rotation * QVector3D(l1 + f/2 + (e-f/2) / 4, 0.0, a2 - (e-f/2) / 4);
    handle_1->wizardParams.insert("Position x", position_h1.x());
    handle_1->wizardParams.insert("Position y", position_h1.y());
    handle_1->wizardParams.insert("Position z", position_h1.z());
    handle_1->wizardParams.insert("Angle x", angle_x);
    handle_1->wizardParams.insert("Angle y", angle_y);
    handle_1->wizardParams.insert("Angle z", angle_z);
    handle_1->wizardParams.insert("l", (e-f/2) / 2);
    handle_1->wizardParams.insert("b", (e-f/2) / 2);
    handle_1->wizardParams.insert("a", (e-f/2) / 2);
    handle_1->layer = this->layer;
    handle_1->processWizardInput();
    handle_1->calculate();

    QVector3D position_h2 = position + matrix_rotation * QVector3D(l1 + f/2 + 3*(e-f/2) / 4, 0.0, a2 / 2);
    handle_2->wizardParams.insert("Position x", position_h2.x());
    handle_2->wizardParams.insert("Position y", position_h2.y());
    handle_2->wizardParams.insert("Position z", position_h2.z());
    handle_2->wizardParams.insert("Angle x", angle_x);
    handle_2->wizardParams.insert("Angle y", angle_y);
    handle_2->wizardParams.insert("Angle z", angle_z);
    handle_2->wizardParams.insert("l", (e-f/2) / 2);
    handle_2->wizardParams.insert("b", (e-f/2) / 2);
    handle_2->wizardParams.insert("a", a2);
    handle_2->layer = this->layer;
    handle_2->processWizardInput();
    handle_2->calculate();

    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l1, 0.0, -l2));
    foreach(CADitem *item, subItems)
    {
        this->boundingBox.enterVertices(item->boundingBox.getVertices());
    }
}

void CAD_heatcool_safetyValve::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a = wizardParams.value("a").toDouble();
    a2 = wizardParams.value("a2").toDouble();
    d = wizardParams.value("d").toDouble();
    e = wizardParams.value("e").toDouble();
    f = wizardParams.value("f").toDouble();
    fe = wizardParams.value("fe").toDouble();
    ff = wizardParams.value("ff").toDouble();
    l1 = wizardParams.value("l1").toDouble();
    l2 = wizardParams.value("l2").toDouble();
    s = wizardParams.value("s").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

QMatrix4x4 CAD_heatcool_safetyValve::rotationOfFlange(quint8 num)
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
        m.rotate(90.0, 0.0, 1.0, 0.0);
        return matrix_rotation * m;
    }
    else
        return matrix_rotation;
}
