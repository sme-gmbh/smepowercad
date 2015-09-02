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

#include "cad_cleanroom_doorslidingdouble.h"
#include "glwidget.h"

CAD_Cleanroom_DoorSlidingDouble::CAD_Cleanroom_DoorSlidingDouble() : CADitem(CADitemTypes::Cleanroom_DoorSlidingDouble)
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

    door_left = new CAD_basic_box();
    door_right = new CAD_basic_box();
    arrow_back_11 = new CAD_basic_line();
    arrow_back_12 = new CAD_basic_line();
    arrow_back_13 = new CAD_basic_line();
    arrow_back_14 = new CAD_basic_line();
    arrow_front_11 = new CAD_basic_line();
    arrow_front_12 = new CAD_basic_line();
    arrow_front_13 = new CAD_basic_line();
    arrow_front_14 = new CAD_basic_line();
    arrow_top_11 = new CAD_basic_line();
    arrow_top_12 = new CAD_basic_line();
    arrow_top_13 = new CAD_basic_line();
    arrow_top_14 = new CAD_basic_line();
    arrow_back_21 = new CAD_basic_line();
    arrow_back_22 = new CAD_basic_line();
    arrow_back_23 = new CAD_basic_line();
    arrow_back_24 = new CAD_basic_line();
    arrow_front_21 = new CAD_basic_line();
    arrow_front_22 = new CAD_basic_line();
    arrow_front_23 = new CAD_basic_line();
    arrow_front_24 = new CAD_basic_line();
    arrow_top_21 = new CAD_basic_line();
    arrow_top_22 = new CAD_basic_line();
    arrow_top_23 = new CAD_basic_line();
    arrow_top_24 = new CAD_basic_line();




    this->subItems.append(door_left);
    this->subItems.append(door_right);
    this->subItems.append(arrow_back_11);
    this->subItems.append(arrow_back_12);
    this->subItems.append(arrow_back_13);
    this->subItems.append(arrow_back_14);
    this->subItems.append(arrow_front_11);
    this->subItems.append(arrow_front_12);
    this->subItems.append(arrow_front_13);
    this->subItems.append(arrow_front_14);
    this->subItems.append(arrow_top_11);
    this->subItems.append(arrow_top_12);
    this->subItems.append(arrow_top_13);
    this->subItems.append(arrow_top_14);
    this->subItems.append(arrow_back_21);
    this->subItems.append(arrow_back_22);
    this->subItems.append(arrow_back_23);
    this->subItems.append(arrow_back_24);
    this->subItems.append(arrow_front_21);
    this->subItems.append(arrow_front_22);
    this->subItems.append(arrow_front_23);
    this->subItems.append(arrow_front_24);
    this->subItems.append(arrow_top_21);
    this->subItems.append(arrow_top_22);
    this->subItems.append(arrow_top_23);
    this->subItems.append(arrow_top_24);


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

CAD_Cleanroom_DoorSlidingDouble::~CAD_Cleanroom_DoorSlidingDouble()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Cleanroom_DoorSlidingDouble::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Cleanroom_DoorSlidingDouble::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");   
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Cleanroom_DoorSlidingDouble::iconPath()
{
    return ":/icons/cad_cleanroom/cad_cleanroom_doorslidingdouble.svg";
}

QString CAD_Cleanroom_DoorSlidingDouble::domain()
{
    return "Cleanroom";
}

QString CAD_Cleanroom_DoorSlidingDouble::description()
{
    return "Cleanroom|Door Sliding Double";
}

