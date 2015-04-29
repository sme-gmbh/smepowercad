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

#include "cad_arch_support.h"
#include "itemdb.h"

CAD_arch_support::CAD_arch_support() : CADitem(CADitemTypes::Arch_Support)
{
    support = new CAD_basic_box();
    this->subItems.append(support);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 3000.0);
    wizardParams.insert("b",  300.0);
    wizardParams.insert("l",  300.0);

    processWizardInput();
    calculate();
}

CAD_arch_support::~CAD_arch_support()
{

}

QList<CADitemTypes::ItemType> CAD_arch_support::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_arch_support::wizardImage()
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

QString CAD_arch_support::iconPath()
{
    return ":/icons/cad_arch/cad_arch_support.svg";
}

QString CAD_arch_support::domain()
{
    return "Architecture";
}

QString CAD_arch_support::description()
{
    return "Architecture|Support";
}

void CAD_arch_support::calculate()
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

    QVector3D position_sp = position + matrix_rotation * (QVector3D(0.0, 0.0, a/2));
    support->wizardParams.insert("Position x", (position_sp.x()));
    support->wizardParams.insert("Position y", (position_sp.y()));
    support->wizardParams.insert("Position z", (position_sp.z()));
    support->wizardParams.insert("Angle x", (angle_x));
    support->wizardParams.insert("Angle y", (angle_y));
    support->wizardParams.insert("Angle z", (angle_z));
    support->wizardParams.insert("l", (l));
    support->wizardParams.insert("b", (b));
    support->wizardParams.insert("a", (a));
    support->processWizardInput();
    support->calculate();

    this->snap_vertices.append(support->snap_vertices);
    this->boundingBox = support->boundingBox;
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(0.0, 0.0, 0.0)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(0.0, a,   0.0)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(0.0, a,   b)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(0.0, 0.0, b)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(l  , 0.0, 0.0)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(l  , a  , 0.0)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(l  , a  , b)));
//    this->boundingBox.enterVertex(position + matrix_rotation * (QVector3D(l  , 0.0, b)));
}

void CAD_arch_support::processWizardInput()
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

QMatrix4x4 CAD_arch_support::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}
