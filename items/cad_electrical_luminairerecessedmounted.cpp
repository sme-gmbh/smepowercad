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

#include "cad_electrical_luminairerecessedmounted.h"
#include "glwidget.h"

CAD_Electrical_LuminaireRecessedMounted::CAD_Electrical_LuminaireRecessedMounted() : CADitem(CADitemTypes::Electrical_LuminaireRecessedMounted)
{
    recess = new CAD_basic_box;
    lamp = new CAD_basic_box;
    this->subItems.append(recess);
    this->subItems.append(lamp);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 50.0);
    wizardParams.insert("b", 350.0);
    wizardParams.insert("g", 300.0);
    wizardParams.insert("h", 50.0);
    wizardParams.insert("l", 1050.0);
    wizardParams.insert("l1", 1000.0);

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

CAD_Electrical_LuminaireRecessedMounted::~CAD_Electrical_LuminaireRecessedMounted()
{
//    arrayBufVertices.destroy();
//    indexBufFaces.destroy();
//    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_Electrical_LuminaireRecessedMounted::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Electrical_LuminaireRecessedMounted::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Electrical_LuminaireRecessedMounted::iconPath()
{
    return ":/icons/cad_electrical/cad_electrical_luminairerecessedmounted.svg";
}

QString CAD_Electrical_LuminaireRecessedMounted::domain()
{
    return "Electrical";
}

QString CAD_Electrical_LuminaireRecessedMounted::description()
{
    return "Electrical|Luminaire Recessed Mounted";
}

void CAD_Electrical_LuminaireRecessedMounted::calculate()
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

    QVector3D position_rec = position + matrix_rotation * QVector3D(l/2, 0.0, h/2);
    recess->wizardParams.insert("Position x", position_rec.x());
    recess->wizardParams.insert("Position y", position_rec.y());
    recess->wizardParams.insert("Position z", position_rec.z());
    recess->wizardParams.insert("Angle x", angle_x);
    recess->wizardParams.insert("Angle y", angle_y);
    recess->wizardParams.insert("Angle z", angle_z);
    recess->wizardParams.insert("l", l1);
    recess->wizardParams.insert("b", g);
    recess->wizardParams.insert("a", h);
    recess->layer = this->layer;
    recess->processWizardInput();
    recess->calculate();

    QVector3D position_lamp = position + matrix_rotation * QVector3D(l/2, 0.0, -a/2);
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

    this->boundingBox.enterVertices(recess->boundingBox.getVertices());
    this->boundingBox.enterVertices(lamp->boundingBox.getVertices());

    this->snap_vertices.append(position);
    this->snap_vertices.append(position + matrix_rotation * QVector3D(l, 0.0, 0.0));
    this->snap_vertices.append(recess->snap_vertices);
    this->snap_vertices.append(lamp->snap_vertices);
    this->snap_center.append(recess->snap_center);
    this->snap_center.append(lamp->snap_center);
}

void CAD_Electrical_LuminaireRecessedMounted::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a = wizardParams.value("a").toDouble();
    b = wizardParams.value("b").toDouble();
    g = wizardParams.value("g").toDouble();
    h = wizardParams.value("h").toDouble();
    l = wizardParams.value("l").toDouble();
    l1 = wizardParams.value("l1").toDouble();
}

//void CAD_Electrical_LuminaireRecessedMounted::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Electrical_LuminaireRecessedMounted::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}
