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

#include "cad_cleanroom_ceilingjoiningknot.h"
#include "glwidget.h"

CAD_Cleanroom_CeilingJoiningKnot::CAD_Cleanroom_CeilingJoiningKnot() : CADitem(CADitemTypes::Cleanroom_CeilingJoiningKnot)
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

CAD_Cleanroom_CeilingJoiningKnot::~CAD_Cleanroom_CeilingJoiningKnot()
{
    arrayBufVertices->destroy();
    indexBufFaces->destroy();
    indexBufLines->destroy();
    delete arrayBufVertices;
    delete indexBufFaces;
    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Cleanroom_CeilingJoiningKnot::flangable_items(int flangeIndex)
{
    QList<CADitemTypes::ItemType> flangable_items;
    if(flangeIndex <= 4)
    {
    flangable_items.append(CADitemTypes::Cleanroom_CeilingCornerPiece);
    flangable_items.append(CADitemTypes::Cleanroom_CeilingJoiningKnot);
    flangable_items.append(CADitemTypes::Cleanroom_CeilingMountingRails);
    flangable_items.append(CADitemTypes::Cleanroom_CeilingTeeJoiningPiece);
    }
    else
    {
        flangable_items.append(CADitemTypes::Cleanroom_CeilingGrating);
        flangable_items.append(CADitemTypes::Cleanroom_CeilingPanel);
    }
    return flangable_items;
}

QImage CAD_Cleanroom_CeilingJoiningKnot::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Cleanroom_CeilingJoiningKnot::iconPath()
{
    return ":/icons/cad_cleanroom/cad_cleanroom_ceilingjoiningknot.svg";
}

QString CAD_Cleanroom_CeilingJoiningKnot::domain()
{
    return "Cleanroom";
}

QString CAD_Cleanroom_CeilingJoiningKnot::description()
{
    return "Cleanroom|Ceiling Joining Knot";
}

void CAD_Cleanroom_CeilingJoiningKnot::calculate()
{
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);
    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l/2, -l/2, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l  ,  0.0, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l/2,  l/2, 0.0));


    this->snap_flanges.append(position + matrix_rotation * QVector3D((l - b)/ 2, -b/2, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D((l + b)/ 2, -b/2, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D((l + b)/ 2, +b/2, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D((l - b)/ 2, +b/2, 0.0));


    QVector3D vertices[24];
    vertices[0]  = position + matrix_rotation * QVector3D(0.0,          -b/2,     0.0);
    vertices[1]  = position + matrix_rotation * QVector3D((l - b)/2,    -b/2,     0.0);
    vertices[2]  = position + matrix_rotation * QVector3D((l - b)/2,    -l/2,     0.0);
    vertices[3]  = position + matrix_rotation * QVector3D((l + b)/2,    -l/2,     0.0);
    vertices[4]  = position + matrix_rotation * QVector3D((l + b)/2,    -b/2,     0.0);
    vertices[5]  = position + matrix_rotation * QVector3D(l,            -b/2,     0.0);

    vertices[6]  = position + matrix_rotation * QVector3D(l,             b/2,     0.0);
    vertices[7]  = position + matrix_rotation * QVector3D((l + b)/2,     b/2,     0.0);
    vertices[8]  = position + matrix_rotation * QVector3D((l + b)/2,     l/2,     0.0);
    vertices[9]  = position + matrix_rotation * QVector3D((l - b)/2,     l/2,     0.0);
    vertices[10]  = position + matrix_rotation * QVector3D((l - b)/2,    b/2,     0.0);
    vertices[11]  = position + matrix_rotation * QVector3D(0.0,          b/2,     0.0);

    vertices[12]  = position + matrix_rotation * QVector3D(0.0,          -b/2,     a);
    vertices[13]  = position + matrix_rotation * QVector3D((l - b)/2,    -b/2,     a);
    vertices[14]  = position + matrix_rotation * QVector3D((l - b)/2,    -l/2,     a);
    vertices[15]  = position + matrix_rotation * QVector3D((l + b)/2,    -l/2,     a);
    vertices[16]  = position + matrix_rotation * QVector3D((l + b)/2,    -b/2,     a);
    vertices[17]  = position + matrix_rotation * QVector3D(l,            -b/2,     a);

    vertices[18]  = position + matrix_rotation * QVector3D(l,             b/2,     a);
    vertices[19]  = position + matrix_rotation * QVector3D((l + b)/2,     b/2,     a);
    vertices[20]  = position + matrix_rotation * QVector3D((l + b)/2,     l/2,     a);
    vertices[21]  = position + matrix_rotation * QVector3D((l - b)/2,     l/2,     a);
    vertices[22]  = position + matrix_rotation * QVector3D((l - b)/2,     b/2,     a);
    vertices[23]  = position + matrix_rotation * QVector3D(0.0,           b/2,     a);

    for(int i = 0; i < 24; i++)
    {
        this->boundingBox.enterVertex(vertices[i]);
    }

    GLushort indicesFaces[] =
    {
        0, 11, 5, 6, 0xABCD,
        1, 2, 4, 3, 0xABCD,
        10, 9, 7, 8, 0xABCD,

        12, 23, 17, 18, 0xABCD,
        13, 14, 16, 15, 0xABCD,
        22, 21, 19, 20, 0xABCD,

        12, 0, 13, 1, 14, 2, 15, 3, 16, 4, 17, 5, 18, 6, 19, 7, 20, 8, 21, 9, 22, 10, 23, 11, 12, 0, 0xABCD
    };

    GLushort indicesLines[72];
    for(int i = 0; i < 11; i++)
    {
        indicesLines[2*i] = i;
        indicesLines[2*i + 1] = i + 1;
    }
    for(int i = 0; i < 11; i++)
    {
        indicesLines[22 + 2*i] = i + 12;
        indicesLines[23 + 2*i] = i + 13;
    }
    indicesLines[44] = 11;
    indicesLines[45] = 0;
    indicesLines[46] = 23;
    indicesLines[47] = 12;

    for(int i = 0; i < 12; i++)
    {
        indicesLines[48 + 2*i] = i;
        indicesLines[49 + 2*i] = i + 12;
    }

    arrayBufVertices->bind();
    arrayBufVertices->allocate(vertices, sizeof(vertices));

    indexBufFaces->bind();
    indexBufFaces->allocate(indicesFaces, sizeof(indicesFaces));

    indexBufLines->bind();
    indexBufLines->allocate(indicesLines, sizeof(indicesLines));
}

void CAD_Cleanroom_CeilingJoiningKnot::processWizardInput()
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

void CAD_Cleanroom_CeilingJoiningKnot::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Cleanroom_CeilingJoiningKnot::rotationOfFlange(quint8 num)
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
        m.rotate(90.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else if(num == 5)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(180.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else if(num == 6)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(-90.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else if(num == 8)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(90.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else
        return matrix_rotation;
}
