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

#include "cad_basic_box.h"
#include "glwidget.h"

CAD_basic_box::CAD_basic_box() : CADitem(CADitemTypes::Basic_Box)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("l", 1000.0);
    wizardParams.insert("b", 1000.0);
    wizardParams.insert("a", 1000.0);

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

CAD_basic_box::~CAD_basic_box()
{
    arrayBufVertices->destroy();
    indexBufFaces->destroy();
    indexBufLines->destroy();
    delete arrayBufVertices;
    delete indexBufFaces;
    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_basic_box::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_basic_box::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_basic_box::iconPath()
{
    return ":/icons/cad_basic/cad_basic_box.svg";
}

QString CAD_basic_box::domain()
{
    return "Basic";
}

QString CAD_basic_box::description()
{
    return "Basic|Box";
}

void CAD_basic_box::calculate()
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

    pos_bot_1 = position;
    pos_bot_2 = position;
    pos_bot_3 = position;
    pos_bot_4 = position;
    pos_top_1 = position;
    pos_top_2 = position;
    pos_top_3 = position;
    pos_top_4 = position;

    pos_bot_1 += matrix_rotation * (QVector3D(-0.5, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z());
    pos_bot_2 += matrix_rotation * (QVector3D( 0.5, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z());
    pos_bot_3 += matrix_rotation * (QVector3D( 0.5, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z());
    pos_bot_4 += matrix_rotation * (QVector3D(-0.5, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0, -0.5) * size.z());
    pos_top_1 += matrix_rotation * (QVector3D(-0.5, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z());
    pos_top_2 += matrix_rotation * (QVector3D( 0.5, 0.0, 0.0) * size.x() + QVector3D(0.0, -0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z());
    pos_top_3 += matrix_rotation * (QVector3D( 0.5, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z());
    pos_top_4 += matrix_rotation * (QVector3D(-0.5, 0.0, 0.0) * size.x() + QVector3D(0.0,  0.5, 0.0) * size.y() + QVector3D(0.0, 0.0,  0.5) * size.z());

    this->snap_vertices.append(pos_bot_1);
    this->snap_vertices.append(pos_bot_2);
    this->snap_vertices.append(pos_bot_3);
    this->snap_vertices.append(pos_bot_4);
    this->snap_vertices.append(pos_top_1);
    this->snap_vertices.append(pos_top_2);
    this->snap_vertices.append(pos_top_3);
    this->snap_vertices.append(pos_top_4);

    boundingBox.enterVertices(this->snap_vertices);

    // Bottom edge centers
    this->snap_center.append((pos_bot_1 + pos_bot_2) / 2.0);
    this->snap_center.append((pos_bot_2 + pos_bot_3) / 2.0);
    this->snap_center.append((pos_bot_3 + pos_bot_4) / 2.0);
    this->snap_center.append((pos_bot_4 + pos_bot_1) / 2.0);
    // Top edge centers
    this->snap_center.append((pos_top_1 + pos_top_2) / 2.0);
    this->snap_center.append((pos_top_2 + pos_top_3) / 2.0);
    this->snap_center.append((pos_top_3 + pos_top_4) / 2.0);
    this->snap_center.append((pos_top_4 + pos_top_1) / 2.0);
    // Connection edge centers
    this->snap_center.append((pos_bot_1 + pos_top_1) / 2.0);
    this->snap_center.append((pos_bot_2 + pos_top_2) / 2.0);
    this->snap_center.append((pos_bot_3 + pos_top_3) / 2.0);
    this->snap_center.append((pos_bot_4 + pos_top_4) / 2.0);

    // Calculate face normal vectors
    this->normal_bot   = (pos_bot_1 - pos_top_1).normalized();
    this->normal_top   = (pos_top_1 - pos_bot_1).normalized();
    this->normal_left  = (pos_bot_1 - pos_bot_2).normalized();
    this->normal_right = (pos_bot_2 - pos_bot_1).normalized();
    this->normal_front = (pos_bot_2 - pos_bot_3).normalized();
    this->normal_back  = (pos_bot_3 - pos_bot_2).normalized();

    QVector3D vertices[] = {
        pos_bot_1,
        pos_bot_2,
        pos_bot_3,
        pos_bot_4,
        pos_top_1,
        pos_top_2,
        pos_top_3,
        pos_top_4
    };

    static GLushort indicesFaces[] = {
        5, 6, 7, 3, 4, 0, 1, 3, 2, 6, 1, 5, 4, 7, 0xABCD
    };

    static GLushort indicesLines[] = {
        0, 1,
        1, 2,
        2, 3,
        3, 0,
        4, 5,
        5, 6,
        6, 7,
        7, 4,
        0, 4,
        1, 5,
        2, 6,
        3, 7
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

void CAD_basic_box::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
    size.setX(wizardParams.value("l").toDouble());
    size.setY(wizardParams.value("b").toDouble());
    size.setZ(wizardParams.value("a").toDouble());
}

void CAD_basic_box::paint(GLWidget *glwidget)
{
    QColor color_pen_tmp = getColorPen();
    QColor color_brush_tmp = getColorBrush();

    glwidget->glEnable(GL_PRIMITIVE_RESTART);
    glwidget->glPrimitiveRestartIndex(0xABCD);

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

QMatrix4x4 CAD_basic_box::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}
