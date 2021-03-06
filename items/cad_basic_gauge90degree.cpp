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

#include "cad_basic_gauge90degree.h"

#include "glwidget.h"

CAD_Basic_Gauge90Degree::CAD_Basic_Gauge90Degree() : CADitem(CADitemTypes::Basic_Gauge90Degree)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("l", 25.0);
    wizardParams.insert("d", 100.0);
    wizardParams.insert("l2", 250.0);

    gauge = new CAD_basic_pipe();
    probe = new CAD_basic_box();
    this->subItems.append(gauge);
    this->subItems.append(probe);

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

CAD_Basic_Gauge90Degree::~CAD_Basic_Gauge90Degree()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Basic_Gauge90Degree::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Basic_Gauge90Degree::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");       
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Basic_Gauge90Degree::iconPath()
{
    return ":/icons/cad_basic/cad_basic_gauge90degree.svg";
}

QString CAD_Basic_Gauge90Degree::domain()
{
    return "Basic";
}

QString CAD_Basic_Gauge90Degree::description()
{
    return "Basic|Gauge 90 Degree";
}

void CAD_Basic_Gauge90Degree::calculate()
{                
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    QVector3D position_gauge = position + matrix_rotation * QVector3D(0.0, 0.0, d/2);
    gauge->wizardParams.insert("Position x", position_gauge.x());
    gauge->wizardParams.insert("Position y", position_gauge.y());
    gauge->wizardParams.insert("Position z", position_gauge.z());
    gauge->wizardParams.insert("Angle x", angle_x);
    gauge->wizardParams.insert("Angle y", angle_y);
    gauge->wizardParams.insert("Angle z", angle_z);
    gauge->wizardParams.insert("l", l);
    gauge->wizardParams.insert("d", d);
    gauge->wizardParams.insert("s", d/2);
    gauge->layer = this->layer;
    gauge->processWizardInput();
    gauge->calculate();

    QVector3D position_probe = position + matrix_rotation * QVector3D(-l2/2, 0.0, d/2);
    probe->wizardParams.insert("Position x", position_probe.x());
    probe->wizardParams.insert("Position y", position_probe.y());
    probe->wizardParams.insert("Position z", position_probe.z());
    probe->wizardParams.insert("Angle x", angle_x);
    probe->wizardParams.insert("Angle y", angle_y);
    probe->wizardParams.insert("Angle z", angle_z);
    probe->wizardParams.insert("l", l2);
    probe->wizardParams.insert("b", d/10);
    probe->wizardParams.insert("a", d/10);
    probe->layer = this->layer;
    probe->processWizardInput();
    probe->calculate();

    this->boundingBox = gauge->boundingBox;
    this->boundingBox.enterVertices(probe->boundingBox.getVertices());
}

void CAD_Basic_Gauge90Degree::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    l = wizardParams.value("l").toDouble();
    d = wizardParams.value("d").toDouble();
    l2 = wizardParams.value("l2").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

//void CAD_Basic_Gauge90Degree::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Basic_Gauge90Degree::rotationOfFlange(quint8 num)
{
    Q_UNUSED(num)
    return matrix_rotation;
}
