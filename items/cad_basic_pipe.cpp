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

#include "cad_basic_pipe.h"
#include "itemdb.h"
#include "glwidget.h"

CAD_basic_pipe::CAD_basic_pipe() : CADitem(CADitemTypes::Basic_Pipe)
{
//    vertices_inner_bottom = QList<QVector3D>();
//    vertices_outer_bottom = QList<QVector3D>();
//    vertices_outer_top = QList<QVector3D>();
//    vertices_inner_top = QList<QVector3D>();

    radius = 1.0;

    length = 20.0;

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);
    wizardParams.insert("l", 20.0);
    wizardParams.insert("d", 10.0);
    wizardParams.insert("s",  1.0);


    arrayBufVertices = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    arrayBufVertices.create();
    arrayBufVertices.setUsagePattern(QOpenGLBuffer::StaticDraw);

    indexBufFaces = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBufFaces.create();
    indexBufFaces.setUsagePattern(QOpenGLBuffer::StaticDraw);

    indexBufLines = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBufLines.create();
    indexBufLines.setUsagePattern(QOpenGLBuffer::StaticDraw);

    processWizardInput();
    calculate();
}

CAD_basic_pipe::~CAD_basic_pipe()
{
    arrayBufVertices.destroy();
    indexBufFaces.destroy();
    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_basic_pipe::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Basic_Pipe);
    flangable_items.append(CADitemTypes::Basic_Turn);
    return flangable_items;
}

QImage CAD_basic_pipe::wizardImage()
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

QString CAD_basic_pipe::iconPath()
{
    return ":/icons/cad_basic/cad_basic_pipe.svg";
}

QString CAD_basic_pipe::domain()
{
    return "Basic";
}

QString CAD_basic_pipe::description()
{
    return "Basic|Pipe";
}

void CAD_basic_pipe::calculate()
{
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    QVector3D vertices[64];
    int index = 0;
    for (qreal i=0.0; i < 1.0; i += 0.0625)    // 16 edges
    {
        qreal angle = 2 * PI * i;
        QVector3D linePos;

        linePos = matrix_rotation * QVector3D(0.0, sin(angle) * radius, cos(angle) * radius);
        linePos += position;
//        vertices_outer_bottom.append(linePos);
        vertices[index] = linePos;
        index++;
        this->boundingBox.enterVertex(linePos);
//        vertices_inner_bottom.append(linePos + (position - linePos).normalized() * wallThickness);
        vertices[index] = linePos + (position - linePos).normalized() * wallThickness;
        index++;
        QVector3D pos_top = position + matrix_rotation * QVector3D(length, 0.0, 0.0);
        linePos = matrix_rotation * QVector3D(0.0, sin(angle) * radius, cos(angle) * radius);
        linePos += pos_top;
//        vertices_outer_top.append(linePos);
        vertices[index] = linePos;
        index++;
        this->boundingBox.enterVertex(linePos);
//        vertices_inner_top.append(linePos + (pos_top - linePos).normalized() * wallThickness);
        vertices[index] = linePos + (pos_top - linePos).normalized() * wallThickness;
        index++;
    }

    static GLushort indicesFaces[150];
    for(int i = 0; i < 32; i++)
        indicesFaces[i] = 2*i;
    indicesFaces[32] = 0;
    indicesFaces[33] = 2;

    for(int i = 0; i < 32; i++)
        indicesFaces[34+i] = 1+2*i;
    indicesFaces[66] = 1;
    indicesFaces[67] = 3;
    indicesFaces[68] = 0xABCD;

    for(int i = 0; i < 16; i++)
    {
        indicesFaces[69+2*i] = 4*i;
        indicesFaces[69+2*i+1] = 4*i+1;
    }
    indicesFaces[101] = 0;
    indicesFaces[102] = 1;
    indicesFaces[103] = 0xABCD;

    for(int i = 0; i < 16; i++)
    {
        indicesFaces[104+2*i] = 4*i+2;
        indicesFaces[104+2*i+1] = 4*i+3;
    }

    indicesFaces[136] = 2;
    indicesFaces[137] = 3;


    static GLushort indicesLines[192];
    //outer circles
    for(int i = 0; i < 30; i++)
    {
        indicesLines[2*i] = 2*i;
        indicesLines[2*i+1] = 2*i + 4;
    }
    indicesLines[60] = 2;
    indicesLines[61] = 62;
    indicesLines[62] = 0;
    indicesLines[63] = 60;

    //inner circles
    for(int i = 0; i < 30; i++)
    {
        indicesLines[64 + 2*i] = 2*i+1;
        indicesLines[64 + 2*i+1] = 2*i + 4 +1;
    }
    indicesLines[124] = 3;
    indicesLines[125] = 63;
    indicesLines[126] = 1;
    indicesLines[127] = 61;

    //in flow direction
    for(int i = 0; i < 16; i++)
    {
        indicesLines[128 + 4*i] = 0 + 4*i;
        indicesLines[129 + 4*i] = 2 + 4*i;
        indicesLines[130 + 4*i] = 1 + 4*i;
        indicesLines[131 + 4*i] = 3 + 4*i;
    }

    arrayBufVertices.bind();
    arrayBufVertices.allocate(vertices, sizeof(vertices));

    indexBufFaces.bind();
    indexBufFaces.allocate(indicesFaces, sizeof(indicesFaces));

    indexBufLines.bind();
    indexBufLines.allocate(indicesLines, sizeof(indicesLines));



    this->snap_vertices.append(position);
    this->snap_vertices.append(this->position + this->direction);
    this->snap_center.append(this->position + this->direction * 0.5);
    this->snap_flanges.append(this->snap_vertices);

}

