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

#include "cad_cleanroom_doorslidingsingle.h"
#include "glwidget.h"

CAD_Cleanroom_DoorSlidingSingle::CAD_Cleanroom_DoorSlidingSingle() : CADitem(CADitemTypes::Cleanroom_DoorSlidingSingle)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("h", 2100.0);
    wizardParams.insert("b",  100.0);
    wizardParams.insert("g", 1000.0);
    wizardParams.insertComboBox("Opening direction", QStringList() << "Left" << "Right", "Left");

    door = new CAD_basic_box();
    arrow_back_1 = new CAD_basic_line();
    arrow_back_2 = new CAD_basic_line();
    arrow_back_3 = new CAD_basic_line();
    arrow_back_4 = new CAD_basic_line();
    arrow_front_1 = new CAD_basic_line();
    arrow_front_2 = new CAD_basic_line();
    arrow_front_3 = new CAD_basic_line();
    arrow_front_4 = new CAD_basic_line();
    arrow_top_1 = new CAD_basic_line();
    arrow_top_2 = new CAD_basic_line();
    arrow_top_3 = new CAD_basic_line();
    arrow_top_4 = new CAD_basic_line();

    this->subItems.append(door);
    this->subItems.append(arrow_back_1);
    this->subItems.append(arrow_back_2);
    this->subItems.append(arrow_back_3);
    this->subItems.append(arrow_back_4);
    this->subItems.append(arrow_front_1);
    this->subItems.append(arrow_front_2);
    this->subItems.append(arrow_front_3);
    this->subItems.append(arrow_front_4);
    this->subItems.append(arrow_top_1);
    this->subItems.append(arrow_top_2);
    this->subItems.append(arrow_top_3);
    this->subItems.append(arrow_top_4);

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

CAD_Cleanroom_DoorSlidingSingle::~CAD_Cleanroom_DoorSlidingSingle()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Cleanroom_DoorSlidingSingle::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Cleanroom_DoorSlidingSingle::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");     
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Cleanroom_DoorSlidingSingle::iconPath()
{
    return ":/icons/cad_cleanroom/cad_cleanroom_doorslidingsingle.svg";
}

QString CAD_Cleanroom_DoorSlidingSingle::domain()
{
    return "Cleanroom";
}

QString CAD_Cleanroom_DoorSlidingSingle::description()
{
    return "Cleanroom|Door Sliding Single";
}

