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

#include "cad_arch_boredPile.h"

CAD_arch_boredPile::CAD_arch_boredPile() : CADitem(CADitemTypes::Arch_BoredPile)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 5000.0);
    wizardParams.insert("d", 1000.0);

    this->pile = new CAD_basic_pipe();
    this->subItems.append(this->pile);

    processWizardInput();
    calculate();
}

CAD_arch_boredPile::~CAD_arch_boredPile()
{

}

QList<CADitemTypes::ItemType> CAD_arch_boredPile::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Arch_BoredPile);
    flangable_items.append(CADitemTypes::Arch_LevelSlab);
    flangable_items.append(CADitemTypes::Arch_Foundation);
    return flangable_items;
}

QImage CAD_arch_boredPile::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_arch_boredPile::iconPath()
{
    return ":/icons/cad_arch/cad_arch_bored_pile.svg";
}

QString CAD_arch_boredPile::domain()
{
    return "Architecture";
}

QString CAD_arch_boredPile::description()
{
    return "Architecture|BoredPile";
}

void CAD_arch_boredPile::calculate()
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

    pile->wizardParams.insert("Position x", (position.x()));
    pile->wizardParams.insert("Position y", (position.y()));
    pile->wizardParams.insert("Position z", (position.z()));
    pile->wizardParams.insert("Angle x", angle_x);
    pile->wizardParams.insert("Angle y", angle_y);
    pile->wizardParams.insert("Angle z", angle_z);
    pile->wizardParams.insert("l", (a));
    pile->wizardParams.insert("d", (d));
    pile->wizardParams.insert("s", (d/2));
    pile->processWizardInput();
    pile->rotateAroundAxis(90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    pile->calculate();

    this->snap_flanges.append(pile->snap_flanges);
    this->boundingBox = pile->boundingBox;

}

void CAD_arch_boredPile::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a = wizardParams.value("a").toDouble();
    d = wizardParams.value("d").toDouble();

}

QMatrix4x4 CAD_arch_boredPile::rotationOfFlange(quint8 num)
{
    Q_UNUSED(num)
    return matrix_rotation;
}