void CAD_basic_pipe::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
    wallThickness = wizardParams.value("s").toDouble();
    length = wizardParams.value("l").toDouble();
    radius = wizardParams.value("d").toDouble() / 2.0;

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);

    direction =  matrix_rotation * QVector3D(1.0, 0.0, 0.0) * length;
}

//void CAD_basic_pipe::paint(GLWidget *glwidget)
//{
//    QColor color_pen = getColorPen();
//    QColor color_brush = getColorBrush();

//    if (glwidget->render_solid)
//    {
//        // Vertical connections (faces)
//        glwidget->setPaintingColor(color_brush);
//        glwidget->glBegin(GL_QUADS);
//        // Outer cylinder
//        QVector3D last_vertex_bottom = vertices_outer_bottom.at(vertices_outer_bottom.count() - 1);
//        QVector3D last_vertex_top = vertices_outer_top.at(vertices_outer_bottom.count() - 1);
//        for (int i = 0; i < vertices_outer_bottom.count(); i++)
//        {
//            QVector3D vertex_bottom = vertices_outer_bottom.at(i);
//            QVector3D vertex_top = vertices_outer_top.at(i);
//            glwidget->glVertex3f((GLfloat)last_vertex_bottom.x(), (GLfloat)last_vertex_bottom.y(), (GLfloat)last_vertex_bottom.z());
//            glwidget->glVertex3f((GLfloat)last_vertex_top.x(), (GLfloat)last_vertex_top.y(), (GLfloat)last_vertex_top.z());
//            glwidget->glVertex3f((GLfloat)vertex_top.x(), (GLfloat)vertex_top.y(), (GLfloat)vertex_top.z());
//            glwidget->glVertex3f((GLfloat)vertex_bottom.x(), (GLfloat)vertex_bottom.y(), (GLfloat)vertex_bottom.z());
//            last_vertex_bottom = vertex_bottom;
//            last_vertex_top = vertex_top;
//        }
//        // Inner cylinder
//        last_vertex_bottom = vertices_inner_bottom.at(vertices_inner_bottom.count() - 1);
//        last_vertex_top = vertices_inner_top.at(vertices_inner_top.count() - 1);
//        for (int i = 0; i < vertices_inner_bottom.count(); i++)
//        {
//            QVector3D vertex_bottom = vertices_inner_bottom.at(i);
//            QVector3D vertex_top = vertices_inner_top.at(i);
//            glwidget->glVertex3f((GLfloat)last_vertex_bottom.x(), (GLfloat)last_vertex_bottom.y(), (GLfloat)last_vertex_bottom.z());
//            glwidget->glVertex3f((GLfloat)last_vertex_top.x(), (GLfloat)last_vertex_top.y(), (GLfloat)last_vertex_top.z());
//            glwidget->glVertex3f((GLfloat)vertex_top.x(), (GLfloat)vertex_top.y(), (GLfloat)vertex_top.z());
//            glwidget->glVertex3f((GLfloat)vertex_bottom.x(), (GLfloat)vertex_bottom.y(), (GLfloat)vertex_bottom.z());
//            last_vertex_bottom = vertex_bottom;
//            last_vertex_top = vertex_top;
//        }
//        // End disks
//        // Pipe length iteration
//        for (int l = 0; l <= 1; l++)
//        {

