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

#include "cad_heatcool_nonreturnflap.h"
#include "glwidget.h"

CAD_HeatCool_NonReturnFlap::CAD_HeatCool_NonReturnFlap() : CADitem(CADitemTypes::HeatCool_NonReturnFlap)
{
    flange_left = new CAD_basic_pipe;
    flange_right = new CAD_basic_pipe;
    valve = new CAD_heatcool_pipeReducer;
    this->subItems.append(flange_left);
    this->subItems.append(flange_right);
    this->subItems.append(valve);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("d1", 150.0);
    wizardParams.insert("d2", 150.0);
    wizardParams.insert("l", 200.0);
    wizardParams.insert("s", 10.0);
    wizardParams.insert("fe", 10.0);
    wizardParams.insert("ff", 10.0);

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

CAD_HeatCool_NonReturnFlap::~CAD_HeatCool_NonReturnFlap()
{
//    arrayBufVertices.destroy();
//    indexBufFaces.destroy();
//    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_HeatCool_NonReturnFlap::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_HeatCool_NonReturnFlap::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_HeatCool_NonReturnFlap::iconPath()
{
    return ":/icons/cad_heatcool/cad_heatcool_nonreturnflap.svg";
}

QString CAD_HeatCool_NonReturnFlap::domain()
{
    return "HeatCool";
}

QString CAD_HeatCool_NonReturnFlap::description()
{
    return "HeatCool|Non Return Flap";
}

void CAD_HeatCool_NonReturnFlap::calculate()
{                
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    flange_left->wizardParams.insert("Position x", position.x());
    flange_left->wizardParams.insert("Position y", position.y());
    flange_left->wizardParams.insert("Position z", position.z());
    flange_left->wizardParams.insert("Angle x", angle_x);
    flange_left->wizardParams.insert("Angle y", angle_y);
    flange_left->wizardParams.insert("Angle z", angle_z);
    flange_left->wizardParams.insert("l", fe);
    flange_left->wizardParams.insert("d", d1 + 2 * ff);
    flange_left->wizardParams.insert("s",  ff);
    flange_left->layer = this->layer;
    flange_left->processWizardInput();
    flange_left->calculate();

    QVector3D position_right = position + matrix_rotation * QVector3D(l -fe, 0.0, 0.0);
    flange_right->wizardParams.insert("Position x", position_right.x());
    flange_right->wizardParams.insert("Position y", position_right.y());
    flange_right->wizardParams.insert("Position z", position_right.z());
    flange_right->wizardParams.insert("Angle x", angle_x);
    flange_right->wizardParams.insert("Angle y", angle_y);
    flange_right->wizardParams.insert("Angle z", angle_z);
    flange_right->wizardParams.insert("l", fe);
    flange_right->wizardParams.insert("d", d2 + 2 * ff);
    flange_right->wizardParams.insert("s",  ff);
    flange_right->layer = this->layer;
    flange_right->processWizardInput();
    flange_right->calculate();

    valve->wizardParams.insert("Position x", position.x());
    valve->wizardParams.insert("Position y", position.y());
    valve->wizardParams.insert("Position z", position.z());
    valve->wizardParams.insert("Angle x", angle_x);
    valve->wizardParams.insert("Angle y", angle_y);
    valve->wizardParams.insert("Angle z", angle_z);
    valve->wizardParams.insert("d1", d1);
    valve->wizardParams.insert("d2", d2);
    valve->wizardParams.insert("l", l);
    valve->wizardParams.insert("l1", fe);
    valve->wizardParams.insert("l2", fe);
    valve->wizardParams.insert("e", 0.0);
    valve->wizardParams.insert("iso1", 0.0);
    valve->wizardParams.insert("iso2", 0.0);
    valve->layer = this->layer;
    valve->processWizardInput();
    valve->calculate();

    this->boundingBox = flange_left->boundingBox;
    this->boundingBox.enterVertices(flange_right->boundingBox.getVertices());
    this->boundingBox.enterVertices(valve->boundingBox.getVertices());

    this->snap_flanges = valve->snap_flanges;
}

void CAD_HeatCool_NonReturnFlap::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    d1 = wizardParams.value("d1").toDouble();
    d2 = wizardParams.value("d2").toDouble();
    l = wizardParams.value("l").toDouble();
    s = wizardParams.value("s").toDouble();
    fe = wizardParams.value("fe").toDouble();
    ff = wizardParams.value("ff").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

//void CAD_HeatCool_NonReturnFlap::paint(GLWidget *glwidget)
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
