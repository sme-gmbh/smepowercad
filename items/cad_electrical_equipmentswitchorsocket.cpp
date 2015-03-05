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

#include "cad_electrical_equipmentswitchorsocket.h"
#include "glwidget.h"

CAD_Electrical_EquipmentSwitchOrSocket::CAD_Electrical_EquipmentSwitchOrSocket() : CADitem(CADitemTypes::Electrical_EquipmentSwitchOrSocket)
{
    equipment = new CAD_basic_box();
    this->subItems.append(equipment);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);
    wizardParams.insert("x", 50.0);
    wizardParams.insert("y", 10.0);
    wizardParams.insert("z", 50.0);

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

CAD_Electrical_EquipmentSwitchOrSocket::~CAD_Electrical_EquipmentSwitchOrSocket()
{
//    arrayBufVertices.destroy();
//    indexBufFaces.destroy();
//    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_Electrical_EquipmentSwitchOrSocket::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Electrical_EquipmentSwitchOrSocket);
    return flangable_items;
}

QImage CAD_Electrical_EquipmentSwitchOrSocket::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Electrical_EquipmentSwitchOrSocket::iconPath()
{
    return ":/icons/cad_electrical/cad_electrical_equipmentswitchorsocket.svg";
}

QString CAD_Electrical_EquipmentSwitchOrSocket::domain()
{
    return "Electrical";
}

QString CAD_Electrical_EquipmentSwitchOrSocket::description()
{
    return "Electrical|Equipment Switch Or Socket";
}

void CAD_Electrical_EquipmentSwitchOrSocket::calculate()
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

    QVector3D position_equ = position + matrix_rotation * QVector3D(0.0, -y/2, -z/2);
    equipment->wizardParams.insert("Position x", position_equ.x());
    equipment->wizardParams.insert("Position y", position_equ.y());
    equipment->wizardParams.insert("Position z", position_equ.z());
    equipment->wizardParams.insert("Angle x", angle_x);
    equipment->wizardParams.insert("Angle y", angle_y);
    equipment->wizardParams.insert("Angle z", angle_z);

    equipment->wizardParams.insert("l", x);
    equipment->wizardParams.insert("b", y);
    equipment->wizardParams.insert("a", z);
    equipment->layer = this->layer;
    equipment->processWizardInput();
    equipment->calculate();

    this->boundingBox = equipment->boundingBox;
    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(x/2, 0.0, -z/2));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(0.0, 0.0, -z));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(-x/2, 0.0, -z/2));
}

void CAD_Electrical_EquipmentSwitchOrSocket::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
    x = wizardParams.value("x").toDouble();
    y = wizardParams.value("y").toDouble();
    z = wizardParams.value("z").toDouble();
}

//void CAD_Electrical_EquipmentSwitchOrSocket::paint(GLWidget *glwidget)
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
