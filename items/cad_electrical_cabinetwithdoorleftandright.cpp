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

#include "cad_electrical_cabinetwithdoorleftandright.h"
#include "glwidget.h"

CAD_Electrical_CabinetWithDoorLeftAndRight::CAD_Electrical_CabinetWithDoorLeftAndRight() : CADitem(CADitemTypes::Electrical_CabinetWithDoorLeftAndRight)
{
    cabinet = new CAD_basic_duct;
    back = new CAD_basic_box;
    door_left = new CAD_basic_box;
    door_right = new CAD_basic_box;
    socket = new CAD_basic_box;
    this->subItems.append(cabinet);
    this->subItems.append(back);
    this->subItems.append(door_left);
    this->subItems.append(door_right);
    this->subItems.append(socket);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 1500.0);               // Höhe Schrank
    wizardParams.insert("a1", 300.0);               // Höhe Sockel
    wizardParams.insert("b", 500.0);                // Tiefe Sockel
    wizardParams.insert("l", 700.0);                // Breite
    wizardParams.insert("l2", 500.0);               // Breite der linken Tür
    wizardParams.insert("i", 100.0);                // Überhang Tür
    wizardParams.insert("s", 10.0);                 // Wandstärke
    wizardParams.insert("alpha left", 45.0);        // Öffnungswinkel links
    wizardParams.insert("alpha right", 45.0);       // Öffnungswinkel rechts


    processWizardInput();
    calculate();

    //    arrayBufVertices = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    //    arrayBufVertices.create();
    //    arrayBufVertices.setUsagePattern(QOpenGLBuffer::StaticDraw);

    //    indexBufFaces = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    //    indexBufFaces.create();
    //    indexBufFaces.setUsagePattern(QOpenGLBuffer::StaticDraw);

    //    indexBufLines = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    //    indexBufLines.create();
    //    indexBufLines.setUsagePattern(QOpenGLBuffer::StaticDraw);

    processWizardInput();
    calculate();
}

