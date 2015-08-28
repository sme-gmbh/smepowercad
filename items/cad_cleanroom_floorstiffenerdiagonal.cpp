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

#include "cad_cleanroom_floorstiffenerdiagonal.h"
#include "glwidget.h"

CAD_Cleanroom_FloorStiffenerDiagonal::CAD_Cleanroom_FloorStiffenerDiagonal() : CADitem(CADitemTypes::Cleanroom_FloorStiffenerDiagonal)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 1000.0);
    wizardParams.insert("f",   10.0);
    wizardParams.insert("l",  600.0);

    stiffener = new CAD_basic_box();
    this->subItems.append(stiffener);

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

CAD_Cleanroom_FloorStiffenerDiagonal::~CAD_Cleanroom_FloorStiffenerDiagonal()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Cleanroom_FloorStiffenerDiagonal::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Cleanroom_FloorStiffenerDiagonal::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");     
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Cleanroom_FloorStiffenerDiagonal::iconPath()
{
    return ":/icons/cad_cleanroom/cad_cleanroom_floorstiffenerdiagonal.svg";
}

QString CAD_Cleanroom_FloorStiffenerDiagonal::domain()
{
    return "Cleanroom";
}

QString CAD_Cleanroom_FloorStiffenerDiagonal::description()
{
    return "Cleanroom|Floor Stiffener Diagonal";
}

void CAD_Cleanroom_FloorStiffenerDiagonal::calculate()
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

    QVector3D pos = position + matrix_rotation * QVector3D(l/2, 0.0, -a/2);
    stiffener->wizardParams.insert("Position x", pos.x());
    stiffener->wizardParams.insert("Position y", pos.y());
    stiffener->wizardParams.insert("Position z", pos.z());
    stiffener->wizardParams.insert("Angle x", angle_x);
    stiffener->wizardParams.insert("Angle y", angle_y);
    stiffener->wizardParams.insert("Angle z", angle_z);
    stiffener->wizardParams.insert("l", sqrt(a*a + l*l) * 0.85);
    stiffener->wizardParams.insert("b", f);
    stiffener->wizardParams.insert("a", f);
    stiffener->layer = this->layer;
    stiffener->processWizardInput();
    qreal angle = atan(a / l) / PI * 180.0;
    stiffener->rotateAroundAxis(angle, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    stiffener->calculate();

    this->boundingBox = stiffener->boundingBox;
}

void CAD_Cleanroom_FloorStiffenerDiagonal::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a = wizardParams.value("a").toDouble();
    f = wizardParams.value("f").toDouble();
    l = wizardParams.value("l").toDouble();
}

//void CAD_cleanroom_FloorStiffenerDiagonal::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Cleanroom_FloorStiffenerDiagonal::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}
