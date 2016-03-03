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

#include "cad_basic_torus.h"

#include "glwidget.h"

CAD_Basic_Torus::CAD_Basic_Torus() : CADitem(CADitemTypes::Basic_Torus)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("r1", 1000.0);
    wizardParams.insert("r2",  100.0);

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

CAD_Basic_Torus::~CAD_Basic_Torus()
{
    arrayBufVertices->destroy();
    indexBufFaces->destroy();
    indexBufLines->destroy();
    delete arrayBufVertices;
    delete indexBufFaces;
    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Basic_Torus::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Basic_Torus::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_Basic_Torus::iconPath()
{
    return ":/icons/cad_basic/cad_basic_torus.svg";
}

QString CAD_Basic_Torus::domain()
{
    return "Basic";
}

QString CAD_Basic_Torus::description()
{
    return "Basic|Torus";
}

void CAD_Basic_Torus::calculate()
{
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    QVector3D vertices[110];
    for(int i = 0; i <= 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            qreal theta = i * PI * 0.2;
            qreal phi = j * PI * 0.2;
            vertices[i * 10 + j] = position + matrix_rotation * QVector3D((r1 + r2 * cos(theta)) * cos(phi), (r1 + r2 * cos(theta)) * sin(phi), r2 * sin(theta));
        }
    }

    static GLushort indicesFaces[410];
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            indicesFaces[41 * i + 4*j] = 10 * i + j + 10;
            indicesFaces[41 * i + 4*j + 1] = 10 * i + j;
            indicesFaces[41 * i + 4*j + 2] = 10 * i + j + 11;
            indicesFaces[41 * i + 4*j + 3] = 10 * i + j + 1;
        }
        indicesFaces[41 * i + 36] = 10 * i + 19;
        indicesFaces[41 * i + 37] = 10 * i + 9;
        indicesFaces[41 * i + 38] = 10 * i + 10;
        indicesFaces[41 * i + 39] = 10 * i + 0;
        indicesFaces[41 * i + 40] = 0xABCD;
    }

    static GLushort indicesLines[400];
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 9; j++){
            indicesLines[20*i + 2*j] = 10*i + j;
            indicesLines[20*i + 2*j + 1] = 10*i + j + 1;
        }
        indicesLines[20*i + 18] = 10*i + 9;
        indicesLines[20*i + 19] = 10*i;
    }

    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 9; j++){
            indicesLines[200 + 20*i + 2*j] = 10*i + j;
            indicesLines[200 + 20*i + 2*j + 1] = 10*i + j + 10;
        }
        indicesLines[200 + 20*i + 18] = 10*i + 19;
        indicesLines[200 + 20*i + 19] = 10*i + 9;
    }



    arrayBufVertices->bind();
    arrayBufVertices->allocate(vertices, sizeof(vertices));

    indexBufFaces->bind();
    indexBufFaces->allocate(indicesFaces, sizeof(indicesFaces));

    indexBufLines->bind();
    indexBufLines->allocate(indicesLines, sizeof(indicesLines));

    boundingBox.enterVertex(position + matrix_rotation * QVector3D(-r1 - r2, 0.0, 0.0));
    boundingBox.enterVertex(position + matrix_rotation * QVector3D( r1 + r2, 0.0, 0.0));
    boundingBox.enterVertex(position + matrix_rotation * QVector3D(0.0, -r1 - r2, 0.0));
    boundingBox.enterVertex(position + matrix_rotation * QVector3D(0.0,  r1 + r2, 0.0));
    boundingBox.enterVertex(position + matrix_rotation * QVector3D(0.0, 0.0,  r2));
    boundingBox.enterVertex(position + matrix_rotation * QVector3D(0.0, 0.0, -r2));
}

void CAD_Basic_Torus::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    r1 = wizardParams.value("r1").toDouble();
    r2 = wizardParams.value("r2").toDouble();


    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

void CAD_Basic_Torus::paint(GLWidget *glwidget)
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

        indexBufLines->bind();
        glwidget->glDrawElements(GL_LINES, indexBufLines->size(), GL_UNSIGNED_SHORT, 0);
        indexBufLines->release();
    }

    arrayBufVertices->release();
}

QMatrix4x4 CAD_Basic_Torus::rotationOfFlange(quint8 num)
{
    Q_UNUSED(num)
    return matrix_rotation;
}
