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

#include "cad_sprinkler_head.h"
#include "itemdb.h"

CAD_sprinkler_head::CAD_sprinkler_head() : CADitem(CADitemTypes::Sprinkler_Head)
{
    head = new CAD_basic_circle;
    endcap = new CAD_basic_pipe;
    this->subItems.append(head);
    this->subItems.append(endcap);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 50.0);
    wizardParams.insert("d", 20.0);
    wizardParams.insert("d2", 50.0);

    processWizardInput();
    calculate();
}

CAD_sprinkler_head::~CAD_sprinkler_head()
{

}

QList<CADitemTypes::ItemType> CAD_sprinkler_head::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Sprinkler_CompressedAirWaterContainer);
    flangable_items.append(CADitemTypes::Sprinkler_Distribution);
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

    return flangable_items;
}

QImage CAD_sprinkler_head::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_sprinkler_head::iconPath()
{
    return ":/icons/cad_sprinkler/cad_sprinkler_head.svg";
}

QString CAD_sprinkler_head::domain()
{
    return "Sprinkler";
}

QString CAD_sprinkler_head::description()
{
    return "Sprinkler|Head";
}

void CAD_sprinkler_head::calculate()
{
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);


    endcap->wizardParams.insert("Position x", position.x());
    endcap->wizardParams.insert("Position y", position.y());
    endcap->wizardParams.insert("Position z", position.z());
    endcap->wizardParams.insert("Angle x", angle_x);
    endcap->wizardParams.insert("Angle y", angle_y);
    endcap->wizardParams.insert("Angle z", angle_z);
    endcap->wizardParams.insert("l", a / 2);
    endcap->wizardParams.insert("d", d);
    endcap->wizardParams.insert("s",  d / 2);
    endcap->processWizardInput();
    endcap->calculate();

    QVector3D position_head = position + matrix_rotation * QVector3D(a, 0.0, 0.0);
    head->wizardParams.insert("Center x", position_head.x());
    head->wizardParams.insert("Center y", position_head.y());
    head->wizardParams.insert("Center z", position_head.z());
    head->wizardParams.insert("Angle x", angle_x);
    head->wizardParams.insert("Angle y", angle_y);
    head->wizardParams.insert("Angle z", angle_z);
    head->wizardParams.insert("r", d2 / 2);
    head->rotateAroundAxis(90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    head->processWizardInput();
    head->calculate();

    this->boundingBox.enterVertices(head->boundingBox.getVertices());
    this->boundingBox.enterVertices(endcap->boundingBox.getVertices());

    this->snap_flanges.append(position);
}

void CAD_sprinkler_head::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a = wizardParams.value("a").toDouble();
    d = wizardParams.value("d").toDouble();
    d2 = wizardParams.value("d2").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

QMatrix4x4 CAD_sprinkler_head::rotationOfFlange(quint8 num)
{
    Q_UNUSED(num);
    QMatrix4x4 m;
    m.setToIdentity();
    m.rotate(180.0, 0.0, 1.0, 0.0);
    return matrix_rotation * m;
}
