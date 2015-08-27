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

#include "cad_arch_grating.h"
#include "itemdb.h"

CAD_arch_grating::CAD_arch_grating() : CADitem(CADitemTypes::Arch_Grating)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",  500.0);
    wizardParams.insert("b", 4000.0);
    wizardParams.insert("l", 4000.0);

    grating = new CAD_basic_box();
    subItems.append(grating);

    processWizardInput();
    calculate();
}

CAD_arch_grating::~CAD_arch_grating()
{

}

QList<CADitemTypes::ItemType> CAD_arch_grating::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_arch_grating::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_arch_grating::iconPath()
{
    return ":/icons/cad_arch/cad_arch_grating.svg";
}

QString CAD_arch_grating::domain()
{
    return "Architecture";
}

QString CAD_arch_grating::description()
{
    return "Architecture|Grating";
}

void CAD_arch_grating::calculate()
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
    QVector3D position_sl = position + matrix_rotation * (QVector3D(l/2, b/2, a/2));
    grating->wizardParams.insert("Position x", (position_sl.x()));
    grating->wizardParams.insert("Position y", (position_sl.y()));
    grating->wizardParams.insert("Position z", (position_sl.z()));
    grating->wizardParams.insert("Angle x", (angle_x));
    grating->wizardParams.insert("Angle y", (angle_y));
    grating->wizardParams.insert("Angle z", (angle_z));
    grating->wizardParams.insert("l", (l));
    grating->wizardParams.insert("b", (b));
    grating->wizardParams.insert("a", (a));
    grating->processWizardInput();
    grating->calculate();

    this->snap_flanges.append(grating->snap_vertices[4]);
    this->snap_flanges.append(grating->snap_vertices[5]);
    this->snap_flanges.append(grating->snap_vertices[6]);
    this->snap_flanges.append(grating->snap_vertices[7]);
    this->snap_vertices = grating->snap_vertices;
    this->snap_center = grating->snap_center;

    this->boundingBox = grating->boundingBox;
}

void CAD_arch_grating::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a = wizardParams.value("a").toDouble();
    b = wizardParams.value("b").toDouble();
    l = wizardParams.value("l").toDouble();
}

QMatrix4x4 CAD_arch_grating::rotationOfFlange(quint8 num)
{
    Q_UNUSED(num);
    return matrix_rotation;
}