//            QVector3D last_vertex_outer = vertices_outer_bottom.at(vertices_outer_bottom.count() - 1);
//            QVector3D last_vertex_inner = vertices_inner_bottom.at(vertices_inner_bottom.count() - 1);
//            if (l == 1)
//            {
//                last_vertex_outer = vertices_outer_top.at(vertices_outer_top.count() - 1);
//                last_vertex_inner = vertices_inner_top.at(vertices_inner_top.count() - 1);
//            }

//            for (int i = 0; i < vertices_outer_bottom.count(); i++)
//            {

//                QVector3D vertex_outer = vertices_outer_bottom.at(i);
//                QVector3D vertex_inner = vertices_inner_bottom.at(i);
//                if (l == 0)
//                {
//                    glwidget->glVertex3f((GLfloat)last_vertex_inner.x(), (GLfloat)last_vertex_inner.y(), (GLfloat)last_vertex_inner.z());
//                    glwidget->glVertex3f((GLfloat)last_vertex_outer.x(), (GLfloat)last_vertex_outer.y(), (GLfloat)last_vertex_outer.z());
//                    glwidget->glVertex3f((GLfloat)vertex_outer.x(), (GLfloat)vertex_outer.y(), (GLfloat)vertex_outer.z());
//                    glwidget->glVertex3f((GLfloat)vertex_inner.x(), (GLfloat)vertex_inner.y(), (GLfloat)vertex_inner.z());
//                }
//                else
//                {
//                    vertex_outer = vertices_outer_top.at(i);
//                    vertex_inner = vertices_inner_top.at(i);
//                    glwidget->glVertex3f((GLfloat)last_vertex_inner.x(), (GLfloat)last_vertex_inner.y(), (GLfloat)last_vertex_inner.z());
//                    glwidget->glVertex3f((GLfloat)last_vertex_outer.x(), (GLfloat)last_vertex_outer.y(), (GLfloat)last_vertex_outer.z());
//                    glwidget->glVertex3f((GLfloat)vertex_outer.x(), (GLfloat)vertex_outer.y(), (GLfloat)vertex_outer.z());
//                    glwidget->glVertex3f((GLfloat)vertex_inner.x(), (GLfloat)vertex_inner.y(), (GLfloat)vertex_inner.z());
//                }

//                last_vertex_outer = vertex_outer;
//                last_vertex_inner = vertex_inner;
//            }
//        }
//        glwidget->glEnd();
//    }

//    if (glwidget->render_outline)
//    {
//        glwidget->setPaintingColor(color_pen);
//        glwidget->glLineWidth(1.0);


