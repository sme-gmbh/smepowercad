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

#include "cad_basic_pipereducer.h"
#include "glwidget.h"

CAD_Basic_PipeReducer::CAD_Basic_PipeReducer() : CADitem(CADitemTypes::Basic_PipeReducer)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("l", 100.0);
    wizardParams.insert("d1", 100.0);
    wizardParams.insert("d2", 150.0);
    wizardParams.insert("e", 50.0);
    wizardParams.insert("s", 10.0);

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

CAD_Basic_PipeReducer::~CAD_Basic_PipeReducer()
{
    arrayBufVertices.destroy();
    indexBufFaces.destroy();
    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_Basic_PipeReducer::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Basic_PipeReducer::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Basic_PipeReducer::iconPath()
{
    return ":/icons/cad_basic/cad_basic_pipereducer.svg";
}

QString CAD_Basic_PipeReducer::domain()
{
    return "Basic";
}

QString CAD_Basic_PipeReducer::description()
{
    return "Basic|Pipe Reducer";
}

void CAD_Basic_PipeReducer::calculate()
{                
    boundingBox.reset();
                    
    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();
                                
    this->snap_basepoint = (position);

    qreal radius = d1/2;
    qreal radius2 = d2/2;

    QVector3D vertices[64];
    int index = 0;

    for (qreal i=0.0; i < 1.0; i += 0.0625)    // 16 edges
    {
        qreal angle = 2 * PI * i;
        QVector3D linePos;

        linePos = matrix_rotation * QVector3D(0.0, sin(angle) * radius, cos(angle) * radius);
        linePos += position;
        vertices[index] = linePos;
        boundingBox.enterVertex(linePos);
        index++;
        vertices[index] = linePos + (position - linePos).normalized() * s;
        boundingBox.enterVertex(linePos + (position - linePos).normalized() * s);
        index++;
        QVector3D pos_top = position + matrix_rotation * QVector3D(l, 0.0, -e);
        linePos = matrix_rotation * QVector3D(0.0, sin(angle) * radius2, cos(angle) * radius2);
        linePos += pos_top;
        vertices[index] = linePos;
        boundingBox.enterVertex(linePos);
        index++;
        vertices[index] = linePos + (pos_top - linePos).normalized() * s;
        boundingBox.enterVertex(linePos + (pos_top - linePos).normalized() * s);
        index++;
    }

    static GLushort indicesFaces[139];
    //outer faces
    for(int i = 0; i < 32; i++)
        indicesFaces[i] = 2*i;

    indicesFaces[32] = 0;
    indicesFaces[33] = 2;
    indicesFaces[34] = 0xABCD;

    //inner faces
    for(int i = 0; i < 16; i++)
    {
        indicesFaces[35 + 2*i] = 4 * i + 3;
        indicesFaces[35 + 2*i + 1] = 4 * i +1;
    }
    indicesFaces[67] = 3;
    indicesFaces[68] = 1;
    indicesFaces[69] = 0xABCD;

    //ring on negative x-end
    for(int i = 0; i < 16; i++)
    {
        indicesFaces[70+2*i] = 4*i+1;
        indicesFaces[70+2*i+1] = 4*i;
    }
    indicesFaces[102] = 1;
    indicesFaces[103] = 0;
    indicesFaces[104] = 0xABCD;

    //ring on positive x-end
    for(int i = 0; i < 16; i++)
    {
        indicesFaces[105+2*i] = 4*i+2;
        indicesFaces[105+2*i+1] = 4*i+3;
    }

    indicesFaces[137] = 2;
    indicesFaces[138] = 3;


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
    this->snap_vertices.append(this->position + matrix_rotation * QVector3D(l, 0.0, -e));

}

void CAD_Basic_PipeReducer::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    l = wizardParams.value("l").toDouble();
    d1 = wizardParams.value("d1").toDouble();
    d2 = wizardParams.value("d2").toDouble();
    e = wizardParams.value("e").toDouble();
    s = wizardParams.value("s").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

void CAD_Basic_PipeReducer::paint(GLWidget *glwidget)
{
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

QMatrix4x4 CAD_Basic_PipeReducer::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}
