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

#include "cad_sanitary_pipeendcap.h"
#include "glwidget.h"

CAD_Sanitary_PipeEndCap::CAD_Sanitary_PipeEndCap() : CADitem(CADitemTypes::Sanitary_PipeEndCap)
{
    flange = new CAD_basic_pipe;
    endcap = new CAD_Basic_TorisphericalHeadDIN28011;
    this->subItems.append(flange);
    this->subItems.append(endcap);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);
    wizardParams.insert("d", 150.0);
    wizardParams.insert("l", 100.0);
    wizardParams.insert("s",  10.0);
    wizardParams.insert("fe", 10.0);
    wizardParams.insert("ff", 10.0);

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

CAD_Sanitary_PipeEndCap::~CAD_Sanitary_PipeEndCap()
{
//    arrayBufVertices.destroy();
//    indexBufFaces.destroy();
//    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_Sanitary_PipeEndCap::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Sanitary_PipeEndCap::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Sanitary_PipeEndCap::iconPath()
{
    return ":/icons/cad_sanitary/cad_sanitary_pipeendcap.svg";
}

QString CAD_Sanitary_PipeEndCap::domain()
{
    return "Sanitary";
}

QString CAD_Sanitary_PipeEndCap::description()
{
    return "Sanitary|Pipe End Cap";
}

void CAD_Sanitary_PipeEndCap::calculate()
{                
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    flange->wizardParams.insert("Position x", position.x());
    flange->wizardParams.insert("Position y", position.x());
    flange->wizardParams.insert("Position z", position.x());
    flange->wizardParams.insert("Angle x", angle_x);
    flange->wizardParams.insert("Angle y", angle_y);
    flange->wizardParams.insert("Angle z", angle_z);
    flange->wizardParams.insert("l", fe);
    flange->wizardParams.insert("d", d + 2 * ff);
    flange->wizardParams.insert("s",  ff + s);
    flange->layer = this->layer;
    flange->processWizardInput();
    flange->calculate();

    endcap->wizardParams.insert("Position x", position.x());
    endcap->wizardParams.insert("Position y", position.x());
    endcap->wizardParams.insert("Position z", position.x());
    endcap->wizardParams.insert("Angle x", angle_x);
    endcap->wizardParams.insert("Angle y", angle_y);
    endcap->wizardParams.insert("Angle z", angle_z);
    endcap->wizardParams.insert("d", d);   // Durchmesser
    qreal h = l - 0.1937742252 * d; // h - (1-sqrt(0.65)) * d
    endcap->wizardParams.insert("h", h);     // Höhe
    endcap->layer = this->layer;
    endcap->processWizardInput();
    endcap->rotateAroundAxis(90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    endcap->calculate();

    foreach(CADitem *item, subItems)
        this->boundingBox.enterVertices(item->boundingBox.getVertices());
    this->snap_flanges.append(position);
}

void CAD_Sanitary_PipeEndCap::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    d = wizardParams.value("d").toDouble();
    l = wizardParams.value("l").toDouble();
    s = wizardParams.value("s").toDouble();
    fe = wizardParams.value("fe").toDouble();
    ff = wizardParams.value("ff").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

//void CAD_Sanitary_PipeEndCap::paint(GLWidget *glwidget)
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
