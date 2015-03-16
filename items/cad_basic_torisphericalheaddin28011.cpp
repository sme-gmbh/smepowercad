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

#include "cad_basic_torisphericalheaddin28011.h"
#include "glwidget.h"

CAD_Basic_TorisphericalHeadDIN28011::CAD_Basic_TorisphericalHeadDIN28011() : CADitem(CADitemTypes::Basic_TorisphericalHeadDIN28011)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);
    wizardParams.insert("d", 100.0);   // Durchmesser
    wizardParams.insert("h", 10.0);     // Höhe

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


CAD_Basic_TorisphericalHeadDIN28011::~CAD_Basic_TorisphericalHeadDIN28011()
{
//    arrayBufVertices.destroy();
//    indexBufFaces.destroy();
//    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_Basic_TorisphericalHeadDIN28011::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Basic_TorisphericalHeadDIN28011::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Basic_TorisphericalHeadDIN28011::iconPath()
{
    return ":/icons/cad_basic/cad_basic_torisphericalheaddin28011.svg";
}

QString CAD_Basic_TorisphericalHeadDIN28011::domain()
{
    return "Basic";
}

QString CAD_Basic_TorisphericalHeadDIN28011::description()
{
    return "Basic|Torispherical Head DIN28011";
}

void CAD_Basic_TorisphericalHeadDIN28011::calculate()
{

    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    qreal r1 = d;
    qreal r2 = 0.1 * d;
    qreal a = 0.4 * d;
    qreal c = 0.9 * d;
    qreal b = sqrt(c*c - a*a);
    qreal beta = 2*0.460553992; // = 2* asin(4/9)
    qreal alpha = PI / 2 - beta;

    QVector3D vertices[111];
    for(int i = 0; i < 11; i++)
    {
        for(int j = 0; j <= 10; j++)
        {
            if(i < 7)
            {
            qreal phi = i * beta / 12;
            qreal psi = j * PI / 5;
            vertices[10*i + j] = position + matrix_rotation * (QVector3D(r1 * sin(phi) * cos(psi), r1 * sin(phi) * sin(psi),r1 * cos(phi)) +
                                                               QVector3D(0.0, 0.0, -b + h));
            }
            else if( i < 10)
            {
                QMatrix4x4 matrix_turn;
                matrix_turn.setToIdentity();
                matrix_turn.rotate(j * 36.0, 0.0, 0.0, 1.0);
                QVector3D offset = QVector3D(a + cos(alpha * (10 - i) / 3) * r2, 0.0, h + sin(alpha * (10 - i) / 3) * r2);
                vertices[10*i + j] = position + matrix_rotation * matrix_turn * offset;
            }
            else
            {
                vertices[10*i + j] = position + matrix_rotation * QVector3D(cos(j * PI / 5) * d/2, sin(j * PI / 5) * d/2, 0.0);
            }

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


//    for(int i = 0; i < 3; i++)
//    {
//        for(int j = 0; j < 10; j++)
//        {
//            indicesFaces[37*i + 4*j]   = 10*j + i;
//            indicesFaces[37*i + 4*j+1] = 10*j + i + 10;
//            indicesFaces[37*i + 4*j+2] = 10*j + i + 1;
//            indicesFaces[37*i + 4*j+3] = 10*j + i + 11;
//        }
//        indicesFaces[37*i + 36] = 0xABCD;
//    }

    static GLushort indicesLines[420];
    for(int i = 0; i < 11; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            indicesLines[20*i+2*j] = 10*i+j;
            indicesLines[20*i+2*j+1] = 10*i+j+1;
        }
        indicesLines[20*i+18] = 10*i+9;
        indicesLines[20*i+19] = 10*i;
    }

    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            indicesLines[220+20*i+2*j] = 10*j+i;
            indicesLines[220+20*i+2*j+1] = 10*j+i+10;
        }
    }


    arrayBufVertices.bind();
    arrayBufVertices.allocate(vertices, sizeof(vertices));

    indexBufFaces.bind();
    indexBufFaces.allocate(indicesFaces, sizeof(indicesFaces));

    indexBufLines.bind();
    indexBufLines.allocate(indicesLines, sizeof(indicesLines));

    boundingBox.enterVertex(position + matrix_rotation * QVector3D( d/2, 0.0, 0.0));
    boundingBox.enterVertex(position + matrix_rotation * QVector3D( -d/2, 0.0, 0.0));
    boundingBox.enterVertex(position + matrix_rotation * QVector3D( 0.0, d/2, 0.0));
    boundingBox.enterVertex(position + matrix_rotation * QVector3D( 0.0, -d/2, 0.0));
    boundingBox.enterVertex(position + matrix_rotation * QVector3D(0.0, 0.0, h + 0.1937742252 * d));
}

void CAD_Basic_TorisphericalHeadDIN28011::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
    d = wizardParams.value("d").toDouble();
    h = wizardParams.value("h").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

void CAD_Basic_TorisphericalHeadDIN28011::paint(GLWidget *glwidget)
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
