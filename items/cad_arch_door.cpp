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

#include "cad_arch_door.h"
#include "itemdb.h"

CAD_arch_door::CAD_arch_door() : CADitem(CADitemTypes::Arch_Door)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);
    wizardParams.insert("a", 2300.0);       //Höhe Rahmen
    wizardParams.insert("b",  300.0);       //Tiefe Ramen
    wizardParams.insert("l", 1000.0);       //Breite Rahmen
    wizardParams.insert("s1",   50.0);       //Rahmenstärke oben
    wizardParams.insert("s2",   50.0);       //Rahmenstärke seitlich
    wizardParams.insert("alpha", -90.0);    //Öffnungswinkel Tür

    box_left = new CAD_basic_box();
    box_right = new CAD_basic_box();
    door = new CAD_basic_box();
    box_up = new CAD_basic_box();
    arc = new CAD_basic_arc();
    arrow_1 = new CAD_basic_line();
    arrow_2 = new CAD_basic_line();

    this->subItems.append(box_left);
    this->subItems.append(box_up);
    this->subItems.append(box_right);
    this->subItems.append(door);
    this->subItems.append(arc);
    this->subItems.append(arrow_1);
    this->subItems.append(arrow_2);

    processWizardInput();
    calculate();
}

CAD_arch_door::~CAD_arch_door()
{

}

QList<CADitemTypes::ItemType> CAD_arch_door::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Arch_Wall_loadBearing);
    flangable_items.append(CADitemTypes::Arch_Wall_nonLoadBearing);
    flangable_items.append(CADitemTypes::Arch_Door);
    flangable_items.append(CADitemTypes::Arch_Window);
    return flangable_items;
}

QImage CAD_arch_door::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_arch_door::iconPath()
{
    return ":/icons/cad_arch/cad_arch_door.svg";
}

QString CAD_arch_door::domain()
{
    return "Architecture";
}

QString CAD_arch_door::description()
{
    return "Architecture|Door";
}

