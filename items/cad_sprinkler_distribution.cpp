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

#include "cad_sprinkler_distribution.h"
#include "itemdb.h"

CAD_sprinkler_distribution::CAD_sprinkler_distribution() : CADitem(CADitemTypes::Sprinkler_Distribution)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("d",  200.0);
    wizardParams.insert("d2",  50.0);
    wizardParams.insert("l", 1000.0);
    wizardParams.insert("a",  200.0);
    wizardParams.insert("n", 5);
    wizardParams.insert("s", 5.0);

    main_pipe = new CAD_basic_pipe();
    this->subItems.append(main_pipe);

    processWizardInput();
    calculate();
}

CAD_sprinkler_distribution::~CAD_sprinkler_distribution()
{

}

QList<CADitemTypes::ItemType> CAD_sprinkler_distribution::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
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

    return flangable_items;
}

QImage CAD_sprinkler_distribution::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_sprinkler_distribution::iconPath()
{
    return ":/icons/cad_sprinkler/cad_sprinkler_distribution.svg";
}

QString CAD_sprinkler_distribution::domain()
{
    return "Sprinkler";
}

QString CAD_sprinkler_distribution::description()
{
    return "Sprinkler|Distribution";
}

void CAD_sprinkler_distribution::calculate()
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

    this->subItems.clear();
    this->subItems.append(this->main_pipe);

    main_pipe->wizardParams.insert("Position x", position.x());
    main_pipe->wizardParams.insert("Position y", position.y());
    main_pipe->wizardParams.insert("Position z", position.z());
    main_pipe->wizardParams.insert("Angle x", angle_x);
    main_pipe->wizardParams.insert("Angle y", angle_y);
    main_pipe->wizardParams.insert("Angle z", angle_z);
    main_pipe->wizardParams.insert("l", l);
    main_pipe->wizardParams.insert("d", d);
    main_pipe->wizardParams.insert("s", s);
    main_pipe->layer = this->layer;
    main_pipe->processWizardInput();
    main_pipe->calculate();

    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, 0.0));
    this->boundingBox.enterVertices(main_pipe->boundingBox.getVertices());

    for(int i = 0; i < n; i++)
    {
        CAD_basic_pipe *pipe = new CAD_basic_pipe();
        this->subItems.append(pipe);
        QVector3D position_pipe = position + matrix_rotation * QVector3D((i + 0.5) * l / n, 0.0, a);
        pipe->wizardParams.insert("Position x", position_pipe.x());
        pipe->wizardParams.insert("Position y", position_pipe.y());
        pipe->wizardParams.insert("Position z", position_pipe.z());
        pipe->wizardParams.insert("Angle x", angle_x);
        pipe->wizardParams.insert("Angle y", angle_y);
        pipe->wizardParams.insert("Angle z", angle_z);
        pipe->wizardParams.insert("l", a);
        pipe->wizardParams.insert("d", d2);
        pipe->wizardParams.insert("s", s);
        pipe->layer = this->layer;
        pipe->processWizardInput();
        pipe->rotateAroundAxis(90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
        pipe->calculate();
        this->boundingBox.enterVertices(pipe->boundingBox.getVertices());
        this->snap_flanges.append(position_pipe);
    }
}

void CAD_sprinkler_distribution::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    l = wizardParams.value("l").toDouble();
    d = wizardParams.value("d").toDouble();
    d2 = wizardParams.value("d2").toDouble();
    s = wizardParams.value("s").toDouble();
    a = wizardParams.value("a").toDouble();

    n = wizardParams.value("n").toInt();
}

QMatrix4x4 CAD_sprinkler_distribution::rotationOfFlange(quint8 num)
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
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(-90.0, 0.0, 1.0, 0.0);
        return matrix_rotation * m;
    }
}
