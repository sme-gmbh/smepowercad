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

#include "cad_arch_wall_nonloadbearing.h"
#include "itemdb.h"

CAD_arch_wall_nonLoadBearing::CAD_arch_wall_nonLoadBearing() : CADitem(CADitemTypes::Arch_Wall_nonLoadBearing)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",  3000.0);
    wizardParams.insert("b",   200.0);
    wizardParams.insert("l", 10000.0);

    basic_box = new CAD_basic_box();
    subItems.append(basic_box);

    processWizardInput();
    calculate();
}

CAD_arch_wall_nonLoadBearing::~CAD_arch_wall_nonLoadBearing()
{

}

QList<CADitemTypes::ItemType> CAD_arch_wall_nonLoadBearing::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Arch_Wall_loadBearing);
    flangable_items.append(CADitemTypes::Arch_Wall_nonLoadBearing);
    return flangable_items;
}

QImage CAD_arch_wall_nonLoadBearing::wizardImage()
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

QString CAD_arch_wall_nonLoadBearing::iconPath()
{
    return ":/icons/cad_arch/cad_arch_wall_nonloadbearing.svg";
}

QString CAD_arch_wall_nonLoadBearing::domain()
{
    return "Architecture";
}

QString CAD_arch_wall_nonLoadBearing::description()
{
    return "Architecture|Wall non load bearing";
}

void CAD_arch_wall_nonLoadBearing::calculate()
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

    QVector3D center = (matrix_rotation * QVector3D(l, b, a) / 2.0) + position;
    basic_box->wizardParams.insert("Position x", (center.x()));
    basic_box->wizardParams.insert("Position y", (center.y()));
    basic_box->wizardParams.insert("Position z", (center.z()));
    basic_box->wizardParams.insert("Angle x", (angle_x));
    basic_box->wizardParams.insert("Angle y", (angle_y));
    basic_box->wizardParams.insert("Angle z", (angle_z));
    basic_box->wizardParams.insert("Size x", (l));
    basic_box->wizardParams.insert("Size y", (b));
    basic_box->wizardParams.insert("Size z", (a));
    basic_box->processWizardInput();
    basic_box->calculate();

    this->snap_vertices.append(basic_box->snap_vertices);
    this->snap_center.append(basic_box->snap_center);
    this->snap_flanges.append(basic_box->pos_bot_1);
    this->snap_flanges.append(basic_box->pos_bot_2);
    this->snap_flanges.append(basic_box->pos_bot_3);
    this->snap_flanges.append(basic_box->pos_bot_4);


    boundingBox = basic_box->boundingBox;
}

void CAD_arch_wall_nonLoadBearing::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    b = wizardParams.value("b").toDouble();
    l = wizardParams.value("l").toDouble();
    a = wizardParams.value("a").toDouble();

}
