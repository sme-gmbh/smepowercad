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

#include "cad_electrical_luminaireescapelighting.h"
#include "glwidget.h"

CAD_Electrical_LuminaireEscapeLighting::CAD_Electrical_LuminaireEscapeLighting() : CADitem(CADitemTypes::Electrical_LuminaireEscapeLighting)
{
    lamp = new CAD_basic_box;
    this->subItems.append(lamp);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 50.0);
    wizardParams.insert("b", 50.0);
    wizardParams.insert("l", 100.0);

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

CAD_Electrical_LuminaireEscapeLighting::~CAD_Electrical_LuminaireEscapeLighting()
{
//    arrayBufVertices.destroy();
//    indexBufFaces.destroy();
//    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_Electrical_LuminaireEscapeLighting::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Electrical_LuminaireEscapeLighting::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Electrical_LuminaireEscapeLighting::iconPath()
{
    return ":/icons/cad_electrical/cad_electrical_luminaireescapelighting.svg";
}

QString CAD_Electrical_LuminaireEscapeLighting::domain()
{
    return "Electrical";
}

QString CAD_Electrical_LuminaireEscapeLighting::description()
{
    return "Electrical|Luminaire Escape Lighting";
}

void CAD_Electrical_LuminaireEscapeLighting::calculate()
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

    QVector3D position_lamp = position + matrix_rotation * QVector3D(0.0, -b/2, -a/2);
    lamp->wizardParams.insert("Position x", position_lamp.x());
    lamp->wizardParams.insert("Position y", position_lamp.y());
    lamp->wizardParams.insert("Position z", position_lamp.z());
    lamp->wizardParams.insert("Angle x", angle_x);
    lamp->wizardParams.insert("Angle y", angle_y);
    lamp->wizardParams.insert("Angle z", angle_z);
    lamp->wizardParams.insert("l", l);
    lamp->wizardParams.insert("b", b);
    lamp->wizardParams.insert("a", a);
    lamp->layer = this->layer;
    lamp->processWizardInput();
    lamp->calculate();

    this->boundingBox.enterVertices(lamp->boundingBox.getVertices());

    this->snap_vertices.append(lamp->snap_vertices);
    this->snap_center.append(lamp->snap_center);
}

void CAD_Electrical_LuminaireEscapeLighting::processWizardInput()
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

//void CAD_Electrical_LuminaireEscapeLighting::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Electrical_LuminaireEscapeLighting::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}