void CAD_Cleanroom_DoorSlidingSingle::calculate()
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

    QVector3D pos;
    if(opensToLeft)
        pos = (matrix_rotation * QVector3D(g * 0.4, - 0.55 * b, h * 0.5)) + position;
    else
        pos = (matrix_rotation * QVector3D(g * 0.6, - 0.55 * b, h * 0.5)) + position;
    door->wizardParams.insert("Position x", (pos.x()));
    door->wizardParams.insert("Position y", (pos.y()));
    door->wizardParams.insert("Position z", (pos.z()));
    door->wizardParams.insert("Angle x", (angle_x));
    door->wizardParams.insert("Angle y", (angle_y));
    door->wizardParams.insert("Angle z", (angle_z));
    door->wizardParams.insert("l", (g));
    door->wizardParams.insert("b", (b * 0.1));
    door->wizardParams.insert("a", (h));
    door->layer = this->layer;
    door->processWizardInput();
    door->calculate();

    //paint arrows
    QVector3D position_arrow_front_1;
    QVector3D position_arrow_front_2;
    QVector3D position_arrow_front_3;
    QVector3D position_arrow_front_4;
    QVector3D position_arrow_back_1;
    QVector3D position_arrow_back_2;
    QVector3D position_arrow_back_3;
    QVector3D position_arrow_back_4;
    QVector3D position_arrow_top_1;
    QVector3D position_arrow_top_2;
    QVector3D position_arrow_top_3;
    QVector3D position_arrow_top_4;
    if(opensToLeft)
    {
        position_arrow_front_1 = position + matrix_rotation * QVector3D(0.2 * g, - 0.6*b - 0.1, 0.5 * h);
        position_arrow_front_2 = position + matrix_rotation * QVector3D(0.8 * g, - 0.6*b - 0.1, 0.5 * h);
        position_arrow_front_3 = position + matrix_rotation * QVector3D(0.3 * g, - 0.6*b - 0.1, 0.6 * h);
        position_arrow_front_4 = position + matrix_rotation * QVector3D(0.3 * g, - 0.6*b - 0.1, 0.4 * h);
        position_arrow_back_1 = position + matrix_rotation * QVector3D(0.2 * g, - 0.5*b + 0.1, 0.5 * h);
        position_arrow_back_2 = position + matrix_rotation * QVector3D(0.8 * g, - 0.5*b + 0.1, 0.5 * h);
        position_arrow_back_3 = position + matrix_rotation * QVector3D(0.3 * g, - 0.5*b + 0.1, 0.6 * h);
        position_arrow_back_4 = position + matrix_rotation * QVector3D(0.3 * g, - 0.5*b + 0.1, 0.4 * h);
        position_arrow_top_1 = position + matrix_rotation * QVector3D(0.2 * g, - 0.55*b, h + 0.1);
        position_arrow_top_2 = position + matrix_rotation * QVector3D(0.8 * g, - 0.55*b, h);
        position_arrow_top_3 = position + matrix_rotation * QVector3D(0.3 * g, - 0.4*b , h);
        position_arrow_top_4 = position + matrix_rotation * QVector3D(0.3 * g, - 0.7*b, h);
    }
    else
    {
        position_arrow_front_1 = position + matrix_rotation * QVector3D(0.8 * g, - 0.6*b - 0.1, 0.5 * h);
        position_arrow_front_2 = position + matrix_rotation * QVector3D(0.2 * g, - 0.6*b - 0.1, 0.5 * h);
        position_arrow_front_3 = position + matrix_rotation * QVector3D(0.7 * g, - 0.6*b - 0.1, 0.6 * h);
        position_arrow_front_4 = position + matrix_rotation * QVector3D(0.7 * g, - 0.6*b - 0.1, 0.4 * h);
        position_arrow_back_1 = position + matrix_rotation * QVector3D(0.8 * g, - 0.5*b + 0.1, 0.5 * h);
        position_arrow_back_2 = position + matrix_rotation * QVector3D(0.2 * g, - 0.5*b + 0.1, 0.5 * h);
        position_arrow_back_3 = position + matrix_rotation * QVector3D(0.7 * g, - 0.5*b + 0.1, 0.6 * h);
        position_arrow_back_4 = position + matrix_rotation * QVector3D(0.7 * g, - 0.5*b + 0.1, 0.4 * h);
        position_arrow_top_1 = position + matrix_rotation * QVector3D(0.8 * g, - 0.55*b, h + 0.1);
        position_arrow_top_2 = position + matrix_rotation * QVector3D(0.2 * g, - 0.55*b, h);
        position_arrow_top_3 = position + matrix_rotation * QVector3D(0.7 * g, - 0.4*b , h);
        position_arrow_top_4 = position + matrix_rotation * QVector3D(0.7 * g, - 0.7*b, h);
    }

    //arrow on front
    arrow_front_1->wizardParams.insert("Position x1", position_arrow_front_1.x());
    arrow_front_1->wizardParams.insert("Position y1", position_arrow_front_1.y());
    arrow_front_1->wizardParams.insert("Position z1", position_arrow_front_1.z());
    arrow_front_1->wizardParams.insert("Position x2", position_arrow_front_2.x());
    arrow_front_1->wizardParams.insert("Position y2", position_arrow_front_2.y());
    arrow_front_1->wizardParams.insert("Position z2", position_arrow_front_2.z());
    arrow_front_1->wizardParams.insert("Width", 1.0);
    arrow_front_1->processWizardInput();
    arrow_front_1->calculate();
    arrow_front_1->layer = this->layer;

    arrow_front_2->wizardParams.insert("Position x1", position_arrow_front_1.x());
    arrow_front_2->wizardParams.insert("Position y1", position_arrow_front_1.y());
    arrow_front_2->wizardParams.insert("Position z1", position_arrow_front_1.z());
    arrow_front_2->wizardParams.insert("Position x2", position_arrow_front_3.x());
    arrow_front_2->wizardParams.insert("Position y2", position_arrow_front_3.y());
    arrow_front_2->wizardParams.insert("Position z2", position_arrow_front_3.z());
    arrow_front_2->wizardParams.insert("Width", 1.0);
    arrow_front_2->processWizardInput();
    arrow_front_2->calculate();
    arrow_front_2->layer = this->layer;

    arrow_front_3->wizardParams.insert("Position x1", position_arrow_front_1.x());
    arrow_front_3->wizardParams.insert("Position y1", position_arrow_front_1.y());
    arrow_front_3->wizardParams.insert("Position z1", position_arrow_front_1.z());
    arrow_front_3->wizardParams.insert("Position x2", position_arrow_front_4.x());
    arrow_front_3->wizardParams.insert("Position y2", position_arrow_front_4.y());
    arrow_front_3->wizardParams.insert("Position z2", position_arrow_front_4.z());
    arrow_front_3->wizardParams.insert("Width", 1.0);
    arrow_front_3->processWizardInput();
    arrow_front_3->calculate();
    arrow_front_3->layer = this->layer;

    arrow_front_4->wizardParams.insert("Position x1", position_arrow_front_3.x());
    arrow_front_4->wizardParams.insert("Position y1", position_arrow_front_3.y());
    arrow_front_4->wizardParams.insert("Position z1", position_arrow_front_3.z());
    arrow_front_4->wizardParams.insert("Position x2", position_arrow_front_4.x());
    arrow_front_4->wizardParams.insert("Position y2", position_arrow_front_4.y());
    arrow_front_4->wizardParams.insert("Position z2", position_arrow_front_4.z());
    arrow_front_4->wizardParams.insert("Width", 1.0);
    arrow_front_4->processWizardInput();
    arrow_front_4->calculate();
    arrow_front_4->layer = this->layer;

    //arrow on back
    arrow_back_1->wizardParams.insert("Position x1", position_arrow_back_1.x());
    arrow_back_1->wizardParams.insert("Position y1", position_arrow_back_1.y());
    arrow_back_1->wizardParams.insert("Position z1", position_arrow_back_1.z());
    arrow_back_1->wizardParams.insert("Position x2", position_arrow_back_2.x());
    arrow_back_1->wizardParams.insert("Position y2", position_arrow_back_2.y());
    arrow_back_1->wizardParams.insert("Position z2", position_arrow_back_2.z());
    arrow_back_1->wizardParams.insert("Width", 1.0);
    arrow_back_1->processWizardInput();
    arrow_back_1->calculate();
    arrow_back_1->layer = this->layer;

    arrow_back_2->wizardParams.insert("Position x1", position_arrow_back_1.x());
    arrow_back_2->wizardParams.insert("Position y1", position_arrow_back_1.y());
    arrow_back_2->wizardParams.insert("Position z1", position_arrow_back_1.z());
    arrow_back_2->wizardParams.insert("Position x2", position_arrow_back_3.x());
    arrow_back_2->wizardParams.insert("Position y2", position_arrow_back_3.y());
    arrow_back_2->wizardParams.insert("Position z2", position_arrow_back_3.z());
    arrow_back_2->wizardParams.insert("Width", 1.0);
    arrow_back_2->processWizardInput();
    arrow_back_2->calculate();
    arrow_back_2->layer = this->layer;

    arrow_back_3->wizardParams.insert("Position x1", position_arrow_back_1.x());
    arrow_back_3->wizardParams.insert("Position y1", position_arrow_back_1.y());
    arrow_back_3->wizardParams.insert("Position z1", position_arrow_back_1.z());
    arrow_back_3->wizardParams.insert("Position x2", position_arrow_back_4.x());
    arrow_back_3->wizardParams.insert("Position y2", position_arrow_back_4.y());
    arrow_back_3->wizardParams.insert("Position z2", position_arrow_back_4.z());
    arrow_back_3->wizardParams.insert("Width", 1.0);
    arrow_back_3->processWizardInput();
    arrow_back_3->calculate();
    arrow_back_3->layer = this->layer;

    arrow_back_4->wizardParams.insert("Position x1", position_arrow_back_3.x());
    arrow_back_4->wizardParams.insert("Position y1", position_arrow_back_3.y());
    arrow_back_4->wizardParams.insert("Position z1", position_arrow_back_3.z());
    arrow_back_4->wizardParams.insert("Position x2", position_arrow_back_4.x());
    arrow_back_4->wizardParams.insert("Position y2", position_arrow_back_4.y());
    arrow_back_4->wizardParams.insert("Position z2", position_arrow_back_4.z());
    arrow_back_4->wizardParams.insert("Width", 1.0);
    arrow_back_4->processWizardInput();
    arrow_back_4->calculate();
    arrow_back_4->layer = this->layer;

    //arrow on top
    arrow_top_1->wizardParams.insert("Position x1", position_arrow_top_1.x());
    arrow_top_1->wizardParams.insert("Position y1", position_arrow_top_1.y());
    arrow_top_1->wizardParams.insert("Position z1", position_arrow_top_1.z());
    arrow_top_1->wizardParams.insert("Position x2", position_arrow_top_2.x());
    arrow_top_1->wizardParams.insert("Position y2", position_arrow_top_2.y());
    arrow_top_1->wizardParams.insert("Position z2", position_arrow_top_2.z());
    arrow_top_1->wizardParams.insert("Width", 1.0);
    arrow_top_1->processWizardInput();
    arrow_top_1->calculate();
    arrow_top_1->layer = this->layer;

    arrow_top_2->wizardParams.insert("Position x1", position_arrow_top_1.x());
    arrow_top_2->wizardParams.insert("Position y1", position_arrow_top_1.y());
    arrow_top_2->wizardParams.insert("Position z1", position_arrow_top_1.z());
    arrow_top_2->wizardParams.insert("Position x2", position_arrow_top_3.x());
    arrow_top_2->wizardParams.insert("Position y2", position_arrow_top_3.y());
    arrow_top_2->wizardParams.insert("Position z2", position_arrow_top_3.z());
    arrow_top_2->wizardParams.insert("Width", 1.0);
    arrow_top_2->processWizardInput();
    arrow_top_2->calculate();
    arrow_top_2->layer = this->layer;

    arrow_top_3->wizardParams.insert("Position x1", position_arrow_top_1.x());
    arrow_top_3->wizardParams.insert("Position y1", position_arrow_top_1.y());
    arrow_top_3->wizardParams.insert("Position z1", position_arrow_top_1.z());
    arrow_top_3->wizardParams.insert("Position x2", position_arrow_top_4.x());
    arrow_top_3->wizardParams.insert("Position y2", position_arrow_top_4.y());
    arrow_top_3->wizardParams.insert("Position z2", position_arrow_top_4.z());
    arrow_top_3->wizardParams.insert("Width", 1.0);
    arrow_top_3->processWizardInput();
    arrow_top_3->calculate();
    arrow_top_3->layer = this->layer;

    arrow_top_4->wizardParams.insert("Position x1", position_arrow_top_3.x());
    arrow_top_4->wizardParams.insert("Position y1", position_arrow_top_3.y());
    arrow_top_4->wizardParams.insert("Position z1", position_arrow_top_3.z());
    arrow_top_4->wizardParams.insert("Position x2", position_arrow_top_4.x());
    arrow_top_4->wizardParams.insert("Position y2", position_arrow_top_4.y());
    arrow_top_4->wizardParams.insert("Position z2", position_arrow_top_4.z());
    arrow_top_4->wizardParams.insert("Width", 1.0);
    arrow_top_4->processWizardInput();
    arrow_top_4->calculate();
    arrow_top_4->layer = this->layer;


    this->boundingBox = door->boundingBox;
}

void CAD_Cleanroom_DoorSlidingSingle::processWizardInput()
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

    opensToLeft = (wizardParams.value("Opening direction").toStringList().at(1) == "Left");
    qDebug() << opensToLeft;
}

//void CAD_cleanroom_DoorSlidingSingle::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Cleanroom_DoorSlidingSingle::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}
