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

#include "cad_arch_beam.h"

CAD_arch_beam::CAD_arch_beam() : CADitem(CADitemTypes::Arch_Beam)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",  500.0);
    wizardParams.insert("b",  200.0);
    wizardParams.insert("l", 5000.0);

    this->beam = new CAD_basic_box();
    this->subItems.append(beam);

    processWizardInput();
    calculate();
}

CAD_arch_beam::~CAD_arch_beam()
{

}

QList<CADitemTypes::ItemType> CAD_arch_beam::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_arch_beam::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_arch_beam::iconPath()
{
    return ":/icons/cad_arch/cad_arch_beam.svg";
}

QString CAD_arch_beam::domain()
{
    return "Architecture";
}

QString CAD_arch_beam::description()
{
    return "Architecture|Beam";
}

void CAD_arch_beam::calculate()
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

    QVector3D position_b = position + matrix_rotation * QVector3D(l / 2, b / 2, -a / 2);
    beam->wizardParams.insert("Position x", (position_b.x()));
    beam->wizardParams.insert("Position y", (position_b.y()));
    beam->wizardParams.insert("Position z", (position_b.z()));
    beam->wizardParams.insert("Angle x", (angle_x));
    beam->wizardParams.insert("Angle y", (angle_y));
    beam->wizardParams.insert("Angle z", (angle_z));
    beam->wizardParams.insert("l", (l));
    beam->wizardParams.insert("b", (b));
    beam->wizardParams.insert("a", (a));
    beam->processWizardInput();
    beam->calculate();

    this->snap_vertices.append(beam->snap_vertices);

    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(0.0,   b, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(  l,   b, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(  l, 0.0, 0.0));

    foreach(QVector3D vec, beam->snap_vertices)
        boundingBox.enterVertex(vec);
}

void CAD_arch_beam::processWizardInput()
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

QMatrix4x4 CAD_arch_beam::rotationOfFlange(quint8 num)
{
    Q_UNUSED(num)
    return matrix_rotation;
}
