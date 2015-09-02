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

#include "cad_cleanroom_doorswingingdouble.h"
#include "glwidget.h"

CAD_Cleanroom_DoorSwingingDouble::CAD_Cleanroom_DoorSwingingDouble() : CADitem(CADitemTypes::Cleanroom_DoorSwingingDouble)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("h",  2500.0);
    wizardParams.insert("g",  1300.0);
    wizardParams.insert("g1",   700.0);
    wizardParams.insert("b",   100.0);
    wizardParams.insert("alpha left", 45.0);
    wizardParams.insert("alpha right", 45.0);

    door_left = new CAD_basic_box();
    door_right = new CAD_basic_box();
    arrow_left_1 = new CAD_basic_line();
    arrow_left_2 = new CAD_basic_line();
    arrow_right_1 = new CAD_basic_line();
    arrow_right_2 = new CAD_basic_line();
    arc_left = new CAD_basic_arc();
    arc_right = new CAD_basic_arc();


    this->subItems.append(door_left);
    this->subItems.append(door_right);
    this->subItems.append(arrow_left_1);
    this->subItems.append(arrow_left_2);
    this->subItems.append(arrow_right_1);
    this->subItems.append(arrow_right_2);
    this->subItems.append(arc_left);
    this->subItems.append(arc_right);

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

CAD_Cleanroom_DoorSwingingDouble::~CAD_Cleanroom_DoorSwingingDouble()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Cleanroom_DoorSwingingDouble::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Cleanroom_DoorSwingingDouble::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");     
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Cleanroom_DoorSwingingDouble::iconPath()
{
    return ":/icons/cad_cleanroom/cad_cleanroom_doorswingingdouble.svg";
}

QString CAD_Cleanroom_DoorSwingingDouble::domain()
{
    return "Cleanroom";
}

QString CAD_Cleanroom_DoorSwingingDouble::description()
{
    return "Cleanroom|Door Swinging Double";
}

