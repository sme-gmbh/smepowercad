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

#include "cad_heatcool_heatexchangersoldered.h"
#include "glwidget.h"

CAD_HeatCool_HeatexchangerSoldered::CAD_HeatCool_HeatexchangerSoldered() : CADitem(CADitemTypes::HeatCool_HeatexchangerSoldered)
{
    body = new CAD_basic_box;
    foot_1 = new CAD_basic_box;
    foot_2 = new CAD_basic_box;
    pipe_1 = new CAD_basic_pipe;
    pipe_2 = new CAD_basic_pipe;
    pipe_3 = new CAD_basic_pipe;
    pipe_4 = new CAD_basic_pipe;
    this->subItems.append(body);
    this->subItems.append(foot_1);
    this->subItems.append(foot_2);
    this->subItems.append(pipe_1);
    this->subItems.append(pipe_2);
    this->subItems.append(pipe_3);
    this->subItems.append(pipe_4);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 700.0);
    wizardParams.insert("a2",200.0);
    wizardParams.insert("b", 400.0);
    wizardParams.insert("d", 150.0);
    wizardParams.insert("e", 200.0);
    wizardParams.insert("f", 400.0);
    wizardParams.insert("l", 200.0);
    wizardParams.insert("l2", 50.0);
    wizardParams.insert("s",  10.0);

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

CAD_HeatCool_HeatexchangerSoldered::~CAD_HeatCool_HeatexchangerSoldered()
{
//    arrayBufVertices.destroy();
//    indexBufFaces.destroy();
//    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_HeatCool_HeatexchangerSoldered::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::HeatCool_Adjustvalve);
    flangable_items.append(CADitemTypes::HeatCool_BallValve);
    flangable_items.append(CADitemTypes::HeatCool_Boiler);
    flangable_items.append(CADitemTypes::HeatCool_ButterflyValveBolted);
    flangable_items.append(CADitemTypes::HeatCool_ButterflyValveClamped);
    flangable_items.append(CADitemTypes::HeatCool_Chiller);
    flangable_items.append(CADitemTypes::HeatCool_Controlvalve);
    flangable_items.append(CADitemTypes::HeatCool_CoolingTower);
    flangable_items.append(CADitemTypes::HeatCool_DirtArrester);
    flangable_items.append(CADitemTypes::HeatCool_ExpansionChamber);
    flangable_items.append(CADitemTypes::HeatCool_Filter);
    flangable_items.append(CADitemTypes::HeatCool_Flange);
    flangable_items.append(CADitemTypes::HeatCool_Flowmeter);
    flangable_items.append(CADitemTypes::HeatCool_Gauge);
    flangable_items.append(CADitemTypes::HeatCool_Gauge90Degree);
    flangable_items.append(CADitemTypes::HeatCool_HeatexchangerSoldered);
    flangable_items.append(CADitemTypes::HeatCool_HeatexchangerBolted);
    flangable_items.append(CADitemTypes::HeatCool_NonReturnFlap);
    flangable_items.append(CADitemTypes::HeatCool_NonReturnValve);
    flangable_items.append(CADitemTypes::HeatCool_Pipe);
    flangable_items.append(CADitemTypes::HeatCool_PipeEndCap);
    flangable_items.append(CADitemTypes::HeatCool_PipeReducer);
    flangable_items.append(CADitemTypes::HeatCool_PipeTeeConnector);
    flangable_items.append(CADitemTypes::HeatCool_PipeTurn);
    flangable_items.append(CADitemTypes::HeatCool_PumpInline);
    flangable_items.append(CADitemTypes::HeatCool_PumpNorm);
    flangable_items.append(CADitemTypes::HeatCool_Radiator);
    flangable_items.append(CADitemTypes::HeatCool_RadiatorCompact);
    flangable_items.append(CADitemTypes::HeatCool_RadiatorFlange);
    flangable_items.append(CADitemTypes::HeatCool_RadiatorFlangeBent);
    flangable_items.append(CADitemTypes::HeatCool_RadiatorValve);
    flangable_items.append(CADitemTypes::HeatCool_SafetyValve);
    flangable_items.append(CADitemTypes::HeatCool_Sensor);
    flangable_items.append(CADitemTypes::HeatCool_StorageBoiler);
    flangable_items.append(CADitemTypes::HeatCool_Valve);
    flangable_items.append(CADitemTypes::HeatCool_Valve90Degree);
    flangable_items.append(CADitemTypes::HeatCool_ValveHandwheel);
    flangable_items.append(CADitemTypes::HeatCool_ValveHandwheelGear);
    flangable_items.append(CADitemTypes::HeatCool_ValveLever);
    flangable_items.append(CADitemTypes::HeatCool_ValveMotorRect);
    flangable_items.append(CADitemTypes::HeatCool_ValveMotorRound);
    flangable_items.append(CADitemTypes::HeatCool_WaterHeater);
    return flangable_items;
}

