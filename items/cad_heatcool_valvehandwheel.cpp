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

#include "cad_heatcool_valvehandwheel.h"
#include "glwidget.h"

CAD_HeatCool_ValveHandwheel::CAD_HeatCool_ValveHandwheel() : CADitem(CADitemTypes::HeatCool_ValveHandwheel)
{
    wheel = new CAD_Basic_Torus;
    cross_1 = new CAD_basic_pipe;
    cross_2 = new CAD_basic_pipe;
    bar = new CAD_basic_pipe;
    this->subItems.append(wheel);
    this->subItems.append(cross_1);
    this->subItems.append(cross_2);
    this->subItems.append(bar);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("g", 50.0);
    wizardParams.insert("e", 10.0);
    wizardParams.insert("f", 50.0);

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

CAD_HeatCool_ValveHandwheel::~CAD_HeatCool_ValveHandwheel()
{
//    arrayBufVertices.destroy();
//    indexBufFaces.destroy();
//    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_HeatCool_ValveHandwheel::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_HeatCool_ValveHandwheel::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_HeatCool_ValveHandwheel::iconPath()
{
    return ":/icons/cad_heatcool/cad_heatcool_valvehandwheel.svg";
}

QString CAD_HeatCool_ValveHandwheel::domain()
{
    return "HeatCool";
}

QString CAD_HeatCool_ValveHandwheel::description()
{
    return "HeatCool|Valve Handwheel";
}

void CAD_HeatCool_ValveHandwheel::calculate()
{
                
    boundingBox.reset();
                    
    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();
                                
    this->snap_basepoint = (position);

    QVector3D position_wheel = position + matrix_rotation * QVector3D(0.0, 0.0, f);
    wheel->wizardParams.insert("Position x", position_wheel.x());
    wheel->wizardParams.insert("Position y", position_wheel.y());
    wheel->wizardParams.insert("Position z", position_wheel.z());
    wheel->wizardParams.insert("Angle x", angle_x);
    wheel->wizardParams.insert("Angle y", angle_y);
    wheel->wizardParams.insert("Angle z", angle_z);
    wheel->wizardParams.insert("r1", g/2 - e/2);
    wheel->wizardParams.insert("r2",  e/2);
    wheel->layer = this->layer;
    wheel->processWizardInput();
    wheel->matrix_rotation = this->matrix_rotation;
    wheel->calculate();

    QVector3D position_c1 = position + matrix_rotation * QVector3D(0.0, -g/2 + e, f);
    cross_1->wizardParams.insert("Position x", position_c1.x());
    cross_1->wizardParams.insert("Position y", position_c1.y());
    cross_1->wizardParams.insert("Position z", position_c1.z());
    cross_1->wizardParams.insert("Angle x", angle_x);
    cross_1->wizardParams.insert("Angle y", angle_y);
    cross_1->wizardParams.insert("Angle z", angle_z);
    cross_1->wizardParams.insert("l", g - 2 * e);
    cross_1->wizardParams.insert("d", 0.2 * e);
    cross_1->wizardParams.insert("s",  0.1 * e);
    cross_1->layer = this->layer;
    cross_1->processWizardInput();
    cross_1->matrix_rotation = this->matrix_rotation;
    cross_1->matrix_rotation.rotate(90, QVector3D(0.0, 0.0, 1.0));
    cross_1->calculate();

    QVector3D position_c2 = position + matrix_rotation * QVector3D(-g/2 + e, 0.0, f);
    cross_2->wizardParams.insert("Position x", position_c2.x());
    cross_2->wizardParams.insert("Position y", position_c2.y());
    cross_2->wizardParams.insert("Position z", position_c2.z());
    cross_2->wizardParams.insert("Angle x", angle_x);
    cross_2->wizardParams.insert("Angle y", angle_y);
    cross_2->wizardParams.insert("Angle z", angle_z);
    cross_2->wizardParams.insert("l", g - 2 * e);
    cross_2->wizardParams.insert("d", 0.2 * e);
    cross_2->wizardParams.insert("s",  0.1 * e);
    cross_2->layer = this->layer;
    cross_2->processWizardInput();
    cross_2->matrix_rotation = this->matrix_rotation;
    cross_2->calculate();

    bar->wizardParams.insert("Position x", position.x());
    bar->wizardParams.insert("Position y", position.y());
    bar->wizardParams.insert("Position z", position.z());
    bar->wizardParams.insert("Angle x", angle_x);
    bar->wizardParams.insert("Angle y", angle_y);
    bar->wizardParams.insert("Angle z", angle_z);
    bar->wizardParams.insert("l", f);
    bar->wizardParams.insert("d", 0.2 * e);
    bar->wizardParams.insert("s",  0.1 * e);
    bar->layer = this->layer;
    bar->processWizardInput();
    bar->matrix_rotation = this->matrix_rotation;
    bar->matrix_rotation.rotate(-90.0, QVector3D(0.0, 1.0, 0.0));
    bar->calculate();

    this->snap_flanges.append(position);
    this->boundingBox = wheel->boundingBox;
    this->boundingBox.enterVertices(bar->boundingBox.getVertices());
}

void CAD_HeatCool_ValveHandwheel::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    e = wizardParams.value("e").toDouble();
    f = wizardParams.value("f").toDouble();
    g = wizardParams.value("g").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

//void CAD_HeatCool_ValveHandwheel::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_HeatCool_ValveHandwheel::rotationOfFlange(quint8 num)
{
    if(num == 1)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(-90.0, 0.0, 1.0, 0.0);
        return matrix_rotation * m;
    }
    else
        return matrix_rotation;
}
