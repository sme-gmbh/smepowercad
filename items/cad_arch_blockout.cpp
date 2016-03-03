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

#include "cad_arch_blockout.h"

CAD_arch_blockOut::CAD_arch_blockOut() : CADitem(CADitemTypes::Arch_BlockOut)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("g", 30.0);
    wizardParams.insert("h", 20.0);
    wizardParams.insert("b",  5.0);

    this->blockout = new CAD_basic_box();
    this->subItems.append(blockout);

    processWizardInput();
    calculate();
}

CAD_arch_blockOut::~CAD_arch_blockOut()
{

}

QList<CADitemTypes::ItemType> CAD_arch_blockOut::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_arch_blockOut::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_arch_blockOut::iconPath()
{
    return ":/icons/cad_arch/cad_arch_blockout.svg";
}

QString CAD_arch_blockOut::domain()
{
    return "Architecture";
}

QString CAD_arch_blockOut::description()
{
    return "Architecture|Blockout";
}

void CAD_arch_blockOut::calculate()
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

    QVector3D position_b = position + matrix_rotation * QVector3D(b / 2, g / 2, h / 2);
    blockout->wizardParams.insert("Position x", (position_b.x()));
    blockout->wizardParams.insert("Position y", (position_b.y()));
    blockout->wizardParams.insert("Position z", (position_b.z()));
    blockout->wizardParams.insert("Angle x", (angle_x));
    blockout->wizardParams.insert("Angle y", (angle_y));
    blockout->wizardParams.insert("Angle z", (angle_z));
    blockout->wizardParams.insert("l", (b));
    blockout->wizardParams.insert("b", (g));
    blockout->wizardParams.insert("a", (h));
    blockout->processWizardInput();
    blockout->calculate();

    this->snap_vertices.append(blockout->snap_vertices);

    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(0.0,   g, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(  b,   g, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(  b, 0.0, 0.0));

    this->boundingBox.enterVertices(blockout->boundingBox.getVertices());
}

void CAD_arch_blockOut::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    g = wizardParams.value("g").toDouble();
    h = wizardParams.value("h").toDouble();
    b = wizardParams.value("b").toDouble();


}

QMatrix4x4 CAD_arch_blockOut::rotationOfFlange(quint8 num)
{
    Q_UNUSED(num)
    return matrix_rotation;
}
