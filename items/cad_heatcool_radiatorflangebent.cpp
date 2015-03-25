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

#include "cad_heatcool_radiatorflangebent.h"
#include "glwidget.h"

CAD_HeatCool_RadiatorFlangeBent::CAD_HeatCool_RadiatorFlangeBent() : CADitem(CADitemTypes::HeatCool_RadiatorFlangeBent)
{
    connector = new CAD_basic_box;
    pipe_l1 = new CAD_basic_pipe;
    pipe_l2 = new CAD_basic_pipe;
    pipe_o1 = new CAD_basic_pipe;
    pipe_o2 = new CAD_basic_pipe;
    this->subItems.append(connector);
    this->subItems.append(pipe_l1);
    this->subItems.append(pipe_l2);
    this->subItems.append(pipe_o1);
    this->subItems.append(pipe_o2);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("d", 20.0);
    wizardParams.insert("l", 60.0);
    wizardParams.insert("e", 50.0);
    wizardParams.insert("h", 40.0);
    wizardParams.insert("s",  5.0);

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

CAD_HeatCool_RadiatorFlangeBent::~CAD_HeatCool_RadiatorFlangeBent()
{
//    arrayBufVertices.destroy();
//    indexBufFaces.destroy();
//    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_HeatCool_RadiatorFlangeBent::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::HeatCool_Pipe);
    flangable_items.append(CADitemTypes::HeatCool_PipeEndCap);
    flangable_items.append(CADitemTypes::HeatCool_PipeReducer);
    flangable_items.append(CADitemTypes::HeatCool_PipeTeeConnector);
    flangable_items.append(CADitemTypes::HeatCool_PipeTurn);
    flangable_items.append(CADitemTypes::HeatCool_RadiatorValve);
    
    return flangable_items;
}

QImage CAD_HeatCool_RadiatorFlangeBent::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_HeatCool_RadiatorFlangeBent::iconPath()
{
    return ":/icons/cad_heatcool/cad_heatcool_radiatorflangebent.svg";
}

QString CAD_HeatCool_RadiatorFlangeBent::domain()
{
    return "HeatCool";
}

QString CAD_HeatCool_RadiatorFlangeBent::description()
{
    return "HeatCool|Radiator Flange Bent";
}

void CAD_HeatCool_RadiatorFlangeBent::calculate()
{                
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    QVector3D position_pl1 = position + matrix_rotation * QVector3D(0.0, e/2, 0.0);
    pipe_l1->wizardParams.insert("Position x", position_pl1.x());
    pipe_l1->wizardParams.insert("Position y", position_pl1.y());
    pipe_l1->wizardParams.insert("Position z", position_pl1.z());
    pipe_l1->wizardParams.insert("Angle x", angle_x);
    pipe_l1->wizardParams.insert("Angle y", angle_y);
    pipe_l1->wizardParams.insert("Angle z", angle_z);
    pipe_l1->wizardParams.insert("l", l);
    pipe_l1->wizardParams.insert("d", d);
    pipe_l1->wizardParams.insert("s", s);
    pipe_l1->layer = this->layer;
    pipe_l1->processWizardInput();
    pipe_l1->calculate();

    QVector3D position_pl2 = position + matrix_rotation * QVector3D(0.0, -e/2, 0.0);
    pipe_l2->wizardParams.insert("Position x", position_pl2.x());
    pipe_l2->wizardParams.insert("Position y", position_pl2.y());
    pipe_l2->wizardParams.insert("Position z", position_pl2.z());
    pipe_l2->wizardParams.insert("Angle x", angle_x);
    pipe_l2->wizardParams.insert("Angle y", angle_y);
    pipe_l2->wizardParams.insert("Angle z", angle_z);
    pipe_l2->wizardParams.insert("l", l);
    pipe_l2->wizardParams.insert("d", d);
    pipe_l2->wizardParams.insert("s", s);
    pipe_l2->layer = this->layer;
    pipe_l2->processWizardInput();
    pipe_l2->calculate();

    QVector3D position_po1 = position + matrix_rotation * QVector3D(l, e/2, 0.0);
    pipe_o1->wizardParams.insert("Position x", position_po1.x());
    pipe_o1->wizardParams.insert("Position y", position_po1.y());
    pipe_o1->wizardParams.insert("Position z", position_po1.z());
    pipe_o1->wizardParams.insert("Angle x", angle_x);
    pipe_o1->wizardParams.insert("Angle y", angle_y);
    pipe_o1->wizardParams.insert("Angle z", angle_z);
    pipe_o1->wizardParams.insert("l", h);
    pipe_o1->wizardParams.insert("d", d);
    pipe_o1->wizardParams.insert("s", s);
    pipe_o1->layer = this->layer;
    pipe_o1->processWizardInput();
    pipe_o1->rotateAroundAxis(-90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    pipe_o1->calculate();

    QVector3D position_po2 = position + matrix_rotation * QVector3D(l, -e/2, 0.0);
    pipe_o2->wizardParams.insert("Position x", position_po2.x());
    pipe_o2->wizardParams.insert("Position y", position_po2.y());
    pipe_o2->wizardParams.insert("Position z", position_po2.z());
    pipe_o2->wizardParams.insert("Angle x", angle_x);
    pipe_o2->wizardParams.insert("Angle y", angle_y);
    pipe_o2->wizardParams.insert("Angle z", angle_z);
    pipe_o2->wizardParams.insert("l", h);
    pipe_o2->wizardParams.insert("d", d);
    pipe_o2->wizardParams.insert("s", s);
    pipe_o2->layer = this->layer;
    pipe_o2->processWizardInput();
    pipe_o2->rotateAroundAxis(-90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    pipe_o2->calculate();

    QVector3D positon_con = position + matrix_rotation * QVector3D(l, 0.0, 0.0);
    connector->wizardParams.insert("Position x", positon_con.x());
    connector->wizardParams.insert("Position y", positon_con.y());
    connector->wizardParams.insert("Position z", positon_con.z());
    connector->wizardParams.insert("Angle x", angle_x);
    connector->wizardParams.insert("Angle y", angle_y);
    connector->wizardParams.insert("Angle z", angle_z);
    connector->wizardParams.insert("l", d);
    connector->wizardParams.insert("b", e + d);
    connector->wizardParams.insert("a", d);
    connector->layer = this->layer;
    connector->processWizardInput();
    connector->calculate();

    this->boundingBox = pipe_l1->boundingBox;
    this->boundingBox.enterVertices(pipe_l2->boundingBox.getVertices());
    this->boundingBox.enterVertices(pipe_o1->boundingBox.getVertices());
    this->boundingBox.enterVertices(pipe_o2->boundingBox.getVertices());
    this->boundingBox.enterVertices(connector->boundingBox.getVertices());

    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, -e/2, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l,  e/2, 0.0));
}

void CAD_HeatCool_RadiatorFlangeBent::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();


    d = wizardParams.value("d").toDouble();
    l = wizardParams.value("l").toDouble();
    e = wizardParams.value("e").toDouble();
    h = wizardParams.value("h").toDouble();
    s = wizardParams.value("s").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

//void CAD_HeatCool_RadiatorFlangeBent::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_HeatCool_RadiatorFlangeBent::rotationOfFlange(quint8 num)
{
    if(num == 1)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(180.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else if(num == 2 || num == 3)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(90.0, 0.0, 1.0, 0.0);
        return matrix_rotation * m;
    }
    else
        return matrix_rotation;
}
