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

#include "cad_cleanroom_ceilingfilterfanunit.h"

CAD_Cleanroom_CeilingFilterFanUnit::CAD_Cleanroom_CeilingFilterFanUnit() : CADitem(CADitemTypes::Cleanroom_CeilingFilterFanUnit)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("g",  600.0);
    wizardParams.insert("l",  600.0);
    wizardParams.insert("h",  100.0);
    wizardParams.insert("b", 200.0);
    wizardParams.insert("a", 400.0);
    wizardParams.insert("h2", 100.0);

    lower = new CAD_basic_box();
    upper = new CAD_basic_box();
    this->subItems.append(lower);
    this->subItems.append(upper);

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

CAD_Cleanroom_CeilingFilterFanUnit::~CAD_Cleanroom_CeilingFilterFanUnit()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Cleanroom_CeilingFilterFanUnit::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Air_HeatExchangerWaterAir);
    return flangable_items;
}

QImage CAD_Cleanroom_CeilingFilterFanUnit::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");       
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Cleanroom_CeilingFilterFanUnit::iconPath()
{
    return ":/icons/cad_cleanroom/cad_cleanroom_ceilingfilterfanunit.svg";
}

QString CAD_Cleanroom_CeilingFilterFanUnit::domain()
{
    return "Cleanroom";
}

QString CAD_Cleanroom_CeilingFilterFanUnit::description()
{
    return "Cleanroom|Ceiling Filter Fan Unit";
}

void CAD_Cleanroom_CeilingFilterFanUnit::calculate()
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

    this->snap_flanges.append(position + matrix_rotation * QVector3D(l/2, g/2, h + h2));

    QVector3D position_lower = position + matrix_rotation * QVector3D(l/2, g/2, h/2);
    lower->wizardParams.insert("Position x", (position_lower.x()));
    lower->wizardParams.insert("Position y", (position_lower.y()));
    lower->wizardParams.insert("Position z", (position_lower.z()));
    lower->wizardParams.insert("Angle x", (angle_x));
    lower->wizardParams.insert("Angle y", (angle_y));
    lower->wizardParams.insert("Angle z", (angle_z));
    lower->wizardParams.insert("l", (l));
    lower->wizardParams.insert("b", (g));
    lower->wizardParams.insert("a", (h));
    lower->layer = this->layer;
    lower->processWizardInput();
    lower->calculate();

    QVector3D position_upper = position + matrix_rotation * QVector3D(l/2, g/2, h + h2/2);
    upper->wizardParams.insert("Position x", (position_upper.x()));
    upper->wizardParams.insert("Position y", (position_upper.y()));
    upper->wizardParams.insert("Position z", (position_upper.z()));
    upper->wizardParams.insert("Angle x", (angle_x));
    upper->wizardParams.insert("Angle y", (angle_y));
    upper->wizardParams.insert("Angle z", (angle_z));
    upper->wizardParams.insert("l", (a));
    upper->wizardParams.insert("b", (b));
    upper->wizardParams.insert("a", (h2));
    upper->layer = this->layer;
    upper->processWizardInput();
    upper->calculate();

    this->boundingBox.enterVertices(upper->boundingBox.getVertices());
    this->boundingBox.enterVertices(lower->boundingBox.getVertices());
}

void CAD_Cleanroom_CeilingFilterFanUnit::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    g = wizardParams.value("g").toDouble();
    h = wizardParams.value("h").toDouble();
    l = wizardParams.value("l").toDouble();
    b = wizardParams.value("b").toDouble();
    h2 = wizardParams.value("h2").toDouble();
    a = wizardParams.value("a").toDouble();
}

//void CAD_cleanroom_CeilingFilterFanUnit::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Cleanroom_CeilingFilterFanUnit::rotationOfFlange(quint8 num)
{
    Q_UNUSED(num);
    QMatrix4x4 m;
    m.setToIdentity();
    m.rotate(-90.0, 0.0, 1.0, 0.0);
    return matrix_rotation * m;
}
