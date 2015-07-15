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

#include "cad_cleanroom_ceilingverticalladder.h"
#include "glwidget.h"

CAD_Cleanroom_CeilingVerticalLadder::CAD_Cleanroom_CeilingVerticalLadder() : CADitem(CADitemTypes::Cleanroom_CeilingVerticalLadder)
{
    left = new CAD_basic_box;
    right = new CAD_basic_box;
    left_down = new CAD_basic_box;
    right_down = new CAD_basic_box;
    left_horizontal = new CAD_basic_box;
    right_horizontal = new CAD_basic_box;
    this->subItems.append(left);
    this->subItems.append(right);
    this->subItems.append(left_down);
    this->subItems.append(right_down);
    this->subItems.append(left_horizontal);
    this->subItems.append(right_horizontal);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",   50.0);    // Breite (in y-Richtung)
    wizardParams.insert("a2", 500.0);    // Überhang Handlauf (in y-Richtung)
    wizardParams.insert("b",  500.0);    // Tiefe (in x-Richtung)
    wizardParams.insert("g2",  50.0);    // Tiefe der Randteile
    wizardParams.insert("l", 3000.0);    // Höhe (in z-Richtung)
    wizardParams.insert("l2", 300.0);    // Abstand der Sprossen (in z-Richtung)
    wizardParams.insert("l3",  20.0);    // Höhe der Sprossen (in z-Richtung)
    wizardParams.insert("l4", 500.0);    // Überhang Handlauf (in z-Richtung)
    wizardParams.insert("n",    7);      // Anzahl der Sprossen


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

CAD_Cleanroom_CeilingVerticalLadder::~CAD_Cleanroom_CeilingVerticalLadder()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Cleanroom_CeilingVerticalLadder::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Cleanroom_CeilingVerticalLadder::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");     
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Cleanroom_CeilingVerticalLadder::iconPath()
{
    return ":/icons/cad_cleanroom/cad_cleanroom_ceilingverticalladder.svg";
}

QString CAD_Cleanroom_CeilingVerticalLadder::domain()
{
    return "Cleanroom";
}

QString CAD_Cleanroom_CeilingVerticalLadder::description()
{
    return "Cleanroom|Ceiling Vertical Ladder";
}

void CAD_Cleanroom_CeilingVerticalLadder::calculate()
{
    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);

    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();
    this->subItems.clear();
    this->subItems.append(left);
    this->subItems.append(right);
    this->subItems.append(left_down);
    this->subItems.append(right_down);
    this->subItems.append(left_horizontal);
    this->subItems.append(right_horizontal);

    this->snap_basepoint = (position);

    QVector3D position_lft = position + matrix_rotation * QVector3D(b/2 - g2/2, a/2, l/2);
    left->wizardParams.insert("Position x", position_lft.x());
    left->wizardParams.insert("Position y", position_lft.y());
    left->wizardParams.insert("Position z", position_lft.z());
    left->wizardParams.insert("Angle x", angle_x);
    left->wizardParams.insert("Angle y", angle_y);
    left->wizardParams.insert("Angle z", angle_z);
    left->wizardParams.insert("l", g2);
    left->wizardParams.insert("b", a);
    left->wizardParams.insert("a", l);
    left->layer = this->layer;
    left->processWizardInput();
    left->calculate();

    QVector3D position_rgt = position + matrix_rotation * QVector3D(-b/2 + g2/2, a/2, l/2);
    right->wizardParams.insert("Position x", position_rgt.x());
    right->wizardParams.insert("Position y", position_rgt.y());
    right->wizardParams.insert("Position z", position_rgt.z());
    right->wizardParams.insert("Angle x", angle_x);
    right->wizardParams.insert("Angle y", angle_y);
    right->wizardParams.insert("Angle z", angle_z);
    right->wizardParams.insert("l", g2);
    right->wizardParams.insert("b", a);
    right->wizardParams.insert("a", l);
    right->layer = this->layer;
    right->processWizardInput();
    right->calculate();

    QVector3D position_lfthori = position + matrix_rotation * QVector3D(b/2 - g2/2, a/2 + a2/2, l - a/2);
    left_horizontal->wizardParams.insert("Position x", position_lfthori.x());
    left_horizontal->wizardParams.insert("Position y", position_lfthori.y());
    left_horizontal->wizardParams.insert("Position z", position_lfthori.z());
    left_horizontal->wizardParams.insert("Angle x", angle_x);
    left_horizontal->wizardParams.insert("Angle y", angle_y);
    left_horizontal->wizardParams.insert("Angle z", angle_z);
    left_horizontal->wizardParams.insert("l", g2);
    left_horizontal->wizardParams.insert("b", a2 - a);
    left_horizontal->wizardParams.insert("a", a);
    left_horizontal->layer = this->layer;
    left_horizontal->processWizardInput();
    left_horizontal->calculate();

    QVector3D position_rgthori = position + matrix_rotation * QVector3D(-b/2 + g2/2, a/2 + a2/2, l - a/2);
    right_horizontal->wizardParams.insert("Position x", position_rgthori.x());
    right_horizontal->wizardParams.insert("Position y", position_rgthori.y());
    right_horizontal->wizardParams.insert("Position z", position_rgthori.z());
    right_horizontal->wizardParams.insert("Angle x", angle_x);
    right_horizontal->wizardParams.insert("Angle y", angle_y);
    right_horizontal->wizardParams.insert("Angle z", angle_z);
    right_horizontal->wizardParams.insert("l", g2);
    right_horizontal->wizardParams.insert("b", a2 - a);
    right_horizontal->wizardParams.insert("a", a);
    right_horizontal->layer = this->layer;
    right_horizontal->processWizardInput();
    right_horizontal->calculate();

    QVector3D position_lftdown = position + matrix_rotation * QVector3D(b/2 - g2/2, a/2 + a2, l - l4/2);
    left_down->wizardParams.insert("Position x", position_lftdown.x());
    left_down->wizardParams.insert("Position y", position_lftdown.y());
    left_down->wizardParams.insert("Position z", position_lftdown.z());
    left_down->wizardParams.insert("Angle x", angle_x);
    left_down->wizardParams.insert("Angle y", angle_y);
    left_down->wizardParams.insert("Angle z", angle_z);
    left_down->wizardParams.insert("l", g2);
    left_down->wizardParams.insert("b", a);
    left_down->wizardParams.insert("a", l4);
    left_down->layer = this->layer;
    left_down->processWizardInput();
    left_down->calculate();

    QVector3D position_rgtdown = position + matrix_rotation * QVector3D(-b/2 + g2/2, a/2 + a2, l - l4/2);
    right_down->wizardParams.insert("Position x", position_rgtdown.x());
    right_down->wizardParams.insert("Position y", position_rgtdown.y());
    right_down->wizardParams.insert("Position z", position_rgtdown.z());
    right_down->wizardParams.insert("Angle x", angle_x);
    right_down->wizardParams.insert("Angle y", angle_y);
    right_down->wizardParams.insert("Angle z", angle_z);
    right_down->wizardParams.insert("l", g2);
    right_down->wizardParams.insert("b", a);
    right_down->wizardParams.insert("a", l4);
    right_down->layer = this->layer;
    right_down->processWizardInput();
    right_down->calculate();



    for(int i = 0; i < n; i++)
    {
        CAD_basic_box *step = new CAD_basic_box;
        QVector3D position_step = position + matrix_rotation * QVector3D(0.0, a/2, (i+1)*l2);
        step->wizardParams.insert("Position x", position_step.x());
        step->wizardParams.insert("Position y", position_step.y());
        step->wizardParams.insert("Position z", position_step.z());
        step->wizardParams.insert("Angle x", angle_x);
        step->wizardParams.insert("Angle y", angle_y);
        step->wizardParams.insert("Angle z", angle_z);

        step->wizardParams.insert("l", b - 2 * g2);
        step->wizardParams.insert("b", a);
        step->wizardParams.insert("a", l3);
        step->layer = this->layer;
        step->processWizardInput();
        step->calculate();
        this->subItems.append(step);
    }

    this->boundingBox.enterVertices(left->boundingBox.getVertices());
    this->boundingBox.enterVertices(right->boundingBox.getVertices());
}

void CAD_Cleanroom_CeilingVerticalLadder::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();


    a = wizardParams.value("a").toDouble();
    a2 = wizardParams.value("a2").toDouble();
    b = wizardParams.value("b").toDouble();
    g2 = wizardParams.value("g2").toDouble();
    l = wizardParams.value("l").toDouble();
    l2 = wizardParams.value("l2").toDouble();
    l3 = wizardParams.value("l3").toDouble();
    l4 = wizardParams.value("l4").toDouble();
    n = wizardParams.value("n").toInt();
}

//void CAD_cleanroom_CeilingVerticalLadder::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Cleanroom_CeilingVerticalLadder::rotationOfFlange(quint8 num)
{
    Q_UNUSED(num);
    return matrix_rotation;
}
