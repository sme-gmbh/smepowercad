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

#include "cad_basic_arc.h"
#include "itemdb.h"
#include "glwidget.h"

#define PI 3.1415926535897

CAD_basic_arc::CAD_basic_arc() : CADitem(CADitemTypes::Basic_Arc)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);

    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("r", 1000.0);
    wizardParams.insert("alpha", 90.0);

    arrayBufVertices = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    arrayBufVertices->create();
    arrayBufVertices->setUsagePattern(QOpenGLBuffer::StaticDraw);

    indexBufLines = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBufLines->create();
    indexBufLines->setUsagePattern(QOpenGLBuffer::StaticDraw);

    processWizardInput();
    calculate();
}

CAD_basic_arc::~CAD_basic_arc()
{

}

QList<CADitemTypes::ItemType> CAD_basic_arc::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_basic_arc::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_basic_arc::iconPath()
{
    return ":/icons/cad_basic/cad_basic_arc.svg";
}

QString CAD_basic_arc::domain()
{
    return "Basic";
}

QString CAD_basic_arc::description()
{
    return "Basic|Arc";
}

void CAD_basic_arc::calculate()
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

//    this->snap_vertices.append(QVector3D(position.x()+radius, position.y(), position.z()));
//    this->snap_vertices.append(QVector3D(position.x()+radius*qCos(centralAngle/180.0f*PI), position.y()+radius*qSin(centralAngle/180.0f*PI), position.z()));
//    this->snap_vertices.append(QVector3D(position.x()+radius*qCos(centralAngle/360.0f*PI), position.y()+radius*qSin(centralAngle/360.0f*PI), position.z()));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.0, this->r, 0.0));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(sin(alpha/180.0f*PI) * this->r, cos(alpha/180.0f*PI) * this->r, 0.0));

    QVector3D vertices[21];
    for (int i = 0; i < 21; i++)
    {
        qreal angle = this->alpha/180.0f * PI * i / 20;
        QVector3D linePos;
        linePos = this->position;

        linePos += matrix_rotation * (QVector3D(sin(angle) * this->r, cos(angle) * this->r, 0.0));
        boundingBox.enterVertex(linePos);
        vertices[i] = linePos;
    }

    GLushort indicesLines[] = {
        0,1,
        1,2,
        2,3,
        3,4,
        4,5,
        5,6,
        6,7,
        7,8,
        8,9,
        9,10,
        10,11,
        11,12,
        12,13,
        13,14,
        14,15,
        15,16,
        16,17,
        17,18,
        18,19,
        19,20};


    arrayBufVertices->bind();
    arrayBufVertices->allocate(vertices, sizeof(vertices));

    indexBufLines->bind();
    indexBufLines->allocate(indicesLines, sizeof(indicesLines));

}

void CAD_basic_arc::processWizardInput()
{
    this->position.setX(wizardParams.value("Center x").toDouble());
    this->position.setY(wizardParams.value("Center y").toDouble());
    this->position.setZ(wizardParams.value("Center z").toDouble());
    //this->center = QVector3D(position.x()+r*qCos(alpha/360.0f*PI), position.y()+r*qSin(alpha/360.0f*PI), position.z());

    this->r = wizardParams.value("r").toDouble();
    this->alpha = wizardParams.value("alpha").toDouble();

    this->angle_x = wizardParams.value("Angle x").toDouble();
    this->angle_y = wizardParams.value("Angle y").toDouble();
    this->angle_z = wizardParams.value("Angle z").toDouble();

}

void CAD_basic_arc::paint(GLWidget *glwidget)
{
    QColor color_pen_tmp = getColorPen();

    glwidget->glEnable(GL_PRIMITIVE_RESTART);
    glwidget->glPrimitiveRestartIndex(0xABCD);

    arrayBufVertices->bind();
    glwidget->shaderProgram->enableAttributeArray(glwidget->shader_vertexLocation);
    glwidget->shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

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

QMatrix4x4 CAD_basic_arc::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}