QImage CAD_HeatCool_HeatexchangerSoldered::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_HeatCool_HeatexchangerSoldered::iconPath()
{
    return ":/icons/cad_heatcool/cad_heatcool_heatexchangersoldered.svg";
}

QString CAD_HeatCool_HeatexchangerSoldered::domain()
{
    return "HeatCool";
}

QString CAD_HeatCool_HeatexchangerSoldered::description()
{
    return "HeatCool|Heatexchanger Soldered";
}

void CAD_HeatCool_HeatexchangerSoldered::calculate()
{                
    boundingBox.reset();
                    
    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();
                                
    this->snap_basepoint = (position);

    QVector3D position_body = position + matrix_rotation * QVector3D(0.0, 0.0, a2 + a/2);
    body->wizardParams.insert("Position x", position_body.x());
    body->wizardParams.insert("Position y", position_body.y());
    body->wizardParams.insert("Position z", position_body.z());
    body->wizardParams.insert("Angle x", angle_x);
    body->wizardParams.insert("Angle y", angle_y);
    body->wizardParams.insert("Angle z", angle_z);
    body->wizardParams.insert("l", l);
    body->wizardParams.insert("b", b);
    body->wizardParams.insert("a", a);
    body->layer = this->layer;
    body->processWizardInput();
    body->calculate();

    QVector3D position_foot_1 = position + matrix_rotation * QVector3D(0.0, 2 * b /5, a2/2);
    foot_1->wizardParams.insert("Position x", position_foot_1.x());
    foot_1->wizardParams.insert("Position y", position_foot_1.y());
    foot_1->wizardParams.insert("Position z", position_foot_1.z());
    foot_1->wizardParams.insert("Angle x", angle_x);
    foot_1->wizardParams.insert("Angle y", angle_y);
    foot_1->wizardParams.insert("Angle z", angle_z);
    foot_1->wizardParams.insert("l", l * 0.8);
    foot_1->wizardParams.insert("b", b * 0.05);
    foot_1->wizardParams.insert("a", a2);
    foot_1->layer = this->layer;
    foot_1->processWizardInput();
    foot_1->calculate();

    QVector3D position_foot_2 = position + matrix_rotation * QVector3D(0.0, -2 * b /5, a2/2);
    foot_2->wizardParams.insert("Position x", position_foot_2.x());
    foot_2->wizardParams.insert("Position y", position_foot_2.y());
    foot_2->wizardParams.insert("Position z", position_foot_2.z());
    foot_2->wizardParams.insert("Angle x", angle_x);
    foot_2->wizardParams.insert("Angle y", angle_y);
    foot_2->wizardParams.insert("Angle z", angle_z);
    foot_2->wizardParams.insert("l", l * 0.8);
    foot_2->wizardParams.insert("b", b * 0.05);
    foot_2->wizardParams.insert("a", a2);
    foot_2->layer = this->layer;
    foot_2->processWizardInput();
    foot_2->calculate();

    QVector3D position_pipe_1 = position + matrix_rotation * QVector3D(-l/2 - l2, -e/2, a2 + a/2 -f/2);
    pipe_1->wizardParams.insert("Position x", position_pipe_1.x());
    pipe_1->wizardParams.insert("Position y", position_pipe_1.y());
    pipe_1->wizardParams.insert("Position z", position_pipe_1.z());
    pipe_1->wizardParams.insert("Angle x", angle_x);
    pipe_1->wizardParams.insert("Angle y", angle_y);
    pipe_1->wizardParams.insert("Angle z", angle_z);
    pipe_1->wizardParams.insert("l", l2);
    pipe_1->wizardParams.insert("d", d);
    pipe_1->wizardParams.insert("s",  s);
    pipe_1->layer = this->layer;
    pipe_1->processWizardInput();
    pipe_1->calculate();

    QVector3D position_pipe_2 = position + matrix_rotation * QVector3D(-l/2 - l2, e/2, a2 + a/2 -f/2);
    pipe_2->wizardParams.insert("Position x", position_pipe_2.x());
    pipe_2->wizardParams.insert("Position y", position_pipe_2.y());
    pipe_2->wizardParams.insert("Position z", position_pipe_2.z());
    pipe_2->wizardParams.insert("Angle x", angle_x);
    pipe_2->wizardParams.insert("Angle y", angle_y);
    pipe_2->wizardParams.insert("Angle z", angle_z);
    pipe_2->wizardParams.insert("l", l2);
    pipe_2->wizardParams.insert("d", d);
    pipe_2->wizardParams.insert("s",  s);
    pipe_2->layer = this->layer;
    pipe_2->processWizardInput();
    pipe_2->calculate();

    QVector3D position_pipe_3 = position + matrix_rotation * QVector3D(-l/2 - l2, e/2, a2 + a/2 + f/2);
    pipe_3->wizardParams.insert("Position x", position_pipe_3.x());
    pipe_3->wizardParams.insert("Position y", position_pipe_3.y());
    pipe_3->wizardParams.insert("Position z", position_pipe_3.z());
    pipe_3->wizardParams.insert("Angle x", angle_x);
    pipe_3->wizardParams.insert("Angle y", angle_y);
    pipe_3->wizardParams.insert("Angle z", angle_z);
    pipe_3->wizardParams.insert("l", l2);
    pipe_3->wizardParams.insert("d", d);
    pipe_3->wizardParams.insert("s",  s);
    pipe_3->layer = this->layer;
    pipe_3->processWizardInput();
    pipe_3->calculate();

    QVector3D position_pipe_4 = position + matrix_rotation * QVector3D(-l/2 - l2, -e/2, a2 + a/2 + f/2);
    pipe_4->wizardParams.insert("Position x", position_pipe_4.x());
    pipe_4->wizardParams.insert("Position y", position_pipe_4.y());
    pipe_4->wizardParams.insert("Position z", position_pipe_4.z());
    pipe_4->wizardParams.insert("Angle x", angle_x);
    pipe_4->wizardParams.insert("Angle y", angle_y);
    pipe_4->wizardParams.insert("Angle z", angle_z);
    pipe_4->wizardParams.insert("l", l2);
    pipe_4->wizardParams.insert("d", d);
    pipe_4->wizardParams.insert("s",  s);
    pipe_4->layer = this->layer;
    pipe_4->processWizardInput();
    pipe_4->calculate();

    this->boundingBox = body->boundingBox;
    this->boundingBox.enterVertices(foot_1->boundingBox.getVertices());
    this->boundingBox.enterVertices(foot_2->boundingBox.getVertices());
    this->boundingBox.enterVertices(pipe_1->boundingBox.getVertices());
    this->boundingBox.enterVertices(pipe_2->boundingBox.getVertices());
    this->boundingBox.enterVertices(pipe_3->boundingBox.getVertices());
    this->boundingBox.enterVertices(pipe_4->boundingBox.getVertices());

    this->snap_flanges.append(position_pipe_1);
    this->snap_flanges.append(position_pipe_2);
    this->snap_flanges.append(position_pipe_3);
    this->snap_flanges.append(position_pipe_4);
}

void CAD_HeatCool_HeatexchangerSoldered::processWizardInput()
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
    d = wizardParams.value("d").toDouble();
    e = wizardParams.value("e").toDouble();
    f = wizardParams.value("f").toDouble();
    l = wizardParams.value("l").toDouble();
    l2 = wizardParams.value("l2").toDouble();
    s = wizardParams.value("s").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

//void CAD_HeatCool_HeatexchangerSoldered::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_HeatCool_HeatexchangerSoldered::rotationOfFlange(quint8 num)
{
    if(num >= 1 && num <= 4)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(180.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else
        return matrix_rotation;
}
