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

#include "cad_electrical_motorasynchronous.h"
#include "glwidget.h"

CAD_Electrical_MotorAsynchronous::CAD_Electrical_MotorAsynchronous() : CADitem(CADitemTypes::Electrical_MotorAsynchronous)
{

    leg_1 = new CAD_basic_box;
    leg_2 = new CAD_basic_box;
    motor = new CAD_basic_pipe;
    axis = new CAD_basic_pipe;
    this->subItems.append(leg_1);
    this->subItems.append(leg_2);
    this->subItems.append(motor);
    this->subItems.append(axis);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("d1", 50.0);
    wizardParams.insert("d2", 800.0);
    wizardParams.insert("l1", 100.0);
    wizardParams.insert("l2", 700.0);
    wizardParams.insert("e", 500.0);

//    arrayBufVertices = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
//    arrayBufVertices.create();
//    arrayBufVertices.setUsagePattern(QOpenGLBuffer::StaticDraw);

//    indexBufFaces = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
//    indexBufFaces.create();
//    indexBufFaces.setUsagePattern(QOpenGLBuffer::StaticDraw);

//    indexBufLines = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
//    indexBufLines.create();
//    indexBufLines.setUsagePattern(QOpenGLBuffer::StaticDraw);
   
    processWizardInput();
    calculate();
}

CAD_Electrical_MotorAsynchronous::~CAD_Electrical_MotorAsynchronous()
{
//    arrayBufVertices.destroy();
//    indexBufFaces.destroy();
//    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_Electrical_MotorAsynchronous::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::HeatCool_PumpNorm);
    return flangable_items;
}

QImage CAD_Electrical_MotorAsynchronous::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Electrical_MotorAsynchronous::iconPath()
{
    return ":/icons/cad_electrical/cad_electrical_motorasynchronous.svg";
}

QString CAD_Electrical_MotorAsynchronous::domain()
{
    return "Electrical";
}

QString CAD_Electrical_MotorAsynchronous::description()
{
    return "Electrical|Motor Asynchronous";
}

void CAD_Electrical_MotorAsynchronous::calculate()
{                
    boundingBox.reset();
                    
    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();
                                
    this->snap_basepoint = (position);

    QVector3D position_axis = position + matrix_rotation * QVector3D(-l2/2, 0.0, e);
    axis->wizardParams.insert("Position x", position_axis.x());
    axis->wizardParams.insert("Position y", position_axis.y());
    axis->wizardParams.insert("Position z", position_axis.z());
    axis->wizardParams.insert("Angle x", angle_x);
    axis->wizardParams.insert("Angle y", angle_y);
    axis->wizardParams.insert("Angle z", angle_z);
    axis->wizardParams.insert("l", l1 + l2);
    axis->wizardParams.insert("d", d1);
    axis->wizardParams.insert("s",  d1/2);
    axis->layer = this->layer;
    axis->processWizardInput();
    axis->calculate();

    motor->wizardParams.insert("Position x", position_axis.x());
    motor->wizardParams.insert("Position y", position_axis.y());
    motor->wizardParams.insert("Position z", position_axis.z());
    motor->wizardParams.insert("Angle x", angle_x);
    motor->wizardParams.insert("Angle y", angle_y);
    motor->wizardParams.insert("Angle z", angle_z);
    motor->wizardParams.insert("l", l2);
    motor->wizardParams.insert("d", d2);
    motor->wizardParams.insert("s",  d2/2 - d1/2);
    motor->layer = this->layer;
    motor->processWizardInput();
    motor->calculate();

    QVector3D position_l1 = position + matrix_rotation * QVector3D(l2 *0.3, 0.0, e/2);
    leg_1->wizardParams.insert("Position x", position_l1.x());
    leg_1->wizardParams.insert("Position y", position_l1.y());
    leg_1->wizardParams.insert("Position z", position_l1.z());
    leg_1->wizardParams.insert("Angle x", angle_x);
    leg_1->wizardParams.insert("Angle y", angle_y);
    leg_1->wizardParams.insert("Angle z", angle_z);
    leg_1->wizardParams.insert("l", 0.05 * d2);
    leg_1->wizardParams.insert("b", 0.6 * d2);
    leg_1->wizardParams.insert("a", e);
    leg_1->layer = this->layer;
    leg_1->processWizardInput();
    leg_1->calculate();

    QVector3D position_l2 = position + matrix_rotation * QVector3D(-l2 *0.3, 0.0, e/2);
    leg_2->wizardParams.insert("Position x", position_l2.x());
    leg_2->wizardParams.insert("Position y", position_l2.y());
    leg_2->wizardParams.insert("Position z", position_l2.z());
    leg_2->wizardParams.insert("Angle x", angle_x);
    leg_2->wizardParams.insert("Angle y", angle_y);
    leg_2->wizardParams.insert("Angle z", angle_z);
    leg_2->wizardParams.insert("l", 0.05 * d2);
    leg_2->wizardParams.insert("b", 0.6 * d2);
    leg_2->wizardParams.insert("a", e);
    leg_2->layer = this->layer;
    leg_2->processWizardInput();
    leg_2->calculate();



    foreach(CADitem *item, subItems)
    {
        this->boundingBox.enterVertices(item->boundingBox.getVertices());
    }
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l2/2 + l1, 0.0, e));
}

void CAD_Electrical_MotorAsynchronous::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();


    d1 = wizardParams.value("d1").toDouble();
    d2 = wizardParams.value("d2").toDouble();
    l1 = wizardParams.value("l1").toDouble();
    l2 = wizardParams.value("l2").toDouble();
    e = wizardParams.value("e").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

//void CAD_Electrical_MotorAsynchronous::paint(GLWidget *glwidget)
//{
//    QColor color_pen_tmp = getColorPen();
//    QColor color_brush_tmp = getColorBrush();

//    arrayBufVertices.bind();
//    glwidget->shaderProgram->enableAttributeArray(glwidget->shader_vertexLocation);
//    glwidget->shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

//    if (glwidget->render_solid)
//    {
//        glwidget->setPaintingColor(color_brush_tmp);

//        indexBufFaces.bind();
//        glwidget->glDrawElements(GL_TRIANGLE_STRIP, indexBufFaces.size(), GL_UNSIGNED_SHORT, 0);

//        indexBufFaces.release();
//    }

//    if (glwidget->render_outline)
//    {
//        glwidget->setPaintingColor(color_pen_tmp);
//        glwidget->glLineWidth(1.0);
                                      
//        indexBufLines.bind();
//        glwidget->glDrawElements(GL_LINES, indexBufLines.size(), GL_UNSIGNED_SHORT, 0);
//        indexBufLines.release();
//     }                          
                                                                                           
//     arrayBufVertices.release();
//}

QMatrix4x4 CAD_Electrical_MotorAsynchronous::rotationOfFlange(quint8 num)
{
    if(num == 1)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(180.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else
        return matrix_rotation;
}
