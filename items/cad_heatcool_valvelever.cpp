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

#include "cad_heatcool_valvelever.h"
#include "glwidget.h"

CAD_HeatCool_ValveLever::CAD_HeatCool_ValveLever() : CADitem(CADitemTypes::HeatCool_ValveLever)
{
    box_1 = new CAD_basic_box;
    box_2 = new CAD_basic_box;
    this->subItems.append(box_1);
    this->subItems.append(box_2);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a1", 70.0);
    wizardParams.insert("a2", 60.0);
    wizardParams.insert("l1", 70.0);
    wizardParams.insert("l2", 20.0);

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

CAD_HeatCool_ValveLever::~CAD_HeatCool_ValveLever()
{
//    arrayBufVertices.destroy();
//    indexBufFaces.destroy();
//    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_HeatCool_ValveLever::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::HeatCool_Valve);
    return flangable_items;
}

QImage CAD_HeatCool_ValveLever::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_HeatCool_ValveLever::iconPath()
{
    return ":/icons/cad_heatcool/cad_heatcool_valvelever.svg";
}

QString CAD_HeatCool_ValveLever::domain()
{
    return "HeatCool";
}

QString CAD_HeatCool_ValveLever::description()
{
    return "HeatCool|Valve Lever";
}

void CAD_HeatCool_ValveLever::calculate()
{                
    boundingBox.reset();
                    
    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();
                                
    this->snap_basepoint = (position);

    QVector3D position_b1 = position + matrix_rotation * QVector3D(0.0, 0.0, a2 / 2);
    box_1->wizardParams.insert("Position x", position_b1.x());
    box_1->wizardParams.insert("Position y", position_b1.y());
    box_1->wizardParams.insert("Position z", position_b1.z());
    box_1->wizardParams.insert("Angle x", angle_x);
    box_1->wizardParams.insert("Angle y", angle_y);
    box_1->wizardParams.insert("Angle z", angle_z);
    box_1->wizardParams.insert("l", l2);
    box_1->wizardParams.insert("b", l2);
    box_1->wizardParams.insert("a", a2);
    box_1->layer = this->layer;
    box_1->processWizardInput();
    box_1->calculate();

    QVector3D position_b2 = position + matrix_rotation * QVector3D((l1 - l2) / 2, 0.0, (a1 + a2) / 2);
    box_2->wizardParams.insert("Position x", position_b2.x());
    box_2->wizardParams.insert("Position y", position_b2.y());
    box_2->wizardParams.insert("Position z", position_b2.z());
    box_2->wizardParams.insert("Angle x", angle_x);
    box_2->wizardParams.insert("Angle y", angle_y);
    box_2->wizardParams.insert("Angle z", angle_z);
    box_2->wizardParams.insert("l", l1);
    box_2->wizardParams.insert("b", l2);
    box_2->wizardParams.insert("a", a1 - a2);
    box_2->layer = this->layer;
    box_2->processWizardInput();
    box_2->calculate();

    this->boundingBox = box_1->boundingBox;
    this->boundingBox.enterVertices(box_2->boundingBox.getVertices());
    this->snap_flanges.append(position);
}

void CAD_HeatCool_ValveLever::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a1 = wizardParams.value("a1").toDouble();
    a2 = wizardParams.value("a2").toDouble();
    l1 = wizardParams.value("l1").toDouble();
    l2 = wizardParams.value("l2").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

//void CAD_HeatCool_ValveLever::paint(GLWidget *glwidget)
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