void CAD_Cleanroom_DoorSwingingDouble::calculate()
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

    QMatrix4x4 matrix_door_left;
    matrix_door_left.setToIdentity();
    matrix_door_left.rotate(-alpha1, 0.0, 0.0, 1.0);
    QVector3D position_door_left = position + matrix_rotation * (QVector3D(0.0, - b * 0.5, 0.0) + matrix_door_left * QVector3D(g1/2, - 0.05 * b, h/2));

    door_left->wizardParams.insert("Position x", position_door_left.x());
    door_left->wizardParams.insert("Position y", position_door_left.y());
    door_left->wizardParams.insert("Position z", position_door_left.z());
    door_left->wizardParams.insert("Angle x", angle_x);
    door_left->wizardParams.insert("Angle y", angle_y);
    door_left->wizardParams.insert("Angle z", angle_z - alpha1);

    door_left->wizardParams.insert("l", g1);
    door_left->wizardParams.insert("b", 0.1*b);
    door_left->wizardParams.insert("a", h);
    door_left->layer = this->layer;
    door_left->processWizardInput();
    door_left->calculate();

    QMatrix4x4 matrix_door_right;
    matrix_door_right.setToIdentity();
    matrix_door_right.rotate(alpha2, 0.0, 0.0, 1.0);
    QVector3D position_door_right = position + matrix_rotation * (QVector3D(g, - b * 0.5, 0.0) + matrix_door_right * QVector3D(-(g - g1) * 0.5, -0.05 * b, h/2));

    door_right->wizardParams.insert("Position x", position_door_right.x());
    door_right->wizardParams.insert("Position y", position_door_right.y());
    door_right->wizardParams.insert("Position z", position_door_right.z());
    door_right->wizardParams.insert("Angle x", angle_x);
    door_right->wizardParams.insert("Angle y", angle_y);
    door_right->wizardParams.insert("Angle z", angle_z + alpha2);
    door_right->wizardParams.insert("l", g - g1);
    door_right->wizardParams.insert("b", 0.1 * b);
    door_right->wizardParams.insert("a", h);
    door_right->layer = this->layer;
    door_right->processWizardInput();
    door_right->calculate();

    //paint arrow tips
    QVector3D pos_end_1 = position + matrix_rotation * QVector3D(g1, 0.0,  0.5 * h);
    arrow_left_1->wizardParams.insert("Position x1", position.x());
    arrow_left_1->wizardParams.insert("Position y1", position.y());
    arrow_left_1->wizardParams.insert("Position z1", position.z());
    arrow_left_1->wizardParams.insert("Position x2", pos_end_1.x());
    arrow_left_1->wizardParams.insert("Position y2", pos_end_1.y());
    arrow_left_1->wizardParams.insert("Position z2", pos_end_1.z());
    arrow_left_1->wizardParams.insert("Width", 1.0);
    arrow_left_1->processWizardInput();
    arrow_left_1->calculate();
    arrow_left_1->layer = this->layer;

    QVector3D pos_start_left_2 = position + matrix_rotation * QVector3D(0.0, 0.0, h);
    arrow_left_2->wizardParams.insert("Position x1", pos_start_left_2.x());
    arrow_left_2->wizardParams.insert("Position y1", pos_start_left_2.y());
    arrow_left_2->wizardParams.insert("Position z1", pos_start_left_2.z());
    arrow_left_2->wizardParams.insert("Position x2", pos_end_1.x());
    arrow_left_2->wizardParams.insert("Position y2", pos_end_1.y());
    arrow_left_2->wizardParams.insert("Position z2", pos_end_1.z());
    arrow_left_2->wizardParams.insert("Width", 1.0);
    arrow_left_2->processWizardInput();
    arrow_left_2->calculate();
    arrow_left_2->layer = this->layer;

    QVector3D pos_start_right_1 = position + matrix_rotation * QVector3D(g, 0.0, 0.0);
    arrow_right_1->wizardParams.insert("Position x1", pos_start_right_1.x());
    arrow_right_1->wizardParams.insert("Position y1", pos_start_right_1.y());
    arrow_right_1->wizardParams.insert("Position z1", pos_start_right_1.z());
    arrow_right_1->wizardParams.insert("Position x2", pos_end_1.x());
    arrow_right_1->wizardParams.insert("Position y2", pos_end_1.y());
    arrow_right_1->wizardParams.insert("Position z2", pos_end_1.z());
    arrow_right_1->wizardParams.insert("Width", 1.0);
    arrow_right_1->processWizardInput();
    arrow_right_1->calculate();
    arrow_right_1->layer = this->layer;

    QVector3D pos_start_right_2 = position + matrix_rotation * QVector3D(g, 0.0, h);
    arrow_right_2->wizardParams.insert("Position x1", pos_start_right_2.x());
    arrow_right_2->wizardParams.insert("Position y1", pos_start_right_2.y());
    arrow_right_2->wizardParams.insert("Position z1", pos_start_right_2.z());
    arrow_right_2->wizardParams.insert("Position x2", pos_end_1.x());
    arrow_right_2->wizardParams.insert("Position y2", pos_end_1.y());
    arrow_right_2->wizardParams.insert("Position z2", pos_end_1.z());
    arrow_right_2->wizardParams.insert("Width", 1.0);
    arrow_right_2->processWizardInput();
    arrow_right_2->calculate();
    arrow_right_2->layer = this->layer;

    QVector3D position_arc_left = position + matrix_rotation * QVector3D(0.0, - 0.5 * b, 0.0);
    arc_left->wizardParams.insert("Position x", position_arc_left.x());
    arc_left->wizardParams.insert("Position y", position_arc_left.y());
    arc_left->wizardParams.insert("Position z", position_arc_left.z());
    arc_left->wizardParams.insert("Angle x", angle_x);
    arc_left->wizardParams.insert("Angle y", angle_y);
    arc_left->wizardParams.insert("Angle z", angle_z);
    arc_left->wizardParams.insert("r", g1);
    arc_left->wizardParams.insert("alpha", alpha1);
    arc_left->layer = this->layer;
    arc_left->processWizardInput();
    arc_left->rotateAroundAxis(-90.0, QVector3D(0.0, 0.0, 1.0), angle_x, angle_y, angle_z);
    arc_left->calculate();

    QVector3D position_arc_right = position + matrix_rotation * QVector3D(g, - 0.5 * b, 0.0);
    arc_right->wizardParams.insert("Position x", position_arc_right.x());
    arc_right->wizardParams.insert("Position y", position_arc_right.y());
    arc_right->wizardParams.insert("Position z", position_arc_right.z());
    arc_right->wizardParams.insert("Angle x", angle_x);
    arc_right->wizardParams.insert("Angle y", angle_y);
    arc_right->wizardParams.insert("Angle z", angle_z);
    arc_right->wizardParams.insert("r", g - g1);
    arc_right->wizardParams.insert("alpha", alpha2);
    arc_right->layer = this->layer;
    arc_right->processWizardInput();
    arc_right->rotateAroundAxis(-270.0 + alpha2, QVector3D(0.0, 0.0, 1.0), angle_x, angle_y, angle_z);
    arc_right->calculate();


    this->boundingBox.enterVertices(door_left->boundingBox.getVertices());
    this->boundingBox.enterVertices(door_right->boundingBox.getVertices());

}

void CAD_Cleanroom_DoorSwingingDouble::processWizardInput()
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
    g1 = wizardParams.value("g1").toDouble();
    alpha1 = wizardParams.value("alpha left").toDouble();
    alpha2 = wizardParams.value("alpha right").toDouble();
}

//void CAD_cleanroom_DoorSwingingDouble::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Cleanroom_DoorSwingingDouble::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}
