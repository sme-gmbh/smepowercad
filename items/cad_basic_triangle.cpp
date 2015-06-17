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

#include "cad_basic_triangle.h"
#include "glwidget.h"

CAD_Basic_Triangle::CAD_Basic_Triangle() : CADitem(CADitemTypes::Basic_Triangle)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);

    wizardParams.insert("Position x1", 0.0);
    wizardParams.insert("Position y1", 0.0);
    wizardParams.insert("Position z1", 0.0);

    wizardParams.insert("Position x2", 0.0);
    wizardParams.insert("Position y2", 0.0);
    wizardParams.insert("Position z2", 0.0);

    arrayBufVertices = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    arrayBufVertices->create();
    arrayBufVertices->setUsagePattern(QOpenGLBuffer::StaticDraw);

    indexBufFaces = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBufFaces->create();
    indexBufFaces->setUsagePattern(QOpenGLBuffer::StaticDraw);

    indexBufLines = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBufLines->create();
    indexBufLines->setUsagePattern(QOpenGLBuffer::StaticDraw);
   
    processWizardInput();
    calculate();
}

CAD_Basic_Triangle::~CAD_Basic_Triangle()
{
    arrayBufVertices->destroy();
    indexBufFaces->destroy();
    indexBufLines->destroy();
    delete arrayBufVertices;
    delete indexBufFaces;
    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Basic_Triangle::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Basic_Triangle::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Basic_Triangle::iconPath()
{
    return ":/icons/cad_basic/cad_basic_triangle.svg";
}

QString CAD_Basic_Triangle::domain()
{
    return "Basic";
}

QString CAD_Basic_Triangle::description()
{
    return "Basic|Triangle";
}

void CAD_Basic_Triangle::calculate()
{                
    boundingBox.reset();
                    
    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();
                                
    this->snap_basepoint = (position);
    this->snap_vertices.append(position);
    this->snap_vertices.append(position1);
    this->snap_vertices.append(position2);

    boundingBox.enterVertex(position);
    boundingBox.enterVertex(position1);
    boundingBox.enterVertex(position2);

    QVector3D vertices[] = {
        position, position1, position2
    };

    static GLushort indicesFaces[] = {
        0,1,2
    };

    static GLushort indicesLines[] = {
        0, 1,
        1, 2,
        2, 0
    };


    arrayBufVertices->bind();
    arrayBufVertices->allocate(vertices, sizeof(vertices));

    indexBufFaces->bind();
    indexBufFaces->allocate(indicesFaces, sizeof(indicesFaces));

    indexBufLines->bind();
    indexBufLines->allocate(indicesLines, sizeof(indicesLines));


}

void CAD_Basic_Triangle::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    position1.setX(wizardParams.value("Position x1").toDouble());
    position1.setY(wizardParams.value("Position y1").toDouble());
    position1.setZ(wizardParams.value("Position z1").toDouble());
    position2.setX(wizardParams.value("Position x2").toDouble());
    position2.setY(wizardParams.value("Position y2").toDouble());
    position2.setZ(wizardParams.value("Position z2").toDouble());

}

QMatrix4x4 CAD_Basic_Triangle::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}

void CAD_Basic_Triangle::paint(GLWidget *glwidget)
{
    QColor color_pen_tmp = getColorPen();
    QColor color_brush_tmp = getColorBrush();

    arrayBufVertices->bind();
    glwidget->shaderProgram->enableAttributeArray(glwidget->shader_vertexLocation);
    glwidget->shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

    if (glwidget->render_solid)
    {
        glwidget->setPaintingColor(color_brush_tmp);

        indexBufFaces->bind();
        glwidget->glDrawElements(GL_TRIANGLE_STRIP, indexBufFaces->size(), GL_UNSIGNED_SHORT, 0);

        indexBufFaces->release();
    }

    if (glwidget->render_outline)
    {
        glwidget->setPaintingColor(color_pen_tmp);
        glwidget->glLineWidth(1.0);
                                      
        indexBufLines->bind();
        glwidget->glDrawElements(GL_LINES, indexBufLines->size(), GL_UNSIGNED_SHORT, 0);
        indexBufLines->release();
     }
                                                                                           
     arrayBufVertices->release();
}