//        // Vertical connections (lines)
//        glwidget->glBegin(GL_LINES);
//        // Outer cylinder
//        QVector3D last_vertex_bottom = vertices_outer_bottom.at(vertices_outer_bottom.count() - 1);
//        QVector3D last_vertex_top = vertices_outer_top.at(vertices_outer_top.count() - 1);
//        for (int i = 0; i < vertices_outer_bottom.count(); i++)
//        {
//            QVector3D vertex_bottom = vertices_outer_bottom.at(i);
//            QVector3D vertex_top = vertices_outer_top.at(i);
//            glwidget->glVertex3f((GLfloat)last_vertex_bottom.x(), (GLfloat)last_vertex_bottom.y(), (GLfloat)last_vertex_bottom.z());
//            glwidget->glVertex3f((GLfloat)(last_vertex_top.x()), (GLfloat)(last_vertex_top.y()), (GLfloat)(last_vertex_top.z()));
//            glwidget->glVertex3f((GLfloat)(vertex_top.x()), (GLfloat)(vertex_top.y()), (GLfloat)(vertex_top.z()));
//            glwidget->glVertex3f((GLfloat)vertex_bottom.x(), (GLfloat)vertex_bottom.y(), (GLfloat)vertex_bottom.z());
//            last_vertex_bottom = vertex_bottom;
//            last_vertex_top = vertex_top;
//        }
//        // Inner cylinder
//        last_vertex_bottom = vertices_inner_bottom.at(vertices_inner_bottom.count() - 1);
//        last_vertex_top = vertices_inner_top.at(vertices_inner_top.count() - 1);
//        for (int i = 0; i < vertices_inner_bottom.count(); i++)
//        {
//            QVector3D vertex_bottom = vertices_inner_bottom.at(i);
//            QVector3D vertex_top = vertices_inner_top.at(i);
//            glwidget->glVertex3f((GLfloat)last_vertex_bottom.x(), (GLfloat)last_vertex_bottom.y(), (GLfloat)last_vertex_bottom.z());
//            glwidget->glVertex3f((GLfloat)(last_vertex_top.x()), (GLfloat)(last_vertex_top.y()), (GLfloat)(last_vertex_top.z()));
//            glwidget->glVertex3f((GLfloat)(vertex_top.x()), (GLfloat)(vertex_top.y()), (GLfloat)(vertex_top.z()));
//            glwidget->glVertex3f((GLfloat)vertex_bottom.x(), (GLfloat)vertex_bottom.y(), (GLfloat)vertex_bottom.z());
//            last_vertex_bottom = vertex_bottom;
//            last_vertex_top = vertex_top;
//        }
//        glwidget->glEnd();
//        //Radial connections (lines)
//        glwidget->glBegin(GL_LINE_LOOP);
//        for (int i = 0; i < vertices_inner_bottom.count(); i++)
//        {
//            glwidget->glVertex3f((GLfloat)vertices_inner_bottom.at(i).x(), (GLfloat)vertices_inner_bottom.at(i).y(), (GLfloat)vertices_inner_bottom.at(i).z());
//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINE_LOOP);
//        for (int i = 0; i < vertices_outer_bottom.count(); i++)
//        {
//            glwidget->glVertex3f((GLfloat)vertices_outer_bottom.at(i).x(), (GLfloat)vertices_outer_bottom.at(i).y(), (GLfloat)vertices_outer_bottom.at(i).z());
//        }
//        glwidget->glEnd();

//        glwidget->glBegin(GL_LINE_LOOP);
//        for (int i = 0; i < vertices_inner_top.count(); i++)
//        {
//            glwidget->glVertex3f((GLfloat)vertices_inner_top.at(i).x(), (GLfloat)vertices_inner_top.at(i).y(), (GLfloat)vertices_inner_top.at(i).z());
//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINE_LOOP);
//        for (int i = 0; i < vertices_outer_top.count(); i++)
//        {
//            glwidget->glVertex3f((GLfloat)vertices_outer_top.at(i).x(), (GLfloat)vertices_outer_top.at(i).y(), (GLfloat)vertices_outer_top.at(i).z());
//        }
//        glwidget->glEnd();

//        // Center line
//        glwidget->glLineWidth(3.0);
//        glwidget->glLineStipple(2, 0x00FF);
//        glwidget->glEnable(GL_LINE_STIPPLE);
//        glwidget->glBegin(GL_LINES);
//        glwidget->glVertex3f((GLfloat)position.x(), (GLfloat)position.y(), (GLfloat)position.z());
//        glwidget->glVertex3f((GLfloat)(snap_flanges.at(0).x()), (GLfloat)(snap_flanges.at(0).y()), (GLfloat)(snap_flanges.at(0).z()));
//        glwidget->glEnd();
//        glwidget->glDisable(GL_LINE_STIPPLE);
//    }
// }

void CAD_basic_pipe::paint(GLWidget *glwidget)
{
    glwidget->glEnable(GL_PRIMITIVE_RESTART);
    glwidget->glPrimitiveRestartIndex(0xABCD);

    QColor color_pen_tmp = getColorPen();
    QColor color_brush_tmp = getColorBrush();

    arrayBufVertices.bind();
    glwidget->shaderProgram->enableAttributeArray(glwidget->shader_vertexLocation);
    glwidget->shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

    if (glwidget->render_solid)
    {
        glwidget->setPaintingColor(color_brush_tmp);

        indexBufFaces.bind();
        glwidget->glDrawElements(GL_TRIANGLE_STRIP, indexBufFaces.size(), GL_UNSIGNED_SHORT, 0);

        indexBufFaces.release();
    }

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
