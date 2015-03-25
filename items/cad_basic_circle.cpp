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

#include "cad_basic_circle.h"
#include "itemdb.h"
#include "glwidget.h"

CAD_basic_circle::CAD_basic_circle() : CADitem(CADitemTypes::Basic_Circle)
{
    this->width = 0.0;
    this->widthByLayer = true;
    this->widthByBlock = false;

    wizardParams.insert("Center x", 0.0);
    wizardParams.insert("Center y", 0.0);
    wizardParams.insert("Center z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("Radius", 1000.0);

    arrayBufVertices = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    arrayBufVertices.create();
    arrayBufVertices.setUsagePattern(QOpenGLBuffer::StaticDraw);

    indexBufLines = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBufLines.create();
    indexBufLines.setUsagePattern(QOpenGLBuffer::StaticDraw);

    processWizardInput();
    calculate();
}

CAD_basic_circle::~CAD_basic_circle()
{

}

QList<CADitemTypes::ItemType> CAD_basic_circle::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_basic_circle::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    ;

    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_basic_circle::iconPath()
{
    return ":/icons/cad_basic/cad_basic_circle.svg";
}

QString CAD_basic_circle::domain()
{
    return "Basic";
}

QString CAD_basic_circle::description()
{
    return "Basic|Circle";
}

void CAD_basic_circle::calculate()
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

    this->snap_basepoint = this->center;

    QVector3D vertices[16];
    for (int i = 0; i < 16; i++)    // 16 edges
    {
        qreal angle = 2 * PI * i * 0.0625f;
        QVector3D linePos;
        linePos = this->center;

        linePos += matrix_rotation * QVector3D(sin(angle) * this->radius, cos(angle) * this->radius, 0.0);
        vertices[i] = linePos;
        boundingBox.enterVertex(linePos);
    }


    GLushort indicesLines[32];
    for(int i = 0; i < 16; i++)
    {
        indicesLines[2*i] = i;
        indicesLines[2*i + 1] = i + 1;
    }
    indicesLines[31] = 0;


    arrayBufVertices.bind();
    arrayBufVertices.allocate(vertices, sizeof(vertices));

    indexBufLines.bind();
    indexBufLines.allocate(indicesLines, sizeof(indicesLines));
}

void CAD_basic_circle::processWizardInput()
{
    center.setX(wizardParams.value("Center x").toDouble());
    center.setY(wizardParams.value("Center y").toDouble());
    center.setZ(wizardParams.value("Center z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
    radius = wizardParams.value("Radius").toDouble();
}

//void CAD_basic_circle::paint(GLWidget *glwidget)
//{
//    QColor color_pen = getColorPen();

//    qreal penWidth = 1.0;
//    if (widthByLayer)
//    {
//        penWidth = layer->width / 100.0 / glwidget->zoomFactor;
//    }
//    else if (widthByBlock)
//    {

//    }
//    else
//    {
//        penWidth = width / glwidget->zoomFactor;
//    }

//    // Default width setting
//    if (penWidth < 1.0)
//        penWidth = 1.0;

//    glwidget->setPaintingColor(color_pen);
//    glwidget->glLineWidth(penWidth);
//    glwidget->glBegin(GL_LINE_LOOP);
//    foreach (QVector3D linePos, circle)    // 50 edges
//    {
//        glwidget->glVertex3f((GLfloat)linePos.x(), (GLfloat)linePos.y(), (GLfloat)linePos.z());
//    }
//    glwidget->glEnd();
//}

void CAD_basic_circle::paint(GLWidget *glwidget)
{
    QColor color_pen_tmp = getColorPen();

    glwidget->glEnable(GL_PRIMITIVE_RESTART);
    glwidget->glPrimitiveRestartIndex(0xABCD);

    arrayBufVertices.bind();
    glwidget->shaderProgram->enableAttributeArray(glwidget->shader_vertexLocation);
    glwidget->shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

    if (glwidget->render_outline)
    {
        glwidget->setPaintingColor(color_pen_tmp);
        glwidget->glLineWidth(1.0);

        indexBufLines.bind();
        glwidget->glDrawElements(GL_LINES, indexBufLines.size(), GL_UNSIGNED_SHORT, 0);
        indexBufLines.release();
    }

    arrayBufVertices.release();
}


QMatrix4x4 CAD_basic_circle::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}
