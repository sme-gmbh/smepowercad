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

#include "cad_basic_sphere.h"
//#include "itemdb.h"
#include "glwidget.h"
//#include <GL/glu.h>

CAD_basic_sphere::CAD_basic_sphere() : CADitem(CADitemTypes::Basic_Sphere)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("r", 100.0);

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

CAD_basic_sphere::~CAD_basic_sphere()
{

}

QList<CADitemTypes::ItemType> CAD_basic_sphere::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_basic_sphere::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_basic_sphere::iconPath()
{
    return ":/icons/cad_basic/cad_basic_sphere.svg";
}

QString CAD_basic_sphere::domain()
{
    return "Basic";
}

QString CAD_basic_sphere::description()
{
    return "Basic|Sphere";
}

void CAD_basic_sphere::calculate()
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

    QVector3D vertices[111];
    for(int i = 0; i < 11; i++)
    {
        for(int j = 0; j <= 10; j++)
        {
            qreal phi = i * PI / 10;
            qreal psi = j * PI / 5;
            vertices[10*i + j] = position + matrix_rotation * QVector3D(r * sin(phi) * cos(psi), r * sin(phi) * sin(psi), r * cos(phi));
        }
    }

    static GLushort indicesFaces[230];
    for(int j = 0; j < 10; j++)
    {
    for(int i = 0; i < 10; i++)
    {
        indicesFaces[23 * j + 2 * i] = 10 * j + i;
        indicesFaces[23 * j + 2 * i + 1] = 10 * j + i + 10;
    }
    indicesFaces[23 * j + 20] = 10 * j + 0;
    indicesFaces[23 * j + 21] = 10 * j + 10;
    indicesFaces[23 * j + 22] = 0xABCD;
    }

    GLushort indicesLines[400];
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            indicesLines[20*i+2*j] = 10*i+j;
            indicesLines[20*i+2*j+1] = 10*i+j+1;
        }
        indicesLines[20*i+18] = 10*i+9;
        indicesLines[20*i+19] = 10*i;
    }

    for(int j = 0; j < 10; j++)
    {
        for(int i = 0; i < 10; i++)
        {
            indicesLines[200 + 20 * j + 2*i] = 10 * j + i;
            indicesLines[201 + 20 * j + 2*i] = 10 * j + i + 10;
        }
    }


    arrayBufVertices->bind();
    arrayBufVertices->allocate(vertices, sizeof(vertices));

    indexBufFaces->bind();
    indexBufFaces->allocate(indicesFaces, sizeof(indicesFaces));

    indexBufLines->bind();
    indexBufLines->allocate(indicesLines, sizeof(indicesLines));

    boundingBox.enterVertex(position + QVector3D( r, 0, 0));
    boundingBox.enterVertex(position + QVector3D(-r, 0, 0));
    boundingBox.enterVertex(position + QVector3D(0,  r, 0));
    boundingBox.enterVertex(position + QVector3D(0, -r, 0));
    boundingBox.enterVertex(position + QVector3D(0, 0,  r));
    boundingBox.enterVertex(position + QVector3D(0, 0, -r));
}

void CAD_basic_sphere::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    r = wizardParams.value("r").toDouble();
}


void CAD_basic_sphere::paint(GLWidget *glwidget)
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


QMatrix4x4 CAD_basic_sphere::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}
