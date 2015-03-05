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

#include "cad_heatcool_expansionchamber.h"
#include "itemdb.h"

CAD_heatcool_expansionChamber::CAD_heatcool_expansionChamber() : CADitem(CADitemTypes::HeatCool_ExpansionChamber)
{
    upper = new CAD_Basic_TorisphericalHeadDIN28011;
    lower = new CAD_Basic_TorisphericalHeadDIN28011;
    pipe = new CAD_basic_pipe;
    leg_1 = new CAD_basic_box;
    leg_2 = new CAD_basic_box;
    leg_3 = new CAD_basic_box;
    this->subItems.append(upper);
    this->subItems.append(lower);
    this->subItems.append(pipe);
    this->subItems.append(leg_1);
    this->subItems.append(leg_2);
    this->subItems.append(leg_3);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",  2000.0);
    wizardParams.insert("b",  1500.0);
    wizardParams.insert("c",   400.0);
    wizardParams.insert("d",  150.0);
    wizardParams.insert("d2", 1000.0);
    wizardParams.insert("s",    10.0);

    processWizardInput();
    calculate();
    qDebug() << wizardImage();
}

CAD_heatcool_expansionChamber::~CAD_heatcool_expansionChamber()
{

}

QList<CADitemTypes::ItemType> CAD_heatcool_expansionChamber::flangable_items()
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
    flangable_items.append(CADitemTypes::HeatCool_HeatexchangerSoldered);
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

QImage CAD_heatcool_expansionChamber::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_heatcool_expansionChamber::iconPath()
{
    return ":/icons/cad_heatcool/cad_heatcool_expansionchamber.svg";
}

QString CAD_heatcool_expansionChamber::domain()
{
    return "HeatCool";
}

QString CAD_heatcool_expansionChamber::description()
{
    return "Heat/Cool|Expansion chamber";
}

void CAD_heatcool_expansionChamber::calculate()
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

    QVector3D position_tori = position + matrix_rotation * QVector3D(0.0, 0.0, b);
    upper->wizardParams.insert("Position x", position_tori.x());
    upper->wizardParams.insert("Position y", position_tori.y());
    upper->wizardParams.insert("Position z", position_tori.z());
    upper->wizardParams.insert("Angle x", angle_x);
    upper->wizardParams.insert("Angle y", angle_y);
    upper->wizardParams.insert("Angle z", angle_z);
    upper->wizardParams.insert("d", d2);   // Durchmesser
    qreal h = a/2 - 0.1937742252 * d2; // = a/2 - d + sqrt(0.65) d
    upper->wizardParams.insert("h", h);     // Höhe
    upper->layer = this->layer;
    upper->processWizardInput();
    upper->calculate();

    lower->wizardParams.insert("Position x", position_tori.x());
    lower->wizardParams.insert("Position y", position_tori.y());
    lower->wizardParams.insert("Position z", position_tori.z());
    lower->wizardParams.insert("Angle x", angle_x);
    lower->wizardParams.insert("Angle y", angle_y);
    lower->wizardParams.insert("Angle z", angle_z);
    lower->wizardParams.insert("d", d2);   // Durchmesser
    lower->wizardParams.insert("h", h);     // Höhe
    lower->layer = this->layer;
    lower->processWizardInput();
    lower->rotateAroundAxis(180.0, QVector3D(1.0, 0.0, 0.0), angle_x, angle_y, angle_z);
    lower->calculate();

    QVector3D position_pipe = position + matrix_rotation * QVector3D(0.0, 0.0, c);
    pipe->wizardParams.insert("Position x", position_pipe.x());
    pipe->wizardParams.insert("Position y", position_pipe.y());
    pipe->wizardParams.insert("Position z", position_pipe.z());
    pipe->wizardParams.insert("Angle x", angle_x);
    pipe->wizardParams.insert("Angle y", angle_y);
    pipe->wizardParams.insert("Angle z", angle_z);
    pipe->wizardParams.insert("l", b - h -c);
    pipe->wizardParams.insert("d", d);
    pipe->wizardParams.insert("s",  s);
    pipe->layer = this->layer;
    pipe->processWizardInput();
    pipe->rotateAroundAxis(-90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    pipe->calculate();

    QVector3D position_leg1 = position + matrix_rotation * QVector3D(cos(PI / 6) * d2 * 0.4, sin(PI / 6) * d2 * 0.4 , (b-h)/2);
    leg_1->wizardParams.insert("Position x", position_leg1.x());
    leg_1->wizardParams.insert("Position y", position_leg1.y());
    leg_1->wizardParams.insert("Position z", position_leg1.z());
    leg_1->wizardParams.insert("Angle x", angle_x);
    leg_1->wizardParams.insert("Angle y", angle_y);
    leg_1->wizardParams.insert("Angle z", angle_z);
    leg_1->wizardParams.insert("l", d2/20);
    leg_1->wizardParams.insert("b", d2/20);
    leg_1->wizardParams.insert("a", b - h);
    leg_1->layer = this->layer;
    leg_1->processWizardInput();
    leg_1->calculate();


    QVector3D position_leg2 = position + matrix_rotation * QVector3D(-cos(PI / 6) * d2 * 0.4, sin(PI / 6) * d2 * 0.4 , (b-h)/2);
    leg_2->wizardParams.insert("Position x", position_leg2.x());
    leg_2->wizardParams.insert("Position y", position_leg2.y());
    leg_2->wizardParams.insert("Position z", position_leg2.z());
    leg_2->wizardParams.insert("Angle x", angle_x);
    leg_2->wizardParams.insert("Angle y", angle_y);
    leg_2->wizardParams.insert("Angle z", angle_z);
    leg_2->wizardParams.insert("l", d2/20);
    leg_2->wizardParams.insert("b", d2/20);
    leg_2->wizardParams.insert("a", b - h);
    leg_2->layer = this->layer;
    leg_2->processWizardInput();
    leg_2->calculate();

    QVector3D position_leg3 = position + matrix_rotation * QVector3D(0.0, -d2 * 0.4 , (b-h)/2);
    leg_3->wizardParams.insert("Position x", position_leg3.x());
    leg_3->wizardParams.insert("Position y", position_leg3.y());
    leg_3->wizardParams.insert("Position z", position_leg3.z());
    leg_3->wizardParams.insert("Angle x", angle_x);
    leg_3->wizardParams.insert("Angle y", angle_y);
    leg_3->wizardParams.insert("Angle z", angle_z);
    leg_3->wizardParams.insert("l", d2/20);
    leg_3->wizardParams.insert("b", d2/20);
    leg_3->wizardParams.insert("a", b - h);
    leg_3->layer = this->layer;
    leg_3->processWizardInput();
    leg_3->calculate();

    this->boundingBox = upper->boundingBox;
    this->boundingBox.enterVertices(lower->boundingBox.getVertices());
    this->snap_flanges.append(position_pipe);
    this->snap_vertices.append(position + matrix_rotation * QVector3D(-cos(PI / 6) * d2 * 0.4, sin(PI / 6) * d2 * 0.4 , 0.0));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.0, -d2 * 0.4 , 0.0));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(cos(PI / 6) * d2 * 0.4, sin(PI / 6) * d2 * 0.4 , 0.0));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.0, 0.0, a/2 + b));

}

void CAD_heatcool_expansionChamber::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a = wizardParams.value("a").toDouble();
    b = wizardParams.value("b").toDouble();
    c = wizardParams.value("c").toDouble();
    s = wizardParams.value("s").toDouble();
    d = wizardParams.value("d").toDouble();
    d2 = wizardParams.value("d2").toDouble();
}
