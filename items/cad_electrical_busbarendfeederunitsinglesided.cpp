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

#include "cad_electrical_busbarendfeederunitsinglesided.h"
#include "glwidget.h"

CAD_Electrical_BusbarEndFeederUnitSingleSided::CAD_Electrical_BusbarEndFeederUnitSingleSided() : CADitem(CADitemTypes::Electrical_BusbarEndFeederUnitSingleSided)
{
    feeder = new CAD_basic_box;
    plug = new CAD_basic_box;
    this->subItems.append(feeder);
    this->subItems.append(plug);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 300.0);    // Höhe
    wizardParams.insert("b", 300.0);    // Breite
    wizardParams.insert("l", 300.0);    // Länge
    wizardParams.insert("e", 100.0);    // Höhe des Anschlusses in z-Richtung
    wizardParams.insert("f", 100.0);    // Breite des Anschlusses in y-Richtung
    wizardParams.insert("u", 100.0);    // Abstand des Anschlusses in z-Richtung
    wizardParams.insert("v", 100.0);    // Abstand des Anschlusses in y-Richtung

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

CAD_Electrical_BusbarEndFeederUnitSingleSided::~CAD_Electrical_BusbarEndFeederUnitSingleSided()
{
//    arrayBufVertices.destroy();
//    indexBufFaces.destroy();
//    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_Electrical_BusbarEndFeederUnitSingleSided::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Electrical_BusbarEndFeederUnitSingleSided::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Electrical_BusbarEndFeederUnitSingleSided::iconPath()
{
    return ":/icons/cad_electrical/cad_electrical_busbarendfeederunitsinglesided.svg";
}

QString CAD_Electrical_BusbarEndFeederUnitSingleSided::domain()
{
    return "Electrical";
}

QString CAD_Electrical_BusbarEndFeederUnitSingleSided::description()
{
    return "Electrical|Busbar End Feeder Unit Single Sided";
}

void CAD_Electrical_BusbarEndFeederUnitSingleSided::calculate()
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

    QVector3D position_feeder = position + matrix_rotation * QVector3D(l/2, v - b/2, u - a/2);
    feeder->wizardParams.insert("Position x", position_feeder.x());
    feeder->wizardParams.insert("Position y", position_feeder.y());
    feeder->wizardParams.insert("Position z", position_feeder.z());
    feeder->wizardParams.insert("Angle x", angle_x);
    feeder->wizardParams.insert("Angle y", angle_y);
    feeder->wizardParams.insert("Angle z", angle_z);
    feeder->wizardParams.insert("Size x", l);
    feeder->wizardParams.insert("Size y", b);
    feeder->wizardParams.insert("Size z", a);
    feeder->layer = this->layer;
    feeder->processWizardInput();
    feeder->calculate();

    QVector3D position_plug = position + matrix_rotation * QVector3D(0.0, - e/2, 0.0);
    plug->wizardParams.insert("Position x", position_plug.x());
    plug->wizardParams.insert("Position y", position_plug.y());
    plug->wizardParams.insert("Position z", position_plug.z());
    plug->wizardParams.insert("Angle x", angle_x);
    plug->wizardParams.insert("Angle y", angle_y);
    plug->wizardParams.insert("Angle z", angle_z);
    plug->wizardParams.insert("Size x", 0.0);
    plug->wizardParams.insert("Size y", e);
    plug->wizardParams.insert("Size z", f);
    plug->layer = this->layer;
    plug->processWizardInput();
    plug->calculate();

    this->boundingBox = feeder->boundingBox;
    this->snap_flanges.append(position);
}

void CAD_Electrical_BusbarEndFeederUnitSingleSided::processWizardInput()
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
    e = wizardParams.value("e").toDouble();
    f = wizardParams.value("f").toDouble();
    u = wizardParams.value("u").toDouble();
    v = wizardParams.value("v").toDouble();
}

//void CAD_Electrical_BusbarEndFeederUnitSingleSided::paint(GLWidget *glwidget)
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
