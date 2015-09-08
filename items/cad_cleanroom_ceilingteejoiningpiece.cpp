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

#include "cad_cleanroom_ceilingteejoiningpiece.h"
#include "glwidget.h"

CAD_Cleanroom_CeilingTeeJoiningPiece::CAD_Cleanroom_CeilingTeeJoiningPiece() : CADitem(CADitemTypes::Cleanroom_CeilingTeeJoiningPiece)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",  50.0);
    wizardParams.insert("b",  20.0);
    wizardParams.insert("l", 120.0);

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

CAD_Cleanroom_CeilingTeeJoiningPiece::~CAD_Cleanroom_CeilingTeeJoiningPiece()
{
    arrayBufVertices->destroy();
    indexBufFaces->destroy();
    indexBufLines->destroy();
    delete arrayBufVertices;
    delete indexBufFaces;
    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Cleanroom_CeilingTeeJoiningPiece::flangable_items(int flangeIndex)
{
    QList<CADitemTypes::ItemType> flangable_items;
    if(flangeIndex <= 3)
    {
        flangable_items.append(CADitemTypes::Cleanroom_CeilingCornerPiece);
        flangable_items.append(CADitemTypes::Cleanroom_CeilingJoiningKnot);
        flangable_items.append(CADitemTypes::Cleanroom_CeilingGrid);
        flangable_items.append(CADitemTypes::Cleanroom_CeilingGridFeedThrough);
        flangable_items.append(CADitemTypes::Cleanroom_CeilingTeeJoiningPiece);
    }
    else
    {
        flangable_items.append(CADitemTypes::Cleanroom_CeilingGrating);
        flangable_items.append(CADitemTypes::Cleanroom_CeilingPanel);
        flangable_items.append(CADitemTypes::Cleanroom_CeilingFilterFanUnit);
        flangable_items.append(CADitemTypes::Cleanroom_CeilingMaintenanceFlap);
        flangable_items.append(CADitemTypes::Cleanroom_CeilingSmokeExtractFlap);
    }
    return flangable_items;
}

QImage CAD_Cleanroom_CeilingTeeJoiningPiece::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Cleanroom_CeilingTeeJoiningPiece::iconPath()
{
    return ":/icons/cad_cleanroom/cad_cleanroom_ceilingteejoiningpiece.svg";
}

QString CAD_Cleanroom_CeilingTeeJoiningPiece::domain()
{
    return "Cleanroom";
}

QString CAD_Cleanroom_CeilingTeeJoiningPiece::description()
{
    return "Cleanroom|Ceiling Tee Joining Piece";
}

void CAD_Cleanroom_CeilingTeeJoiningPiece::calculate()
{
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);
    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l/2, -l/2, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l  ,  0.0, 0.0));

    this->snap_flanges.append(position + matrix_rotation * QVector3D((l - b)/ 2, -b/2, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D((l + b)/ 2, -b/2, 0.0));


    QVector3D vertices[16];
    vertices[0]  = position + matrix_rotation * QVector3D(0.0,          -b/2,     0.0);
    vertices[1]  = position + matrix_rotation * QVector3D((l - b)/2,    -b/2,     0.0);
    vertices[2]  = position + matrix_rotation * QVector3D((l - b)/2,    -l/2,     0.0);
    vertices[3]  = position + matrix_rotation * QVector3D((l + b)/2,    -l/2,     0.0);
    vertices[4]  = position + matrix_rotation * QVector3D((l + b)/2,    -b/2,     0.0);
    vertices[5]  = position + matrix_rotation * QVector3D(l,            -b/2,     0.0);
    vertices[6]  = position + matrix_rotation * QVector3D(l,             b/2,     0.0);
    vertices[7]  = position + matrix_rotation * QVector3D(0.0,           b/2,     0.0);


    vertices[8]  = position + matrix_rotation * QVector3D(0.0,          -b/2,      a);
    vertices[9]  = position + matrix_rotation * QVector3D((l - b)/2,    -b/2,      a);
    vertices[10]  = position + matrix_rotation * QVector3D((l - b)/2,    -l/2,     a);
    vertices[11]  = position + matrix_rotation * QVector3D((l + b)/2,    -l/2,     a);
    vertices[12]  = position + matrix_rotation * QVector3D((l + b)/2,    -b/2,     a);
    vertices[13]  = position + matrix_rotation * QVector3D(l,            -b/2,     a);
    vertices[14]  = position + matrix_rotation * QVector3D(l,             b/2,     a);
    vertices[15]  = position + matrix_rotation * QVector3D(0.0,           b/2,     a);


    for(int i = 0; i < 16; i++)
    {
        this->boundingBox.enterVertex(vertices[i]);
    }

    GLushort indicesFaces[] =
    {
        0, 7, 5, 6, 0xABCD,
        2, 1, 3, 4, 0xABCD,

        15, 8, 14, 13, 0xABCD,
        9, 10, 12, 11, 0xABCD,

        8, 0 ,9, 1, 10, 2, 11, 3, 12, 4, 13, 5, 14, 6, 15, 7, 8, 0, 0xABCD
    };

    GLushort indicesLines[48];
    for(int i = 0; i < 7; i++)
    {
        indicesLines[2*i] = i;
        indicesLines[2*i + 1] = i + 1;
    }
    for(int i = 0; i < 7; i++)
    {
        indicesLines[14 + 2*i] = i + 8;
        indicesLines[15 + 2*i] = i + 9;
    }
    indicesLines[28] = 7;
    indicesLines[29] = 0;
    indicesLines[30] = 15;
    indicesLines[31] = 8;

    for(int i = 0; i < 8; i++)
    {
        indicesLines[32 + 2*i] = i;
        indicesLines[33 + 2*i] = i + 8;
    }

    arrayBufVertices->bind();
    arrayBufVertices->allocate(vertices, sizeof(vertices));

    indexBufFaces->bind();
    indexBufFaces->allocate(indicesFaces, sizeof(indicesFaces));

    indexBufLines->bind();
    indexBufLines->allocate(indicesLines, sizeof(indicesLines));
}

void CAD_Cleanroom_CeilingTeeJoiningPiece::processWizardInput()
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

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

void CAD_Cleanroom_CeilingTeeJoiningPiece::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Cleanroom_CeilingTeeJoiningPiece::rotationOfFlange(quint8 num)
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
    else if(num == 4)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(180.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else if(num == 5)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(-90.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else
        return matrix_rotation;
}
