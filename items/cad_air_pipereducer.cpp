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

#include "cad_air_pipereducer.h"
#include "caditemtypes.h"

CAD_air_pipeReducer::CAD_air_pipeReducer() : CADitem(CADitemTypes::Air_PipeReducer)
{
    this->reducer = new CAD_Basic_PipeReducer();
    this->subItems.append(reducer);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("l",  40.0);
    wizardParams.insert("d1", 30.0);
    wizardParams.insert("d2", 20.0);
    wizardParams.insert("s",   1.0);

    processWizardInput();
    calculate();
}

CAD_air_pipeReducer::~CAD_air_pipeReducer()
{

}

QList<CADitemTypes::ItemType> CAD_air_pipeReducer::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Air_Pipe);
    flangable_items.append(CADitemTypes::Air_PipeEndCap);
    flangable_items.append(CADitemTypes::Air_PipeFireDamper);
    flangable_items.append(CADitemTypes::Air_PipeReducer);
    flangable_items.append(CADitemTypes::Air_PipeSilencer);
    flangable_items.append(CADitemTypes::Air_PipeTeeConnector);
    flangable_items.append(CADitemTypes::Air_PipeTurn);
    flangable_items.append(CADitemTypes::Air_PipeVolumetricFlowController);
    return flangable_items;
}

QImage CAD_air_pipeReducer::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_pipeReducer::iconPath()
{
    return ":/icons/cad_air/cad_air_pipereducer.svg";
}

QString CAD_air_pipeReducer::domain()
{
    return "Air";
}

QString CAD_air_pipeReducer::description()
{
    return "Air|Pipe reducer";
}

void CAD_air_pipeReducer::calculate()
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
    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, 0.0));

    reducer->wizardParams.insert("Position x", (position.x()));
    reducer->wizardParams.insert("Position y", (position.y()));
    reducer->wizardParams.insert("Position z", (position.z()));
    reducer->wizardParams.insert("Angle x", (angle_x));
    reducer->wizardParams.insert("Angle y", (angle_y));
    reducer->wizardParams.insert("Angle z", (angle_z));
    reducer->wizardParams.insert("l", (l));
    reducer->wizardParams.insert("e", (0.0));
    reducer->wizardParams.insert("d1", (d1));
    reducer->wizardParams.insert("d2", (d2));
    reducer->wizardParams.insert("s", (s));
    reducer->layer = this->layer;
    reducer->processWizardInput();
    reducer->calculate();


    this->boundingBox = reducer->boundingBox;
}

void CAD_air_pipeReducer::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    s = wizardParams.value("s").toDouble();
    d1 = wizardParams.value("d1").toDouble();
    d2 = wizardParams.value("d2").toDouble();
    l = wizardParams.value("l").toDouble();

}


