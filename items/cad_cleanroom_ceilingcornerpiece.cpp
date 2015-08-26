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

#include "cad_cleanroom_ceilingcornerpiece.h"
#include "glwidget.h"

CAD_Cleanroom_CeilingCornerPiece::CAD_Cleanroom_CeilingCornerPiece() : CADitem(CADitemTypes::Cleanroom_CeilingCornerPiece)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",  50.0);
    wizardParams.insert("b",  20.0);
    wizardParams.insert("l",  70.0);

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

CAD_Cleanroom_CeilingCornerPiece::~CAD_Cleanroom_CeilingCornerPiece()
{
    arrayBufVertices->destroy();
    indexBufFaces->destroy();
    indexBufLines->destroy();
    delete arrayBufVertices;
    delete indexBufFaces;
    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Cleanroom_CeilingCornerPiece::flangable_items(int flangeIndex)
{
    QList<CADitemTypes::ItemType> flangable_items;
    if(flangeIndex == 3)
    {
        flangable_items.append(CADitemTypes::Cleanroom_CeilingGrating);
        flangable_items.append(CADitemTypes::Cleanroom_CeilingPanel);
    }
    else
    {
        flangable_items.append(CADitemTypes::Cleanroom_CeilingCornerPiece);
        flangable_items.append(CADitemTypes::Cleanroom_CeilingJoiningKnot);
        flangable_items.append(CADitemTypes::Cleanroom_CeilingMountingRails);
        flangable_items.append(CADitemTypes::Cleanroom_CeilingTeeJoiningPiece);
    }
    return flangable_items;
}

QImage CAD_Cleanroom_CeilingCornerPiece::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_Cleanroom_CeilingCornerPiece::iconPath()
{
    return ":/icons/cad_cleanroom/cad_cleanroom_ceilingcornerpiece.svg";
}

QString CAD_Cleanroom_CeilingCornerPiece::domain()
{
    return "Cleanroom";
}

QString CAD_Cleanroom_CeilingCornerPiece::description()
{
    return "Cleanroom|Ceiling Corner Piece";
}

void CAD_Cleanroom_CeilingCornerPiece::calculate()
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
    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l - b/2, -l + b/2, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l - b, -b/2, 0.0));

    QVector3D vertices[12];
    vertices[0]  = position + matrix_rotation * QVector3D(0.0,   -b/2,     0.0);
    vertices[1]  = position + matrix_rotation * QVector3D(l - b, -b/2,     0.0);
    vertices[2]  = position + matrix_rotation * QVector3D(l - b, -l + b/2, 0.0);
    vertices[3]  = position + matrix_rotation * QVector3D(l    , -l + b/2, 0.0);
    vertices[4]  = position + matrix_rotation * QVector3D(l    ,  b/2,     0.0);
    vertices[5]  = position + matrix_rotation * QVector3D(0.0,    b/2,     0.0);
    vertices[6]  = position + matrix_rotation * QVector3D(0.0,   -b/2,     a);
    vertices[7]  = position + matrix_rotation * QVector3D(l - b, -b/2,     a);
    vertices[8]  = position + matrix_rotation * QVector3D(l - b, -l + b/2, a);
    vertices[9]  = position + matrix_rotation * QVector3D(l    , -l + b/2, a);
    vertices[10] = position + matrix_rotation * QVector3D(l    ,  b/2,     a);
    vertices[11] = position + matrix_rotation * QVector3D(0.0,    b/2,     a);

    for(int i = 0; i < 12; i++)
    {
        this->boundingBox.enterVertex(vertices[i]);
    }

    GLushort indicesFaces[] =
    {
        3, 2, 4, 1, 5, 0, 0xABCD,

        0, 6, 5, 11, 4, 10, 3, 9, 2, 8, 1, 7, 0, 6, 0xABCD,

        11, 6, 10, 7, 9, 8, 0xABCD
    };

    GLushort indicesLines[] =
    {
        0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 0,
        6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 6,
        0, 6, 1, 7, 2, 8, 3, 9, 4, 10, 5, 11
    };

    arrayBufVertices->bind();
    arrayBufVertices->allocate(vertices, sizeof(vertices));
    arrayBufVertices->release();

    indexBufFaces->bind();
    indexBufFaces->allocate(indicesFaces, sizeof(indicesFaces));
    indexBufFaces->release();

    indexBufLines->bind();
    indexBufLines->allocate(indicesLines, sizeof(indicesLines));
    indexBufLines->release();
}

void CAD_Cleanroom_CeilingCornerPiece::processWizardInput()
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

void CAD_Cleanroom_CeilingCornerPiece::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Cleanroom_CeilingCornerPiece::rotationOfFlange(quint8 num)
{
    if(num == 1)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(180.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else if(num == 2)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(-90.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else if(num == 3)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(180.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else
        return matrix_rotation;
}
