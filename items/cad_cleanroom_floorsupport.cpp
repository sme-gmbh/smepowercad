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

#include "cad_cleanroom_floorsupport.h"
#include "glwidget.h"

CAD_Cleanroom_FloorSupport::CAD_Cleanroom_FloorSupport() : CADitem(CADitemTypes::Cleanroom_FloorSupport)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 1000.0);
    wizardParams.insert("b",  100.0);
    wizardParams.insert("d",   50.0);

    bottom = new CAD_basic_box();
    top = new CAD_basic_box();
    support = new CAD_basic_box();
    this->subItems.append(bottom);
    this->subItems.append(top);
    this->subItems.append(support);

//    arrayBufVertices = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
//    arrayBufVertices->create();
//    arrayBufVertices->setUsagePattern(QOpenGLBuffer::StaticDraw);

//    indexBufFaces = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
//    indexBufFaces->create();
//    indexBufFaces->setUsagePattern(QOpenGLBuffer::StaticDraw);

//    indexBufLines = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
//    indexBufLines->create();
//    indexBufLines->setUsagePattern(QOpenGLBuffer::StaticDraw);
   
    processWizardInput();
    calculate();
}

CAD_Cleanroom_FloorSupport::~CAD_Cleanroom_FloorSupport()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Cleanroom_FloorSupport::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    if (flangeIndex == 2)
        flangable_items.append(CADitemTypes::Cleanroom_FloorStiffenerDiagonal);
    if (flangeIndex == 1)
        flangable_items.append(CADitemTypes::Cleanroom_FloorStiffenerHorizontal);
    return flangable_items;
}

QImage CAD_Cleanroom_FloorSupport::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");   
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Cleanroom_FloorSupport::iconPath()
{
    return ":/icons/cad_cleanroom/cad_cleanroom_floorsupport.svg";
}

QString CAD_Cleanroom_FloorSupport::domain()
{
    return "Cleanroom";
}

QString CAD_Cleanroom_FloorSupport::description()
{
    return "Cleanroom|Floor Support";
}

void CAD_Cleanroom_FloorSupport::calculate()
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

    QVector3D position_bottom = position + matrix_rotation * QVector3D(0.0, 0.0, 5.0);
    bottom->wizardParams.insert("Position x", position_bottom.x());
    bottom->wizardParams.insert("Position y", position_bottom.y());
    bottom->wizardParams.insert("Position z", position_bottom.z());
    bottom->wizardParams.insert("Angle x", angle_x);
    bottom->wizardParams.insert("Angle y", angle_y);
    bottom->wizardParams.insert("Angle z", angle_z);
    bottom->wizardParams.insert("l", b);
    bottom->wizardParams.insert("b", b);
    bottom->wizardParams.insert("a", 10.0);
    bottom->layer = this->layer;
    bottom->processWizardInput();
    bottom->calculate();

    QVector3D position_top = position + matrix_rotation * QVector3D(0.0, 0.0, a - 5.0);
    top->wizardParams.insert("Position x", position_top.x());
    top->wizardParams.insert("Position y", position_top.y());
    top->wizardParams.insert("Position z", position_top.z());
    top->wizardParams.insert("Angle x", angle_x);
    top->wizardParams.insert("Angle y", angle_y);
    top->wizardParams.insert("Angle z", angle_z);
    top->wizardParams.insert("l", b);
    top->wizardParams.insert("b", b);
    top->wizardParams.insert("a", 10.0);
    top->layer = this->layer;
    top->processWizardInput();
    top->calculate();

    QVector3D position_support = position + matrix_rotation * QVector3D(0.0, 0.0, a * 0.5);
    support->wizardParams.insert("Position x", position_support.x());
    support->wizardParams.insert("Position y", position_support.y());
    support->wizardParams.insert("Position z", position_support.z());
    support->wizardParams.insert("Angle x", angle_x);
    support->wizardParams.insert("Angle y", angle_y);
    support->wizardParams.insert("Angle z", angle_z);
    support->wizardParams.insert("l", d);
    support->wizardParams.insert("b", d);
    support->wizardParams.insert("a", a - 20.0);
    support->layer = this->layer;
    support->processWizardInput();
    support->calculate();

    this->boundingBox = top->boundingBox;
    this->boundingBox.enterVertices(bottom->boundingBox.getVertices());
    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(0.0, 0.0, a));
}

void CAD_Cleanroom_FloorSupport::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
    a = wizardParams.value("a").toDouble();
    b = wizardParams.value("b").toDouble();
    d = wizardParams.value("d").toDouble();
}

//void CAD_cleanroom_FloorSupport::paint(GLWidget *glwidget)
//{
//    QColor color_pen_tmp = getColorPen();
//    QColor color_brush_tmp = getColorBrush();

//    arrayBufVertices->bind();
//    glwidget->shaderProgram->enableAttributeArray(glwidget->shader_vertexLocation);
//    glwidget->shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

//    if (glwidget->render_solid)
//    {
//        glwidget->setPaintingColor(color_brush_tmp);

//        indexBufFaces->bind();
//        glwidget->glDrawElements(GL_TRIANGLE_STRIP, indexBufFaces->size(), GL_UNSIGNED_SHORT, 0);

//        indexBufFaces->release();
//    }

//    if (glwidget->render_outline)
//    {
//        glwidget->setPaintingColor(color_pen_tmp);
                                      
//        indexBufLines->bind();
//        glwidget->glDrawElements(GL_LINES, indexBufLines->size(), GL_UNSIGNED_SHORT, 0);
//        indexBufLines->release();
//     }                          
                                                                                           
//     arrayBufVertices->release();
//}

QMatrix4x4 CAD_Cleanroom_FloorSupport::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}
