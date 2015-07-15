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

#include "cad_sprinkler_flange.h"
#include "glwidget.h"

CAD_Sprinkler_Flange::CAD_Sprinkler_Flange() : CADitem(CADitemTypes::Sprinkler_Flange)
{
    pipe = new CAD_basic_pipe;
    flange = new CAD_basic_pipe;
    this->subItems.append(pipe);
    this->subItems.append(flange);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("d",  150.0);
    wizardParams.insert("fe",  10.0);
    wizardParams.insert("ff",  40.0);
    wizardParams.insert("l",  100.0);
    wizardParams.insert("s",    5.0);

//    arrayBufVertices = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
//    arrayBufVertices->create();
//    arrayBufVertices->setUsagePattern(QOpenGLBuffer::StaticDraw);

//    indexBufFaces = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
//    indexBufFaces->create();
//    indexBufFaces->setUsagePattern(QOpenGLBuffer::StaticDraw);

//    indexBufLines = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
//    indexBufLines->create();
//    indexBufLines->setUsagePattern(QOpenGLBuffer::StaticDraw);
   
    processWizardInput();
    calculate();
}

CAD_Sprinkler_Flange::~CAD_Sprinkler_Flange()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Sprinkler_Flange::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    if (flangeIndex == 1)
    {
        flangable_items.append(CADitemTypes::Sprinkler_Pipe);
        flangable_items.append(CADitemTypes::Sprinkler_PipeEndCap);
        flangable_items.append(CADitemTypes::Sprinkler_PipeReducer);
        flangable_items.append(CADitemTypes::Sprinkler_PipeTurn);
        flangable_items.append(CADitemTypes::Sprinkler_PipeTeeConnector);
    }
    else if (flangeIndex == 2)
    {
        flangable_items.append(CADitemTypes::Sprinkler_Flange);
        flangable_items.append(CADitemTypes::Sprinkler_Distribution);
    }
    return flangable_items;
}

QImage CAD_Sprinkler_Flange::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");   
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Sprinkler_Flange::iconPath()
{
    return ":/icons/cad_sprinkler/cad_sprinkler_flange.svg";
}

QString CAD_Sprinkler_Flange::domain()
{
    return "Sprinkler";
}

QString CAD_Sprinkler_Flange::description()
{
    return "Sprinkler|Flange";
}

void CAD_Sprinkler_Flange::calculate()
{                
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    pipe->wizardParams.insert("Position x", position.x());
    pipe->wizardParams.insert("Position y", position.y());
    pipe->wizardParams.insert("Position z", position.z());
    pipe->wizardParams.insert("Angle x", angle_x);
    pipe->wizardParams.insert("Angle y", angle_y);
    pipe->wizardParams.insert("Angle z", angle_z);
    pipe->wizardParams.insert("l", l);
    pipe->wizardParams.insert("d", d);
    pipe->wizardParams.insert("s",  s);
    pipe->layer = this->layer;
    pipe->processWizardInput();
    pipe->calculate();

    QVector3D position_fl = position + matrix_rotation * QVector3D(l - fe,0.0, 0.0);
    flange->wizardParams.insert("Position x", position_fl.x());
    flange->wizardParams.insert("Position y", position_fl.y());
    flange->wizardParams.insert("Position z", position_fl.z());
    flange->wizardParams.insert("Angle x", angle_x);
    flange->wizardParams.insert("Angle y", angle_y);
    flange->wizardParams.insert("Angle z", angle_z);
    flange->wizardParams.insert("l", fe);
    flange->wizardParams.insert("d", d + 2 * ff);
    flange->wizardParams.insert("s",  ff);
    flange->layer = this->layer;
    flange->processWizardInput();
    flange->calculate();

    this->boundingBox = pipe->boundingBox;
    this->boundingBox.enterVertices(flange->boundingBox.getVertices());
    this->snap_flanges = pipe->snap_flanges;
}

void CAD_Sprinkler_Flange::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    d = wizardParams.value("d").toDouble();
    ff = wizardParams.value("ff").toDouble();
    l = wizardParams.value("l").toDouble();
    fe = wizardParams.value("fe").toDouble();
    s = wizardParams.value("s").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

QMatrix4x4 CAD_Sprinkler_Flange::rotationOfFlange(quint8 num)
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
        return matrix_rotation;
    }
    else
        return matrix_rotation;
}

//void CAD_Sprinkler_Flange::paint(GLWidget *glwidget)
//{
//    QColor color_pen_tmp = getColorPen();
//    QColor color_brush_tmp = getColorBrush();

//    arrayBufVertices->bind();
//    glwidget->shaderProgram->enableAttributeArray(glwidget->shader_vertexLocation);
//    glwidget->shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

//    if (glwidget->render_solid)
//    {
//        glwidget->setPaintingColor(color_brush_tmp);

//        indexBufFaces->bind();
//        glwidget->glDrawElements(GL_TRIANGLE_STRIP, indexBufFaces->size(), GL_UNSIGNED_SHORT, 0);

//        indexBufFaces->release();
//    }

//    if (glwidget->render_outline)
//    {
//        glwidget->setPaintingColor(color_pen_tmp);
                                      
//        indexBufLines->bind();
//        glwidget->glDrawElements(GL_LINES, indexBufLines->size(), GL_UNSIGNED_SHORT, 0);
//        indexBufLines->release();
//     }                          
                                                                                           
//     arrayBufVertices->release();
//}