void CAD_Cleanroom_DoorSlidingDouble::calculate()
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

    QVector3D pos_left = (matrix_rotation * QVector3D(g * 0.2, - 0.55 * b, h * 0.5)) + position;
    door_left->wizardParams.insert("Position x", (pos_left.x()));
    door_left->wizardParams.insert("Position y", (pos_left.y()));
    door_left->wizardParams.insert("Position z", (pos_left.z()));
    door_left->wizardParams.insert("Angle x", (angle_x));
    door_left->wizardParams.insert("Angle y", (angle_y));
    door_left->wizardParams.insert("Angle z", (angle_z));
    door_left->wizardParams.insert("l", (g * 0.5));
    door_left->wizardParams.insert("b", (b * 0.1));
    door_left->wizardParams.insert("a", (h));
    door_left->layer = this->layer;
    door_left->processWizardInput();
    door_left->calculate();

    QVector3D pos_right = (matrix_rotation * QVector3D(g * 0.8, - 0.55 * b, h/2.0)) + position;
    door_right->wizardParams.insert("Position x", (pos_right.x()));
    door_right->wizardParams.insert("Position y", (pos_right.y()));
    door_right->wizardParams.insert("Position z", (pos_right.z()));
    door_right->wizardParams.insert("Angle x", (angle_x));
    door_right->wizardParams.insert("Angle y", (angle_y));
    door_right->wizardParams.insert("Angle z", (angle_z));
    door_right->wizardParams.insert("l", (g * 0.5));
    door_right->wizardParams.insert("b", (b * 0.1));
    door_right->wizardParams.insert("a", (h));
    door_right->layer = this->layer;
    door_right->processWizardInput();
    door_right->calculate();

    //paint arrows
    QVector3D position_arrow_front_11 = position + matrix_rotation * QVector3D(0.1 * g, - 0.6*b - 0.1, 0.5 * h);
    QVector3D position_arrow_front_12 = position + matrix_rotation * QVector3D(0.4 * g, - 0.6*b - 0.1, 0.5 * h);
    QVector3D position_arrow_front_13 = position + matrix_rotation * QVector3D(0.2 * g, - 0.6*b - 0.1, 0.6 * h);
    QVector3D position_arrow_front_14 = position + matrix_rotation * QVector3D(0.2 * g, - 0.6*b - 0.1, 0.4 * h);
    QVector3D position_arrow_back_11 = position + matrix_rotation * QVector3D(0.1 * g, - 0.5*b + 0.1, 0.5 * h);
    QVector3D position_arrow_back_12 = position + matrix_rotation * QVector3D(0.4 * g, - 0.5*b + 0.1, 0.5 * h);
    QVector3D position_arrow_back_13 = position + matrix_rotation * QVector3D(0.2 * g, - 0.5*b + 0.1, 0.6 * h);
    QVector3D position_arrow_back_14 = position + matrix_rotation * QVector3D(0.2 * g, - 0.5*b + 0.1, 0.4 * h);
    QVector3D position_arrow_top_11 = position + matrix_rotation * QVector3D(0.1 * g, - 0.55*b, h + 0.1);
    QVector3D position_arrow_top_12 = position + matrix_rotation * QVector3D(0.4 * g, - 0.55*b, h);
    QVector3D position_arrow_top_13 = position + matrix_rotation * QVector3D(0.2 * g, - 0.4*b , h);
    QVector3D position_arrow_top_14 = position + matrix_rotation * QVector3D(0.2 * g, - 0.7*b, h);

    QVector3D position_arrow_front_21 = position + matrix_rotation * QVector3D(0.9 * g, - 0.6*b - 0.1, 0.5 * h);
    QVector3D position_arrow_front_22 = position + matrix_rotation * QVector3D(0.6 * g, - 0.6*b - 0.1, 0.5 * h);
    QVector3D position_arrow_front_23 = position + matrix_rotation * QVector3D(0.8 * g, - 0.6*b - 0.1, 0.6 * h);
    QVector3D position_arrow_front_24 = position + matrix_rotation * QVector3D(0.8 * g, - 0.6*b - 0.1, 0.4 * h);
    QVector3D position_arrow_back_21 = position + matrix_rotation * QVector3D(0.9 * g, - 0.5*b + 0.1, 0.5 * h);
    QVector3D position_arrow_back_22 = position + matrix_rotation * QVector3D(0.6 * g, - 0.5*b + 0.1, 0.5 * h);
    QVector3D position_arrow_back_23 = position + matrix_rotation * QVector3D(0.8 * g, - 0.5*b + 0.1, 0.6 * h);
    QVector3D position_arrow_back_24 = position + matrix_rotation * QVector3D(0.8 * g, - 0.5*b + 0.1, 0.4 * h);
    QVector3D position_arrow_top_21 = position + matrix_rotation * QVector3D(0.9 * g, - 0.55*b, h + 0.1);
    QVector3D position_arrow_top_22 = position + matrix_rotation * QVector3D(0.6 * g, - 0.55*b, h);
    QVector3D position_arrow_top_23 = position + matrix_rotation * QVector3D(0.8 * g, - 0.4*b , h);
    QVector3D position_arrow_top_24 = position + matrix_rotation * QVector3D(0.8 * g, - 0.7*b, h);


    //********* door on the left ***********************
    //arrow on front
    arrow_front_11->wizardParams.insert("Position x1", position_arrow_front_11.x());
    arrow_front_11->wizardParams.insert("Position y1", position_arrow_front_11.y());
    arrow_front_11->wizardParams.insert("Position z1", position_arrow_front_11.z());
    arrow_front_11->wizardParams.insert("Position x2", position_arrow_front_12.x());
    arrow_front_11->wizardParams.insert("Position y2", position_arrow_front_12.y());
    arrow_front_11->wizardParams.insert("Position z2", position_arrow_front_12.z());
    arrow_front_11->wizardParams.insert("Width", 1.0);
    arrow_front_11->processWizardInput();
    arrow_front_11->calculate();
    arrow_front_11->layer = this->layer;

    arrow_front_12->wizardParams.insert("Position x1", position_arrow_front_11.x());
    arrow_front_12->wizardParams.insert("Position y1", position_arrow_front_11.y());
    arrow_front_12->wizardParams.insert("Position z1", position_arrow_front_11.z());
    arrow_front_12->wizardParams.insert("Position x2", position_arrow_front_13.x());
    arrow_front_12->wizardParams.insert("Position y2", position_arrow_front_13.y());
    arrow_front_12->wizardParams.insert("Position z2", position_arrow_front_13.z());
    arrow_front_12->wizardParams.insert("Width", 1.0);
    arrow_front_12->processWizardInput();
    arrow_front_12->calculate();
    arrow_front_12->layer = this->layer;

    arrow_front_13->wizardParams.insert("Position x1", position_arrow_front_11.x());
    arrow_front_13->wizardParams.insert("Position y1", position_arrow_front_11.y());
    arrow_front_13->wizardParams.insert("Position z1", position_arrow_front_11.z());
    arrow_front_13->wizardParams.insert("Position x2", position_arrow_front_14.x());
    arrow_front_13->wizardParams.insert("Position y2", position_arrow_front_14.y());
    arrow_front_13->wizardParams.insert("Position z2", position_arrow_front_14.z());
    arrow_front_13->wizardParams.insert("Width", 1.0);
    arrow_front_13->processWizardInput();
    arrow_front_13->calculate();
    arrow_front_13->layer = this->layer;

    arrow_front_14->wizardParams.insert("Position x1", position_arrow_front_13.x());
    arrow_front_14->wizardParams.insert("Position y1", position_arrow_front_13.y());
    arrow_front_14->wizardParams.insert("Position z1", position_arrow_front_13.z());
    arrow_front_14->wizardParams.insert("Position x2", position_arrow_front_14.x());
    arrow_front_14->wizardParams.insert("Position y2", position_arrow_front_14.y());
    arrow_front_14->wizardParams.insert("Position z2", position_arrow_front_14.z());
    arrow_front_14->wizardParams.insert("Width", 1.0);
    arrow_front_14->processWizardInput();
    arrow_front_14->calculate();
    arrow_front_14->layer = this->layer;

    //arrow on back
    arrow_back_11->wizardParams.insert("Position x1", position_arrow_back_11.x());
    arrow_back_11->wizardParams.insert("Position y1", position_arrow_back_11.y());
    arrow_back_11->wizardParams.insert("Position z1", position_arrow_back_11.z());
    arrow_back_11->wizardParams.insert("Position x2", position_arrow_back_12.x());
    arrow_back_11->wizardParams.insert("Position y2", position_arrow_back_12.y());
    arrow_back_11->wizardParams.insert("Position z2", position_arrow_back_12.z());
    arrow_back_11->wizardParams.insert("Width", 1.0);
    arrow_back_11->processWizardInput();
    arrow_back_11->calculate();
    arrow_back_11->layer = this->layer;

    arrow_back_12->wizardParams.insert("Position x1", position_arrow_back_11.x());
    arrow_back_12->wizardParams.insert("Position y1", position_arrow_back_11.y());
    arrow_back_12->wizardParams.insert("Position z1", position_arrow_back_11.z());
    arrow_back_12->wizardParams.insert("Position x2", position_arrow_back_13.x());
    arrow_back_12->wizardParams.insert("Position y2", position_arrow_back_13.y());
    arrow_back_12->wizardParams.insert("Position z2", position_arrow_back_13.z());
    arrow_back_12->wizardParams.insert("Width", 1.0);
    arrow_back_12->processWizardInput();
    arrow_back_12->calculate();
    arrow_back_12->layer = this->layer;

    arrow_back_13->wizardParams.insert("Position x1", position_arrow_back_11.x());
    arrow_back_13->wizardParams.insert("Position y1", position_arrow_back_11.y());
    arrow_back_13->wizardParams.insert("Position z1", position_arrow_back_11.z());
    arrow_back_13->wizardParams.insert("Position x2", position_arrow_back_14.x());
    arrow_back_13->wizardParams.insert("Position y2", position_arrow_back_14.y());
    arrow_back_13->wizardParams.insert("Position z2", position_arrow_back_14.z());
    arrow_back_13->wizardParams.insert("Width", 1.0);
    arrow_back_13->processWizardInput();
    arrow_back_13->calculate();
    arrow_back_13->layer = this->layer;

    arrow_back_14->wizardParams.insert("Position x1", position_arrow_back_13.x());
    arrow_back_14->wizardParams.insert("Position y1", position_arrow_back_13.y());
    arrow_back_14->wizardParams.insert("Position z1", position_arrow_back_13.z());
    arrow_back_14->wizardParams.insert("Position x2", position_arrow_back_14.x());
    arrow_back_14->wizardParams.insert("Position y2", position_arrow_back_14.y());
    arrow_back_14->wizardParams.insert("Position z2", position_arrow_back_14.z());
    arrow_back_14->wizardParams.insert("Width", 1.0);
    arrow_back_14->processWizardInput();
    arrow_back_14->calculate();
    arrow_back_14->layer = this->layer;

    //arrow on top
    arrow_top_11->wizardParams.insert("Position x1", position_arrow_top_11.x());
    arrow_top_11->wizardParams.insert("Position y1", position_arrow_top_11.y());
    arrow_top_11->wizardParams.insert("Position z1", position_arrow_top_11.z());
    arrow_top_11->wizardParams.insert("Position x2", position_arrow_top_12.x());
    arrow_top_11->wizardParams.insert("Position y2", position_arrow_top_12.y());
    arrow_top_11->wizardParams.insert("Position z2", position_arrow_top_12.z());
    arrow_top_11->wizardParams.insert("Width", 1.0);
    arrow_top_11->processWizardInput();
    arrow_top_11->calculate();
    arrow_top_11->layer = this->layer;

    arrow_top_12->wizardParams.insert("Position x1", position_arrow_top_11.x());
    arrow_top_12->wizardParams.insert("Position y1", position_arrow_top_11.y());
    arrow_top_12->wizardParams.insert("Position z1", position_arrow_top_11.z());
    arrow_top_12->wizardParams.insert("Position x2", position_arrow_top_13.x());
    arrow_top_12->wizardParams.insert("Position y2", position_arrow_top_13.y());
    arrow_top_12->wizardParams.insert("Position z2", position_arrow_top_13.z());
    arrow_top_12->wizardParams.insert("Width", 1.0);
    arrow_top_12->processWizardInput();
    arrow_top_12->calculate();
    arrow_top_12->layer = this->layer;

    arrow_top_13->wizardParams.insert("Position x1", position_arrow_top_11.x());
    arrow_top_13->wizardParams.insert("Position y1", position_arrow_top_11.y());
    arrow_top_13->wizardParams.insert("Position z1", position_arrow_top_11.z());
    arrow_top_13->wizardParams.insert("Position x2", position_arrow_top_14.x());
    arrow_top_13->wizardParams.insert("Position y2", position_arrow_top_14.y());
    arrow_top_13->wizardParams.insert("Position z2", position_arrow_top_14.z());
    arrow_top_13->wizardParams.insert("Width", 1.0);
    arrow_top_13->processWizardInput();
    arrow_top_13->calculate();
    arrow_top_13->layer = this->layer;

    arrow_top_14->wizardParams.insert("Position x1", position_arrow_top_13.x());
    arrow_top_14->wizardParams.insert("Position y1", position_arrow_top_13.y());
    arrow_top_14->wizardParams.insert("Position z1", position_arrow_top_13.z());
    arrow_top_14->wizardParams.insert("Position x2", position_arrow_top_14.x());
    arrow_top_14->wizardParams.insert("Position y2", position_arrow_top_14.y());
    arrow_top_14->wizardParams.insert("Position z2", position_arrow_top_14.z());
    arrow_top_14->wizardParams.insert("Width", 1.0);
    arrow_top_14->processWizardInput();
    arrow_top_14->calculate();
    arrow_top_14->layer = this->layer;

    //********* door on the right ***********************
    //arrow on front
    arrow_front_21->wizardParams.insert("Position x1", position_arrow_front_21.x());
    arrow_front_21->wizardParams.insert("Position y1", position_arrow_front_21.y());
    arrow_front_21->wizardParams.insert("Position z1", position_arrow_front_21.z());
    arrow_front_21->wizardParams.insert("Position x2", position_arrow_front_22.x());
    arrow_front_21->wizardParams.insert("Position y2", position_arrow_front_22.y());
    arrow_front_21->wizardParams.insert("Position z2", position_arrow_front_22.z());
    arrow_front_21->wizardParams.insert("Width", 1.0);
    arrow_front_21->processWizardInput();
    arrow_front_21->calculate();
    arrow_front_21->layer = this->layer;

    arrow_front_22->wizardParams.insert("Position x1", position_arrow_front_21.x());
    arrow_front_22->wizardParams.insert("Position y1", position_arrow_front_21.y());
    arrow_front_22->wizardParams.insert("Position z1", position_arrow_front_21.z());
    arrow_front_22->wizardParams.insert("Position x2", position_arrow_front_23.x());
    arrow_front_22->wizardParams.insert("Position y2", position_arrow_front_23.y());
    arrow_front_22->wizardParams.insert("Position z2", position_arrow_front_23.z());
    arrow_front_22->wizardParams.insert("Width", 1.0);
    arrow_front_22->processWizardInput();
    arrow_front_22->calculate();
    arrow_front_22->layer = this->layer;

    arrow_front_23->wizardParams.insert("Position x1", position_arrow_front_21.x());
    arrow_front_23->wizardParams.insert("Position y1", position_arrow_front_21.y());
    arrow_front_23->wizardParams.insert("Position z1", position_arrow_front_21.z());
    arrow_front_23->wizardParams.insert("Position x2", position_arrow_front_24.x());
    arrow_front_23->wizardParams.insert("Position y2", position_arrow_front_24.y());
    arrow_front_23->wizardParams.insert("Position z2", position_arrow_front_24.z());
    arrow_front_23->wizardParams.insert("Width", 1.0);
    arrow_front_23->processWizardInput();
    arrow_front_23->calculate();
    arrow_front_23->layer = this->layer;

    arrow_front_24->wizardParams.insert("Position x1", position_arrow_front_23.x());
    arrow_front_24->wizardParams.insert("Position y1", position_arrow_front_23.y());
    arrow_front_24->wizardParams.insert("Position z1", position_arrow_front_23.z());
    arrow_front_24->wizardParams.insert("Position x2", position_arrow_front_24.x());
    arrow_front_24->wizardParams.insert("Position y2", position_arrow_front_24.y());
    arrow_front_24->wizardParams.insert("Position z2", position_arrow_front_24.z());
    arrow_front_24->wizardParams.insert("Width", 1.0);
    arrow_front_24->processWizardInput();
    arrow_front_24->calculate();
    arrow_front_24->layer = this->layer;

    //arrow on back
    arrow_back_21->wizardParams.insert("Position x1", position_arrow_back_21.x());
    arrow_back_21->wizardParams.insert("Position y1", position_arrow_back_21.y());
    arrow_back_21->wizardParams.insert("Position z1", position_arrow_back_21.z());
    arrow_back_21->wizardParams.insert("Position x2", position_arrow_back_22.x());
    arrow_back_21->wizardParams.insert("Position y2", position_arrow_back_22.y());
    arrow_back_21->wizardParams.insert("Position z2", position_arrow_back_22.z());
    arrow_back_21->wizardParams.insert("Width", 1.0);
    arrow_back_21->processWizardInput();
    arrow_back_21->calculate();
    arrow_back_21->layer = this->layer;

    arrow_back_22->wizardParams.insert("Position x1", position_arrow_back_21.x());
    arrow_back_22->wizardParams.insert("Position y1", position_arrow_back_21.y());
    arrow_back_22->wizardParams.insert("Position z1", position_arrow_back_21.z());
    arrow_back_22->wizardParams.insert("Position x2", position_arrow_back_23.x());
    arrow_back_22->wizardParams.insert("Position y2", position_arrow_back_23.y());
    arrow_back_22->wizardParams.insert("Position z2", position_arrow_back_23.z());
    arrow_back_22->wizardParams.insert("Width", 1.0);
    arrow_back_22->processWizardInput();
    arrow_back_22->calculate();
    arrow_back_22->layer = this->layer;

    arrow_back_23->wizardParams.insert("Position x1", position_arrow_back_21.x());
    arrow_back_23->wizardParams.insert("Position y1", position_arrow_back_21.y());
    arrow_back_23->wizardParams.insert("Position z1", position_arrow_back_21.z());
    arrow_back_23->wizardParams.insert("Position x2", position_arrow_back_24.x());
    arrow_back_23->wizardParams.insert("Position y2", position_arrow_back_24.y());
    arrow_back_23->wizardParams.insert("Position z2", position_arrow_back_24.z());
    arrow_back_23->wizardParams.insert("Width", 1.0);
    arrow_back_23->processWizardInput();
    arrow_back_23->calculate();
    arrow_back_23->layer = this->layer;

    arrow_back_24->wizardParams.insert("Position x1", position_arrow_back_23.x());
    arrow_back_24->wizardParams.insert("Position y1", position_arrow_back_23.y());
    arrow_back_24->wizardParams.insert("Position z1", position_arrow_back_23.z());
    arrow_back_24->wizardParams.insert("Position x2", position_arrow_back_24.x());
    arrow_back_24->wizardParams.insert("Position y2", position_arrow_back_24.y());
    arrow_back_24->wizardParams.insert("Position z2", position_arrow_back_24.z());
    arrow_back_24->wizardParams.insert("Width", 1.0);
    arrow_back_24->processWizardInput();
    arrow_back_24->calculate();
    arrow_back_24->layer = this->layer;

    //arrow on top
    arrow_top_21->wizardParams.insert("Position x1", position_arrow_top_21.x());
    arrow_top_21->wizardParams.insert("Position y1", position_arrow_top_21.y());
    arrow_top_21->wizardParams.insert("Position z1", position_arrow_top_21.z());
    arrow_top_21->wizardParams.insert("Position x2", position_arrow_top_22.x());
    arrow_top_21->wizardParams.insert("Position y2", position_arrow_top_22.y());
    arrow_top_21->wizardParams.insert("Position z2", position_arrow_top_22.z());
    arrow_top_21->wizardParams.insert("Width", 1.0);
    arrow_top_21->processWizardInput();
    arrow_top_21->calculate();
    arrow_top_21->layer = this->layer;

    arrow_top_22->wizardParams.insert("Position x1", position_arrow_top_21.x());
    arrow_top_22->wizardParams.insert("Position y1", position_arrow_top_21.y());
    arrow_top_22->wizardParams.insert("Position z1", position_arrow_top_21.z());
    arrow_top_22->wizardParams.insert("Position x2", position_arrow_top_23.x());
    arrow_top_22->wizardParams.insert("Position y2", position_arrow_top_23.y());
    arrow_top_22->wizardParams.insert("Position z2", position_arrow_top_23.z());
    arrow_top_22->wizardParams.insert("Width", 1.0);
    arrow_top_22->processWizardInput();
    arrow_top_22->calculate();
    arrow_top_22->layer = this->layer;

    arrow_top_23->wizardParams.insert("Position x1", position_arrow_top_21.x());
    arrow_top_23->wizardParams.insert("Position y1", position_arrow_top_21.y());
    arrow_top_23->wizardParams.insert("Position z1", position_arrow_top_21.z());
    arrow_top_23->wizardParams.insert("Position x2", position_arrow_top_24.x());
    arrow_top_23->wizardParams.insert("Position y2", position_arrow_top_24.y());
    arrow_top_23->wizardParams.insert("Position z2", position_arrow_top_24.z());
    arrow_top_23->wizardParams.insert("Width", 1.0);
    arrow_top_23->processWizardInput();
    arrow_top_23->calculate();
    arrow_top_23->layer = this->layer;

    arrow_top_24->wizardParams.insert("Position x1", position_arrow_top_23.x());
    arrow_top_24->wizardParams.insert("Position y1", position_arrow_top_23.y());
    arrow_top_24->wizardParams.insert("Position z1", position_arrow_top_23.z());
    arrow_top_24->wizardParams.insert("Position x2", position_arrow_top_24.x());
    arrow_top_24->wizardParams.insert("Position y2", position_arrow_top_24.y());
    arrow_top_24->wizardParams.insert("Position z2", position_arrow_top_24.z());
    arrow_top_24->wizardParams.insert("Width", 1.0);
    arrow_top_24->processWizardInput();
    arrow_top_24->calculate();
    arrow_top_24->layer = this->layer;


    this->boundingBox.enterVertices(door_left->boundingBox.getVertices());
    this->boundingBox.enterVertices(door_right->boundingBox.getVertices());
}

void CAD_Cleanroom_DoorSlidingDouble::processWizardInput()
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
}

//void CAD_cleanroom_DoorSlidingDouble::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Cleanroom_DoorSlidingDouble::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}
