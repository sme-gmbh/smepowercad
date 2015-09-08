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

#include "cad_cleanroom_doorframe.h"
#include "glwidget.h"

CAD_Cleanroom_DoorFrame::CAD_Cleanroom_DoorFrame() : CADitem(CADitemTypes::Cleanroom_DoorFrame)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",  2500.0);
    wizardParams.insert("h", 2100.0);
    wizardParams.insert("l",  2000.0);
    wizardParams.insert("g", 1000.0);
    wizardParams.insert("b",   100.0);

    box_left = new CAD_basic_box();
    box_right = new CAD_basic_box();
    box_up = new CAD_basic_box();


    this->subItems.append(box_left);
    this->subItems.append(box_up);
    this->subItems.append(box_right);

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

CAD_Cleanroom_DoorFrame::~CAD_Cleanroom_DoorFrame()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Cleanroom_DoorFrame::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Cleanroom_DoorSlidingDouble);
    flangable_items.append(CADitemTypes::Cleanroom_DoorSlidingSingle);
    flangable_items.append(CADitemTypes::Cleanroom_DoorSwingingDouble);
    flangable_items.append(CADitemTypes::Cleanroom_DoorSwingingSingle);
    return flangable_items;
}

QImage CAD_Cleanroom_DoorFrame::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");     
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Cleanroom_DoorFrame::iconPath()
{
    return ":/icons/cad_cleanroom/cad_cleanroom_doorframe.svg";
}

QString CAD_Cleanroom_DoorFrame::domain()
{
    return "Cleanroom";
}

QString CAD_Cleanroom_DoorFrame::description()
{
    return "Cleanroom|Door Frame";
}

void CAD_Cleanroom_DoorFrame::calculate()
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

    QVector3D pos_left = (matrix_rotation * QVector3D((l - g) * 0.25, 0.0, a * 0.5)) + position;
    box_left->wizardParams.insert("Position x", (pos_left.x()));
    box_left->wizardParams.insert("Position y", (pos_left.y()));
    box_left->wizardParams.insert("Position z", (pos_left.z()));
    box_left->wizardParams.insert("Angle x", (angle_x));
    box_left->wizardParams.insert("Angle y", (angle_y));
    box_left->wizardParams.insert("Angle z", (angle_z));
    box_left->wizardParams.insert("l", ((l - g) * 0.5));
    box_left->wizardParams.insert("b", (b));
    box_left->wizardParams.insert("a", (a));
    box_left->layer = this->layer;
    box_left->processWizardInput();
    box_left->calculate();

    QVector3D pos_right = (matrix_rotation * QVector3D(l * 0.75 + g * 0.25, 0.0, a/2.0)) + position;
    box_right->wizardParams.insert("Position x", (pos_right.x()));
    box_right->wizardParams.insert("Position y", (pos_right.y()));
    box_right->wizardParams.insert("Position z", (pos_right.z()));
    box_right->wizardParams.insert("Angle x", (angle_x));
    box_right->wizardParams.insert("Angle y", (angle_y));
    box_right->wizardParams.insert("Angle z", (angle_z));
    box_right->wizardParams.insert("l", (l - g) * 0.5);
    box_right->wizardParams.insert("b", (b));
    box_right->wizardParams.insert("a", (a));
    box_right->layer = this->layer;
    box_right->processWizardInput();
    box_right->calculate();

    QVector3D pos_up = (matrix_rotation * QVector3D(l/2.0, 0.0, (a + h) * 0.5)) + position;
    box_up->wizardParams.insert("Position x", (pos_up.x()));
    box_up->wizardParams.insert("Position y", (pos_up.y()));
    box_up->wizardParams.insert("Position z", (pos_up.z()));
    box_up->wizardParams.insert("Angle x", (angle_x));
    box_up->wizardParams.insert("Angle y", (angle_y));
    box_up->wizardParams.insert("Angle z", (angle_z));
    box_up->wizardParams.insert("l", (g));
    box_up->wizardParams.insert("b", (b));
    box_up->wizardParams.insert("a", (a - h));
    box_up->layer = this->layer;
    box_up->processWizardInput();
    box_up->calculate();

    this->boundingBox.enterVertices(box_left->boundingBox.getVertices());
    this->boundingBox.enterVertices(box_right->boundingBox.getVertices());
    this->boundingBox.enterVertices(box_up->boundingBox.getVertices());

    this->snap_flanges.append(position + matrix_rotation * QVector3D((l - g) * 0.5, 0.0, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D((l + g) * 0.5, 0.0, 0.0));

    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.0, -0.5 * b, 0.0));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(l  , -0.5 * b, 0.0));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(l  ,  0.5 * b, 0.0));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.0,  0.5 * b, 0.0));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.0, -0.5 * b, a));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(l  , -0.5 * b, a));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(l  ,  0.5 * b, a));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.0,  0.5 * b, a));
}

void CAD_Cleanroom_DoorFrame::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a = wizardParams.value("a").toDouble();
    h = wizardParams.value("h").toDouble();
    b = wizardParams.value("b").toDouble();
    l = wizardParams.value("l").toDouble();
    g = wizardParams.value("g").toDouble();

}

//void CAD_Cleanroom_DoorFrame::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Cleanroom_DoorFrame::rotationOfFlange(quint8 num)
{
    if (num == 2)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(180.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else
        return matrix_rotation;
}
