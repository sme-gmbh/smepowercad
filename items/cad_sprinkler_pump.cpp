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

#include "cad_sprinkler_pump.h"
#include "itemdb.h"

CAD_sprinkler_pump::CAD_sprinkler_pump() : CADitem(CADitemTypes::Sprinkler_Pump)
{
    pipe_left = new CAD_basic_pipe();
    pipe_upper = new CAD_basic_pipe();
    housing = new CAD_basic_pipe();
    axis = new CAD_basic_pipe();
    this->subItems.append(pipe_left);
    this->subItems.append(pipe_upper);
    this->subItems.append(housing);
    this->subItems.append(axis);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",  100.0);
    wizardParams.insert("d",  150.0);
    wizardParams.insert("d3",  50.0);
    wizardParams.insert("d4", 500.0);
    wizardParams.insert("e",  100.0);
    wizardParams.insert("l1", 100.0);
    wizardParams.insert("l2", 100.0);
    wizardParams.insert("l3", 100.0);
    wizardParams.insert("s", 10.0);

    processWizardInput();
    calculate();
}

CAD_sprinkler_pump::~CAD_sprinkler_pump()
{

}

QList<CADitemTypes::ItemType> CAD_sprinkler_pump::flangable_items(int flangeIndex)
{
    QList<CADitemTypes::ItemType> flangable_items;
    if(flangeIndex == 3)
    {
        flangable_items.append(CADitemTypes::Electrical_MotorAsynchronous);
    }
    else
    {
        flangable_items.append(CADitemTypes::Sprinkler_CompressedAirWaterContainer);
        flangable_items.append(CADitemTypes::Sprinkler_Distribution);
        flangable_items.append(CADitemTypes::Sprinkler_Flange);
        flangable_items.append(CADitemTypes::Sprinkler_Head);
        flangable_items.append(CADitemTypes::Sprinkler_Pipe);
        flangable_items.append(CADitemTypes::Sprinkler_PipeEndCap);
        flangable_items.append(CADitemTypes::Sprinkler_PipeReducer);
        flangable_items.append(CADitemTypes::Sprinkler_PipeTurn);
        flangable_items.append(CADitemTypes::Sprinkler_Pump);
        flangable_items.append(CADitemTypes::Sprinkler_PipeTeeConnector);
        flangable_items.append(CADitemTypes::Sprinkler_Valve);
        flangable_items.append(CADitemTypes::Sprinkler_WetAlarmValve);
        flangable_items.append(CADitemTypes::Sprinkler_ZoneCheck);
    }
    return flangable_items;
}

QImage CAD_sprinkler_pump::wizardImage()
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

QString CAD_sprinkler_pump::iconPath()
{
    return ":/icons/cad_sprinkler/cad_sprinkler_pump.svg";
}

QString CAD_sprinkler_pump::domain()
{
    return "Sprinkler";
}

QString CAD_sprinkler_pump::description()
{
    return "Sprinkler|Pump";
}

void CAD_sprinkler_pump::calculate()
{
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    QVector3D position_left = position + matrix_rotation * QVector3D(-l2/2 - l1, 0.0, d4/2);
    pipe_left->wizardParams.insert("Position x", position_left.x());
    pipe_left->wizardParams.insert("Position y", position_left.y());
    pipe_left->wizardParams.insert("Position z", position_left.z());
    pipe_left->wizardParams.insert("Angle x", angle_x);
    pipe_left->wizardParams.insert("Angle y", angle_y);
    pipe_left->wizardParams.insert("Angle z", angle_z);
    pipe_left->wizardParams.insert("l", l1);
    pipe_left->wizardParams.insert("d", d);
    pipe_left->wizardParams.insert("s",  s);
    pipe_left->layer = this->layer;
    pipe_left->processWizardInput();
    pipe_left->calculate();

    QVector3D position_upper = position + matrix_rotation * QVector3D(0.0, e, d4/2);
    pipe_upper->wizardParams.insert("Position x", position_upper.x());
    pipe_upper->wizardParams.insert("Position y", position_upper.y());
    pipe_upper->wizardParams.insert("Position z", position_upper.z());
    pipe_upper->wizardParams.insert("Angle x", angle_x);
    pipe_upper->wizardParams.insert("Angle y", angle_y);
    pipe_upper->wizardParams.insert("Angle z", angle_z);
    pipe_upper->wizardParams.insert("l",a + d4/2);
    pipe_upper->wizardParams.insert("d", d);
    pipe_upper->wizardParams.insert("s",  s);
    pipe_upper->layer = this->layer;
    pipe_upper->processWizardInput();
    pipe_upper->rotateAroundAxis(-90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    pipe_upper->calculate();

    QVector3D position_axis = position + matrix_rotation * QVector3D(l2/2 + l3, 0.0, d4/2);
    axis->wizardParams.insert("Position x", position_axis.x());
    axis->wizardParams.insert("Position y", position_axis.y());
    axis->wizardParams.insert("Position z", position_axis.z());
    axis->wizardParams.insert("Angle x", angle_x);
    axis->wizardParams.insert("Angle y", angle_y);
    axis->wizardParams.insert("Angle z", angle_z);
    axis->wizardParams.insert("l", l3);
    axis->wizardParams.insert("d", d3);
    axis->wizardParams.insert("s",  d3/2);
    axis->layer = this->layer;
    axis->processWizardInput();
    axis->rotateAroundAxis(180.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    axis->calculate();

    QVector3D position_housing = position + matrix_rotation * QVector3D(-l2/2, 0.0, d4/2);
    housing->wizardParams.insert("Position x", position_housing.x());
    housing->wizardParams.insert("Position y", position_housing.y());
    housing->wizardParams.insert("Position z", position_housing.z());
    housing->wizardParams.insert("Angle x", angle_x);
    housing->wizardParams.insert("Angle y", angle_y);
    housing->wizardParams.insert("Angle z", angle_z);
    housing->wizardParams.insert("l", l2);
    housing->wizardParams.insert("d", d4);
    housing->wizardParams.insert("s",  d4/2);
    housing->layer = this->layer;
    housing->processWizardInput();
    housing->calculate();

    foreach(CADitem *item, subItems)
    {
        this->boundingBox.enterVertices(item->boundingBox.getVertices());
    }
    this->snap_flanges.append(position + matrix_rotation * QVector3D(-l2/2 -l1, 0.0, d4/2));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(0.0,e, d4 + a));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l2/2 + l3,0.0, d4/2));
}

void CAD_sprinkler_pump::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a = wizardParams.value("a").toDouble();
    d = wizardParams.value("d").toDouble();
    d3 = wizardParams.value("d3").toDouble();
    d4 = wizardParams.value("d4").toDouble();
    e = wizardParams.value("e").toDouble();
    fe = wizardParams.value("fe").toDouble();
    ff = wizardParams.value("ff").toDouble();
    l1 = wizardParams.value("l1").toDouble();
    l2 = wizardParams.value("l2").toDouble();
    l3 = wizardParams.value("l3").toDouble();
    s = wizardParams.value("s").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);

}

QMatrix4x4 CAD_sprinkler_pump::rotationOfFlange(quint8 num)
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
//        QMatrix4x4 m;
//        m.setToIdentity();
//        m.rotate(90.0, 0.0, 1.0, 0.0);
//        return matrix_rotation * m;
        return pipe_upper->matrix_rotation;
    }
    else
        return matrix_rotation;
}