void CAD_arch_door::calculate()
{
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    QVector3D pos_left = (matrix_rotation * QVector3D(s2/2.0, 0.0, a/2.0)) + position;
    box_left->wizardParams.insert("Position x", (pos_left.x()));
    box_left->wizardParams.insert("Position y", (pos_left.y()));
    box_left->wizardParams.insert("Position z", (pos_left.z()));
    box_left->wizardParams.insert("Angle x", (angle_x));
    box_left->wizardParams.insert("Angle y", (angle_y));
    box_left->wizardParams.insert("Angle z", (angle_z));
    box_left->wizardParams.insert("l", (s2));
    box_left->wizardParams.insert("b", (b));
    box_left->wizardParams.insert("a", (a));
    box_left->layer = this->layer;
    box_left->processWizardInput();
    box_left->calculate();

    QVector3D pos_right = (matrix_rotation * QVector3D(l - s2/2.0, 0.0, a/2.0)) + position;
    box_right->wizardParams.insert("Position x", (pos_right.x()));
    box_right->wizardParams.insert("Position y", (pos_right.y()));
    box_right->wizardParams.insert("Position z", (pos_right.z()));
    box_right->wizardParams.insert("Angle x", (angle_x));
    box_right->wizardParams.insert("Angle y", (angle_y));
    box_right->wizardParams.insert("Angle z", (angle_z));
    box_right->wizardParams.insert("l", (s2));
    box_right->wizardParams.insert("b", (b));
    box_right->wizardParams.insert("a", (a));
    box_right->layer = this->layer;
    box_right->processWizardInput();
    box_right->calculate();

    QVector3D pos_up = (matrix_rotation * QVector3D(l/2.0, 0.0, a - s1/2.0)) + position;
    box_up->wizardParams.insert("Position x", (pos_up.x()));
    box_up->wizardParams.insert("Position y", (pos_up.y()));
    box_up->wizardParams.insert("Position z", (pos_up.z()));
    box_up->wizardParams.insert("Angle x", (angle_x));
    box_up->wizardParams.insert("Angle y", (angle_y));
    box_up->wizardParams.insert("Angle z", (angle_z));
    box_up->wizardParams.insert("l", (l - 2 * s2));
    box_up->wizardParams.insert("b", (b));
    box_up->wizardParams.insert("a", (s1));
    box_up->layer = this->layer;
    box_up->processWizardInput();
    box_up->calculate();

    qreal door_width = 0.1 * b;
    QVector3D position_door;
    QVector3D position_arc;
    if(alpha < 0.0)
    {
        QMatrix4x4 matrix_door;
        matrix_door.setToIdentity();
        matrix_door.rotate(alpha, 0.0, 0.0, 1.0);
        position_door = position + matrix_rotation * (QVector3D(s2, -b/2 , 0.0) + matrix_door * QVector3D(l/2 - s2, 0.0, (a-s1)/2));
        position_arc = position + matrix_rotation * QVector3D(s2, -b/2, 0.02 * a);

        //paint arrow tips
        QVector3D pos_start_1 = position + matrix_rotation * QVector3D(s2, 0.0, a - s1);
        QVector3D pos_end_1 = position + matrix_rotation * QVector3D(l - s2, 0.0,  0.5 * (a - s1));
        arrow_1->wizardParams.insert("Position x1", pos_start_1.x());
        arrow_1->wizardParams.insert("Position y1", pos_start_1.y());
        arrow_1->wizardParams.insert("Position z1", pos_start_1.z());
        arrow_1->wizardParams.insert("Position x2", pos_end_1.x());
        arrow_1->wizardParams.insert("Position y2", pos_end_1.y());
        arrow_1->wizardParams.insert("Position z2", pos_end_1.z());
        arrow_1->wizardParams.insert("Width", 1.0);
        arrow_1->processWizardInput();
        arrow_1->calculate();
        arrow_1->layer = this->layer;

        QVector3D pos_start_2 = position + matrix_rotation * QVector3D(s2, 0.0, 0.0);
        arrow_2->wizardParams.insert("Position x1", pos_start_2.x());
        arrow_2->wizardParams.insert("Position y1", pos_start_2.y());
        arrow_2->wizardParams.insert("Position z1", pos_start_2.z());
        arrow_2->wizardParams.insert("Position x2", pos_end_1.x());
        arrow_2->wizardParams.insert("Position y2", pos_end_1.y());
        arrow_2->wizardParams.insert("Position z2", pos_end_1.z());
        arrow_2->wizardParams.insert("Width", 1.0);
        arrow_2->processWizardInput();
        arrow_2->calculate();
        arrow_2->layer = this->layer;
    }
    else
    {
        QMatrix4x4 matrix_door;
        matrix_door.setToIdentity();
        matrix_door.rotate(alpha, 0.0, 0.0, 1.0);
        position_door = position + matrix_rotation * (QVector3D(l - s2, -b/2, 0.0) + matrix_door * QVector3D(-l/2 + s2, 0.0, (a-s1)/2));
        position_arc = position + matrix_rotation * QVector3D(l - s2, -b/2, 0.02 * a);

        //paint arrow tips
        QVector3D pos_start_1 = position + matrix_rotation * QVector3D(l - s2, 0.0, a - s1);
        QVector3D pos_end_1 = position + matrix_rotation * QVector3D(s2, 0.0,  0.5 * (a - s1));
        arrow_1->wizardParams.insert("Position x1", pos_start_1.x());
        arrow_1->wizardParams.insert("Position y1", pos_start_1.y());
        arrow_1->wizardParams.insert("Position z1", pos_start_1.z());
        arrow_1->wizardParams.insert("Position x2", pos_end_1.x());
        arrow_1->wizardParams.insert("Position y2", pos_end_1.y());
        arrow_1->wizardParams.insert("Position z2", pos_end_1.z());
        arrow_1->wizardParams.insert("Width", 1.0);
        arrow_1->processWizardInput();
        arrow_1->calculate();
        arrow_1->layer = this->layer;

        QVector3D pos_start_2 = position + matrix_rotation * QVector3D(l - s2, 0.0, 0.0);
        arrow_2->wizardParams.insert("Position x1", pos_start_2.x());
        arrow_2->wizardParams.insert("Position y1", pos_start_2.y());
        arrow_2->wizardParams.insert("Position z1", pos_start_2.z());
        arrow_2->wizardParams.insert("Position x2", pos_end_1.x());
        arrow_2->wizardParams.insert("Position y2", pos_end_1.y());
        arrow_2->wizardParams.insert("Position z2", pos_end_1.z());
        arrow_2->wizardParams.insert("Width", 1.0);
        arrow_2->processWizardInput();
        arrow_2->calculate();
        arrow_2->layer = this->layer;
    }
    door->wizardParams.insert("Position x", position_door.x());
    door->wizardParams.insert("Position y", position_door.y());
    door->wizardParams.insert("Position z", position_door.z());
    door->wizardParams.insert("Angle x", angle_x);
    door->wizardParams.insert("Angle y", angle_y);
    door->wizardParams.insert("Angle z", angle_z + alpha);

    door->wizardParams.insert("l", l - 2*s2);
    door->wizardParams.insert("b", door_width);
    door->wizardParams.insert("a", a - s1);
    door->layer = this->layer;
    door->layer = this->layer;
    door->processWizardInput();
    door->calculate();

    arc->wizardParams.insert("Position x", position_arc.x());
    arc->wizardParams.insert("Position y", position_arc.y());
    arc->wizardParams.insert("Position z", position_arc.z());
    arc->wizardParams.insert("Angle x", angle_x);
    arc->wizardParams.insert("Angle y", angle_y);
    arc->wizardParams.insert("Angle z", angle_z);
    arc->wizardParams.insert("r", l - 2*s2);
    arc->wizardParams.insert("alpha", alpha);
    arc->layer = this->layer;
    arc->processWizardInput();
    if(alpha < 0.0)
        arc->rotateAroundAxis(-90 + alpha, QVector3D(0.0, 0.0, 1.0), angle_x, angle_y, angle_z);
    else
        arc->rotateAroundAxis(-270.0 + alpha, QVector3D(0.0, 0.0, 1.0), angle_x, angle_y, angle_z);
    arc->calculate();

    this->boundingBox.enterVertices(box_left->boundingBox.getVertices());
    this->boundingBox.enterVertices(box_right->boundingBox.getVertices());
    this->boundingBox.enterVertices(box_up->boundingBox.getVertices());
    this->boundingBox.enterVertices(door->boundingBox.getVertices());

    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, a));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(0.0, 0.0, a));

    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.0, -b/2, 0.0));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(l  , -b/2, 0.0));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(l  ,  b/2, 0.0));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.0,  b/2, 0.0));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.0, -b/2, a));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(l  , -b/2, a));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(l  ,  b/2, a));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.0,  b/2, a));
}

void CAD_arch_door::processWizardInput()
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
    s1 = wizardParams.value("s1").toDouble();
    s2 = wizardParams.value("s2").toDouble();
    alpha = wizardParams.value("alpha").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

QMatrix4x4 CAD_arch_door::rotationOfFlange(quint8 num)
{
    if (num == 1 || num == 3)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(180.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }

    else if (num == 2 || num == 4)
    {
        return matrix_rotation;
    }
}
