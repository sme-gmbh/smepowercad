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

#include "cad_basic_plane.h"
#include "itemdb.h"
#include "glwidget.h"

CAD_basic_plane::CAD_basic_plane() : CADitem(CADitemTypes::Basic_Plane)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 10.0);
    wizardParams.insert("b",  10.0);

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

CAD_basic_plane::~CAD_basic_plane()
{

}

QList<CADitemTypes::ItemType> CAD_basic_plane::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_basic_plane::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_basic_plane::iconPath()
{
    return ":/icons/cad_basic/cad_basic_plane.svg";
}

QString CAD_basic_plane::domain()
{
    return "Basic";
}

QString CAD_basic_plane::description()
{
    return "Basic|Plane";
}

void CAD_basic_plane::calculate()
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

    QVector3D vertices[] = {
        position + matrix_rotation * QVector3D(a, 0.0, 0.0),
        position + matrix_rotation * QVector3D(a, b, 0.0),
        position + matrix_rotation * QVector3D(0.0,  b, 0.0),
        position + matrix_rotation * QVector3D(0.0,  0.0, 0.0),
    };

    static GLushort indicesFaces[] = {
        2,3,1,0
    };

    static GLushort indicesLines[] = {
        0,1,1,2,2,3,3,0
    };


    arrayBufVertices->bind();
    arrayBufVertices->allocate(vertices, sizeof(vertices));

    indexBufFaces->bind();
    indexBufFaces->allocate(indicesFaces, sizeof(indicesFaces));

    indexBufLines->bind();
    indexBufLines->allocate(indicesLines, sizeof(indicesLines));

    boundingBox.enterVertex(vertices[0]);
    boundingBox.enterVertex(vertices[1]);
    boundingBox.enterVertex(vertices[2]);
    boundingBox.enterVertex(vertices[3]);

    this->snap_vertices.append(vertices[0]);
    this->snap_vertices.append(vertices[1]);
    this->snap_vertices.append(vertices[2]);
    this->snap_vertices.append(vertices[3]);
}

void CAD_basic_plane::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
    a = wizardParams.value("a").toDouble();
    b = wizardParams.value("b").toDouble();

}

void CAD_basic_plane::paint(GLWidget *glwidget)
{
    glwidget->glEnable(GL_PRIMITIVE_RESTART);
    glwidget->glPrimitiveRestartIndex(0xABCD);

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

        indexBufLines->bind();
        glwidget->glDrawElements(GL_LINES, indexBufLines->size(), GL_UNSIGNED_SHORT, 0);
        indexBufLines->release();
    }

    arrayBufVertices->release();
}

QMatrix4x4 CAD_basic_plane::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}
