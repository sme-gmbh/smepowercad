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

#include "cad_air_pipebranch.h"

CAD_air_pipeBranch::CAD_air_pipeBranch() : CADitem(CADitemTypes::Air_PipeBranch)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("l",    1000.0);
    wizardParams.insert("l2",    500.0);
    wizardParams.insert("l3",    500.0);
    wizardParams.insert("d",     200.0);
    wizardParams.insert("d3",    150.0);
    wizardParams.insert("alpha",  30.0);
    wizardParams.insert("s",       1.0);

    this->pipe = new CAD_basic_pipe();
    this->branch = new CAD_basic_pipe();
    this->subItems.append(pipe);
    this->subItems.append(branch);

    processWizardInput();
    calculate();
}

CAD_air_pipeBranch::~CAD_air_pipeBranch()
{

}

QList<CADitemTypes::ItemType> CAD_air_pipeBranch::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Air_Pipe);
    flangable_items.append(CADitemTypes::Air_PipeBranch);
    flangable_items.append(CADitemTypes::Air_PipeSilencer);
    flangable_items.append(CADitemTypes::Air_PipeVolumetricFlowController);
    flangable_items.append(CADitemTypes::Air_PipeFireDamper);
    flangable_items.append(CADitemTypes::Air_PipeEndCap);
    flangable_items.append(CADitemTypes::Air_PipeReducer);
    flangable_items.append(CADitemTypes::Air_PipeTurn);
    return flangable_items;
}

QImage CAD_air_pipeBranch::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_pipeBranch::iconPath()
{
    return ":/icons/cad_air/cad_air_pipebranch.svg";
}

QString CAD_air_pipeBranch::domain()
{
    return "Air";
}

QString CAD_air_pipeBranch::description()
{
    return "Air|Pipe branch";
}

void CAD_air_pipeBranch::calculate()
{
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    pipe->wizardParams.insert("Position x", (position.x()));
    pipe->wizardParams.insert("Position y", (position.y()));
    pipe->wizardParams.insert("Position z", (position.z()));
    pipe->wizardParams.insert("Angle x", (angle_x));
    pipe->wizardParams.insert("Angle y", (angle_y));
    pipe->wizardParams.insert("Angle z", (angle_z));
    pipe->wizardParams.insert("l", (l));
    pipe->wizardParams.insert("d", (d));
    pipe->wizardParams.insert("s", (s));
    pipe->processWizardInput();
    pipe->calculate();

    QVector3D position_br = position + matrix_rotation * QVector3D(l2, 0.0, 0.0);
    branch->wizardParams.insert("Position x", (position_br.x()));
    branch->wizardParams.insert("Position y", (position_br.y()));
    branch->wizardParams.insert("Position z", (position_br.z()));
    branch->wizardParams.insert("Angle x", (angle_x));
    branch->wizardParams.insert("Angle y", (angle_y));
    branch->wizardParams.insert("Angle z", (angle_z));
    branch->wizardParams.insert("l", (l3));
    branch->wizardParams.insert("d", (d3));
    branch->wizardParams.insert("s", (s));
    branch->processWizardInput();
    branch->rotateAroundAxis(-alpha, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    branch->calculate();

    this->snap_flanges.append(position);
    this->snap_flanges.append(pipe->snap_flanges.at(1));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l2 + cos(alpha / 180.0 * PI) * l3, 0.0, sin(alpha / 180.0 * PI) * l3));
    //this->snap_vertices.append(pipe->snap_vertices);

    this->boundingBox = pipe->boundingBox;
    this->boundingBox.enterVertices(branch->boundingBox.getVertices());
}

void CAD_air_pipeBranch::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();


    d = wizardParams.value("d").toDouble();
    d3 = wizardParams.value("d3").toDouble();
    s = wizardParams.value("s").toDouble();
    l = wizardParams.value("l").toDouble();
    l2 = wizardParams.value("l2").toDouble();
    l3 = wizardParams.value("l3").toDouble();
    alpha = wizardParams.value("alpha").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

QMatrix4x4 CAD_air_pipeBranch::rotationOfFlange(quint8 num)
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
    else if(num == 3)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(-alpha, 0.0, 1.0, 0.0);
        return matrix_rotation * m;
    }
    else
        return matrix_rotation;
}
