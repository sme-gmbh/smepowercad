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

#include "cad_arch_wall_loadbearing.h"
#include "itemdb.h"

CAD_arch_wall_loadBearing::CAD_arch_wall_loadBearing() : CADitem(CADitemTypes::Arch_Wall_loadBearing)
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

CAD_arch_wall_loadBearing::~CAD_arch_wall_loadBearing()
{

}

QList<CADitemTypes::ItemType> CAD_arch_wall_loadBearing::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Arch_Wall_loadBearing);
    flangable_items.append(CADitemTypes::Arch_Wall_nonLoadBearing);
    flangable_items.append(CADitemTypes::Arch_Door);
    flangable_items.append(CADitemTypes::Arch_Window);
    return flangable_items;
}

QImage CAD_arch_wall_loadBearing::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_arch_wall_loadBearing::iconPath()
{
    return ":/icons/cad_arch/cad_arch_wall_loadbearing.svg";
}

QString CAD_arch_wall_loadBearing::domain()
{
    return "Architecture";
}

QString CAD_arch_wall_loadBearing::description()
{
    return "Architecture|Wall load bearing";
}

void CAD_arch_wall_loadBearing::calculate()
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

    QVector3D center = (matrix_rotation * QVector3D(l/2.0, 0.0, a/2.0)) + position;
    basic_box->wizardParams.insert("Position x", (center.x()));
    basic_box->wizardParams.insert("Position y", (center.y()));
    basic_box->wizardParams.insert("Position z", (center.z()));
    basic_box->wizardParams.insert("Angle x", (angle_x));
    basic_box->wizardParams.insert("Angle y", (angle_y));
    basic_box->wizardParams.insert("Angle z", (angle_z));
    basic_box->wizardParams.insert("l", (l));
    basic_box->wizardParams.insert("b", (b));
    basic_box->wizardParams.insert("a", (a));
    basic_box->processWizardInput();
    basic_box->calculate();

    this->snap_vertices.append(basic_box->snap_vertices);
    this->snap_center.append(basic_box->snap_center);
    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(b/2, -b/2, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l - b/2, -b/2, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l - b/2, b/2, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(b/2, b/2, 0.0));

    boundingBox = basic_box->boundingBox;
}

void CAD_arch_wall_loadBearing::processWizardInput()
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

QMatrix4x4 CAD_arch_wall_loadBearing::rotationOfFlange(quint8 num)
{
    if (num == 1)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(180.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else if (num == 2  || num == 3)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(-90.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else if (num == 4)
    {
        return matrix_rotation;
    }
    else if (num == 5 || num == 6)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(90.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }

}
