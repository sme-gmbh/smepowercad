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

#include "cad_cleanroom_doorswingingsingle.h"

CAD_Cleanroom_DoorSwingingSingle::CAD_Cleanroom_DoorSwingingSingle() : CADitem(CADitemTypes::Cleanroom_DoorSwingingSingle)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("h",  2500.0);
    wizardParams.insert("g",  2000.0);
    wizardParams.insert("b",   100.0);
    wizardParams.insert("alpha", -45.0);

    door = new CAD_basic_box();
    arrow_1 = new CAD_basic_line();
    arrow_2 = new CAD_basic_line();
    arc = new CAD_basic_arc();

    this->subItems.append(door);
    this->subItems.append(arrow_1);
    this->subItems.append(arrow_2);
    this->subItems.append(arc);
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

CAD_Cleanroom_DoorSwingingSingle::~CAD_Cleanroom_DoorSwingingSingle()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Cleanroom_DoorSwingingSingle::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Cleanroom_DoorSwingingSingle::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");     
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Cleanroom_DoorSwingingSingle::iconPath()
{
    return ":/icons/cad_cleanroom/cad_cleanroom_doorswingingsingle.svg";
}

QString CAD_Cleanroom_DoorSwingingSingle::domain()
{
    return "Cleanroom";
}

QString CAD_Cleanroom_DoorSwingingSingle::description()
{
    return "Cleanroom|Door Swinging Single";
}

void CAD_Cleanroom_DoorSwingingSingle::calculate()
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

    QVector3D position_door;
    QVector3D position_arc;
    if(alpha < 0.0)
    {
        QMatrix4x4 matrix_door;
        matrix_door.setToIdentity();
        matrix_door.rotate(alpha, 0.0, 0.0, 1.0);
        position_arc = position + matrix_rotation * QVector3D(0.0, -b/2, 0.02 * h);
        position_door = position + matrix_rotation * (QVector3D(0.0, -b/2 , 0.0) + matrix_door * QVector3D(g/2, 0.0, h / 2));

        //paint arrow tips
        QVector3D pos_start_1 = position + matrix_rotation * QVector3D(0.0, 0.0, h);
        QVector3D pos_end_1 = position + matrix_rotation * QVector3D(g, 0.0,  0.5 * h);
        arrow_1->wizardParams.insert("Position x1", pos_start_1.x());
        arrow_1->wizardParams.insert("Position y1", pos_start_1.y());
        arrow_1->wizardParams.insert("Position z1", pos_start_1.z());
        arrow_1->wizardParams.insert("Position x2", pos_end_1.x());
        arrow_1->wizardParams.insert("Position y2", pos_end_1.y());
        arrow_1->wizardParams.insert("Position z2", pos_end_1.z());
        arrow_1->wizardParams.insert("Width", 1.0);
        arrow_1->processWizardInput();
        arrow_1->calculate();
        arrow_1->layer = this->layer;

        arrow_2->wizardParams.insert("Position x1", position.x());
        arrow_2->wizardParams.insert("Position y1", position.y());
        arrow_2->wizardParams.insert("Position z1", position.z());
        arrow_2->wizardParams.insert("Position x2", pos_end_1.x());
        arrow_2->wizardParams.insert("Position y2", pos_end_1.y());
        arrow_2->wizardParams.insert("Position z2", pos_end_1.z());
        arrow_2->wizardParams.insert("Width", 1.0);
        arrow_2->processWizardInput();
        arrow_2->calculate();
        arrow_2->layer = this->layer;
    }
    else
    {
        QMatrix4x4 matrix_door;
        matrix_door.setToIdentity();
        matrix_door.rotate(alpha, 0.0, 0.0, 1.0);
        position_arc = position + matrix_rotation * QVector3D(g, -b/2, 0.02 * h);
        position_door = position + matrix_rotation * (QVector3D(g, -b/2, 0.0) + matrix_door * QVector3D(-g/2 , -0.0, h / 2));

        //paint arrow tips
        QVector3D pos_start_1 = position + matrix_rotation * QVector3D(g, 0.0, h);
        QVector3D pos_end_1 = position + matrix_rotation * QVector3D(0.0, 0.0,  0.5 * h);
        arrow_1->wizardParams.insert("Position x1", pos_start_1.x());
        arrow_1->wizardParams.insert("Position y1", pos_start_1.y());
        arrow_1->wizardParams.insert("Position z1", pos_start_1.z());
        arrow_1->wizardParams.insert("Position x2", pos_end_1.x());
        arrow_1->wizardParams.insert("Position y2", pos_end_1.y());
        arrow_1->wizardParams.insert("Position z2", pos_end_1.z());
        arrow_1->wizardParams.insert("Width", 1.0);
        arrow_1->processWizardInput();
        arrow_1->calculate();
        arrow_1->layer = this->layer;

        QVector3D pos_start_2 = position + matrix_rotation * QVector3D(g, 0.0, 0.0);
        arrow_2->wizardParams.insert("Position x1", pos_start_2.x());
        arrow_2->wizardParams.insert("Position y1", pos_start_2.y());
        arrow_2->wizardParams.insert("Position z1", pos_start_2.z());
        arrow_2->wizardParams.insert("Position x2", pos_end_1.x());
        arrow_2->wizardParams.insert("Position y2", pos_end_1.y());
        arrow_2->wizardParams.insert("Position z2", pos_end_1.z());
        arrow_2->wizardParams.insert("Width", 1.0);
        arrow_2->processWizardInput();
        arrow_2->calculate();
        arrow_2->layer = this->layer;
    }

    door->wizardParams.insert("Position x", position_door.x());
    door->wizardParams.insert("Position y", position_door.y());
    door->wizardParams.insert("Position z", position_door.z());
    door->wizardParams.insert("Angle x", angle_x);
    door->wizardParams.insert("Angle y", angle_y);
    door->wizardParams.insert("Angle z", angle_z + alpha);

    door->wizardParams.insert("l", g);
    door->wizardParams.insert("b", 0.1*b);
    door->wizardParams.insert("a", h);
    door->layer = this->layer;
    door->processWizardInput();
    door->calculate();

    arc->wizardParams.insert("Position x", position_arc.x());
    arc->wizardParams.insert("Position y", position_arc.y());
    arc->wizardParams.insert("Position z", position_arc.z());
    arc->wizardParams.insert("Angle x", angle_x);
    arc->wizardParams.insert("Angle y", angle_y);
    arc->wizardParams.insert("Angle z", angle_z);
    arc->wizardParams.insert("r", g);
    arc->wizardParams.insert("alpha", alpha);
    arc->layer = this->layer;
    arc->processWizardInput();
    if(alpha < 0.0)
        arc->rotateAroundAxis(-90 + alpha, QVector3D(0.0, 0.0, 1.0), angle_x, angle_y, angle_z);
    else
        arc->rotateAroundAxis(-270.0 + alpha, QVector3D(0.0, 0.0, 1.0), angle_x, angle_y, angle_z);
    arc->calculate();

    this->boundingBox = door->boundingBox;
    this->boundingBox.enterVertices(arc->boundingBox.getVertices());
}

void CAD_Cleanroom_DoorSwingingSingle::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    h = wizardParams.value("h").toDouble();
    b = wizardParams.value("b").toDouble();
    g = wizardParams.value("g").toDouble();
    alpha = wizardParams.value("alpha").toDouble();
}

//void CAD_cleanroom_DoorSwingingSingle::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Cleanroom_DoorSwingingSingle::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}
