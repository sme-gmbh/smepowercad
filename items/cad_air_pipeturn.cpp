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

#include "cad_air_pipeturn.h"
#include "itemdb.h"

CAD_air_pipeTurn::CAD_air_pipeTurn() : CADitem(CADitemTypes::Air_PipeTurn)
{
    this->turn = new CAD_basic_turn();
    this->subItems.append(turn);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("d",     20.0);
    wizardParams.insert("r",    100.0);
    wizardParams.insert("alpha", 90.0);
    wizardParams.insert("s",      1.0);

    processWizardInput();
    calculate();
}

CAD_air_pipeTurn::~CAD_air_pipeTurn()
{

}

QList<CADitemTypes::ItemType> CAD_air_pipeTurn::flangable_items()
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

QImage CAD_air_pipeTurn::wizardImage()
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

QString CAD_air_pipeTurn::iconPath()
{
    return ":/icons/cad_air/cad_air_pipeturn.svg";
}

QString CAD_air_pipeTurn::domain()
{
    return "Air";
}

QString CAD_air_pipeTurn::description()
{
    return "Air|Pipe turn";
}

void CAD_air_pipeTurn::calculate()
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

    turn->wizardParams.insert("Position x", (position.x()));
    turn->wizardParams.insert("Position y", (position.y()));
    turn->wizardParams.insert("Position z", (position.z()));
    turn->wizardParams.insert("Angle x", (angle_x));
    turn->wizardParams.insert("Angle y", (angle_y));
    turn->wizardParams.insert("Angle z", (angle_z));
    turn->wizardParams.insert("s", (s));
    turn->wizardParams.insert("Turn radius", (r));
    turn->wizardParams.insert("Turn angle", (alpha));
    turn->wizardParams.insert("Outer diameter", (d));
    turn->processWizardInput();
    turn->calculate();

    this->snap_flanges = turn->snap_flanges;
    this->snap_center = turn->snap_center;
    this->snap_vertices = turn->snap_vertices;
    this->boundingBox = turn->boundingBox;
}

void CAD_air_pipeTurn::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
    alpha = wizardParams.value("alpha").toDouble();
    r = wizardParams.value("r").toDouble();
    d = wizardParams.value("d").toDouble();
    s = wizardParams.value("s").toDouble();

}