CAD_Electrical_CabinetWithDoorLeftAndRight::~CAD_Electrical_CabinetWithDoorLeftAndRight()
{
    //    arrayBufVertices.destroy();
    //    indexBufFaces.destroy();
    //    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_Electrical_CabinetWithDoorLeftAndRight::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Electrical_CabinetWithDoorLeftAndRight::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_Electrical_CabinetWithDoorLeftAndRight::iconPath()
{
    return ":/icons/cad_electrical/cad_electrical_cabinetwithdoorleftandright.svg";
}

QString CAD_Electrical_CabinetWithDoorLeftAndRight::domain()
{
    return "Electrical";
}

QString CAD_Electrical_CabinetWithDoorLeftAndRight::description()
{
    return "Electrical|Cabinet With Door Left And Right";
}

void CAD_Electrical_CabinetWithDoorLeftAndRight::calculate()
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

    QVector3D position_cab = position + matrix_rotation * QVector3D(l/2, b/2, a1 + a/2);
    cabinet->wizardParams.insert("Position x", position_cab.x());
    cabinet->wizardParams.insert("Position y", position_cab.y());
    cabinet->wizardParams.insert("Position z", position_cab.z());
    cabinet->wizardParams.insert("Angle x", angle_x);
    cabinet->wizardParams.insert("Angle y", angle_y);
    cabinet->wizardParams.insert("Angle z", angle_z-90.0);

    cabinet->wizardParams.insert("l", (b+i));
    cabinet->wizardParams.insert("b", (l));
    cabinet->wizardParams.insert("a", (a));
    cabinet->wizardParams.insert("s", (s));
    cabinet->layer = this->layer;
    cabinet->processWizardInput();
    cabinet->calculate();

    QVector3D position_sock = position + matrix_rotation * QVector3D(l/2, 0.0, a1/2);
    socket->wizardParams.insert("Position x", position_sock.x());
    socket->wizardParams.insert("Position y", position_sock.y());
    socket->wizardParams.insert("Position z", position_sock.z());
    socket->wizardParams.insert("Angle x", angle_x);
    socket->wizardParams.insert("Angle y", angle_y);
    socket->wizardParams.insert("Angle z", angle_z);

    socket->wizardParams.insert("Size x", l);
    socket->wizardParams.insert("Size y", b);
    socket->wizardParams.insert("Size z", a1);
    socket->layer = this->layer;
    socket->processWizardInput();
    socket->calculate();

    QVector3D position_back = position + matrix_rotation * QVector3D(l/2, b/2 - s/2, a1 + a/2);
    back->wizardParams.insert("Position x", position_back.x());
    back->wizardParams.insert("Position y", position_back.y());
    back->wizardParams.insert("Position z", position_back.z());
    back->wizardParams.insert("Angle x", angle_x);
    back->wizardParams.insert("Angle y", angle_y);
    back->wizardParams.insert("Angle z", angle_z);

    back->wizardParams.insert("Size x", l - 2*s);
    back->wizardParams.insert("Size y", s);
    back->wizardParams.insert("Size z", a - 2*s);
    back->layer = this->layer;
    back->processWizardInput();
    back->calculate();

    QMatrix4x4 matrix_door_left;
    matrix_door_left.setToIdentity();
    matrix_door_left.rotate(-alpha_left, 0.0, 0.0, 1.0);
    QVector3D position_door_left = position + matrix_rotation * (QVector3D(0.0, -b/2 - i, 0.0) + matrix_door_left * QVector3D(l2/2 + s, s/2, a1 + a/2));

    door_left->wizardParams.insert("Position x", position_door_left.x());
    door_left->wizardParams.insert("Position y", position_door_left.y());
    door_left->wizardParams.insert("Position z", position_door_left.z());
    door_left->wizardParams.insert("Angle x", angle_x);
    door_left->wizardParams.insert("Angle y", angle_y);
    door_left->wizardParams.insert("Angle z", angle_z - alpha_left);

    door_left->wizardParams.insert("Size x", l2);
    door_left->wizardParams.insert("Size y", s);
    door_left->wizardParams.insert("Size z", a - 2*s);
    door_left->layer = this->layer;
    door_left->processWizardInput();
    door_left->calculate();

    QMatrix4x4 matrix_door_right;
    matrix_door_right.setToIdentity();
    matrix_door_right.rotate(alpha_right, 0.0, 0.0, 1.0);
    QVector3D position_door_right = position + matrix_rotation * (QVector3D(l, -b/2 - i, 0.0) + matrix_door_right * QVector3D(-(l - l2) / 2, s/2, a1 + a/2));

    door_right->wizardParams.insert("Position x", position_door_right.x());
    door_right->wizardParams.insert("Position y", position_door_right.y());
    door_right->wizardParams.insert("Position z", position_door_right.z());
    door_right->wizardParams.insert("Angle x", angle_x);
    door_right->wizardParams.insert("Angle y", angle_y);
    door_right->wizardParams.insert("Angle z", angle_z + alpha_right);

    door_right->wizardParams.insert("Size x", l - l2 - 2 * s);
    door_right->wizardParams.insert("Size y", s);
    door_right->wizardParams.insert("Size z", a - 2*s);
    door_right->layer = this->layer;
    door_right->processWizardInput();
    door_right->calculate();

    this->boundingBox.enterVertices(cabinet->boundingBox.getVertices());
    this->boundingBox.enterVertices(door_left->boundingBox.getVertices());
    this->boundingBox.enterVertices(door_right->boundingBox.getVertices());
    this->boundingBox.enterVertices(socket->boundingBox.getVertices());
    this->boundingBox.enterVertices(back->boundingBox.getVertices());
}

void CAD_Electrical_CabinetWithDoorLeftAndRight::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a = wizardParams.value("a").toDouble();
    a1 = wizardParams.value("a1").toDouble();
    b = wizardParams.value("b").toDouble();
    l = wizardParams.value("l").toDouble();
    l2 = wizardParams.value("l2").toDouble();
    i = wizardParams.value("i").toDouble();
    s = wizardParams.value("s").toDouble();
    alpha_left = wizardParams.value("alpha left").toDouble();
    alpha_right = wizardParams.value("alpha right").toDouble();
}

//void CAD_Electrical_CabinetWithDoorLeftAndRight::paint(GLWidget *glwidget)
//{
//    QColor color_pen_tmp = getColorPen();
//    QColor color_brush_tmp = getColorBrush();

//    arrayBufVertices.bind();
//    glwidget->shaderProgram->enableAttributeArray(glwidget->shader_vertexLocation);
//    glwidget->shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

//    if (glwidget->render_solid)
//    {
//        glwidget->setPaintingColor(color_brush_tmp);

//        indexBufFaces.bind();
//        glwidget->glDrawElements(GL_TRIANGLE_STRIP, indexBufFaces.size(), GL_UNSIGNED_SHORT, 0);

//        indexBufFaces.release();
//    }

//    if (glwidget->render_outline)
//    {
//        glwidget->setPaintingColor(color_pen_tmp);
//        glwidget->glLineWidth(1.0);

//        indexBufLines.bind();
//        glwidget->glDrawElements(GL_LINES, indexBufLines.size(), GL_UNSIGNED_SHORT, 0);
//        indexBufLines.release();
//     }                          

//     arrayBufVertices.release();
//}
