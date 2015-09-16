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

#include "cad_heatcool_butterflyvalveclamped.h"

CAD_HeatCool_ButterflyValveClamped::CAD_HeatCool_ButterflyValveClamped() : CADitem(CADitemTypes::HeatCool_ButterflyValveClamped)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("d", 150.0);
    wizardParams.insert("d3", 50.0);
    wizardParams.insert("l", 200.0);
    wizardParams.insert("h", 120.0);
    wizardParams.insert("t", 200.0);
    wizardParams.insert("s",  10.0);

    pipe = new CAD_basic_pipe();
    ring_upper = new CAD_Basic_Flume();
    ring_lower = new CAD_Basic_Flume();
    upper = new CAD_basic_pipe();
    this->subItems.append(pipe);
    this->subItems.append(ring_upper);
    this->subItems.append(ring_lower);
    this->subItems.append(upper);

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

CAD_HeatCool_ButterflyValveClamped::~CAD_HeatCool_ButterflyValveClamped()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_HeatCool_ButterflyValveClamped::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    if(flangeIndex == 3)
    {
        QList<CADitemTypes::ItemType> flangable_items;
        flangable_items.append(CADitemTypes::HeatCool_ValveHandwheel);
        flangable_items.append(CADitemTypes::HeatCool_ValveHandwheelGear);
        flangable_items.append(CADitemTypes::HeatCool_ValveLever);
        flangable_items.append(CADitemTypes::HeatCool_ValveMotorRect);
        flangable_items.append(CADitemTypes::HeatCool_ValveMotorRound);
        return flangable_items;
    }
    else
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
        flangable_items.append(CADitemTypes::HeatCool_WaterHeater);
        return flangable_items;
    }
}

QImage CAD_HeatCool_ButterflyValveClamped::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_HeatCool_ButterflyValveClamped::iconPath()
{
    return ":/icons/cad_heatcool/cad_heatcool_butterflyvalveclamped.svg";
}

QString CAD_HeatCool_ButterflyValveClamped::domain()
{
    return "HeatCool";
}

QString CAD_HeatCool_ButterflyValveClamped::description()
{
    return "HeatCool|Butterfly Valve Clamped";
}

void CAD_HeatCool_ButterflyValveClamped::calculate()
{                
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    pipe->wizardParams.insert("Position x", position.x());
    pipe->wizardParams.insert("Position y", position.y());
    pipe->wizardParams.insert("Position z", position.z());
    pipe->wizardParams.insert("Angle x", angle_x);
    pipe->wizardParams.insert("Angle y", angle_y);
    pipe->wizardParams.insert("Angle z", angle_z);
    pipe->wizardParams.insert("l", l);
    pipe->wizardParams.insert("d", d);
    pipe->wizardParams.insert("s",  s);
    pipe->layer = this->layer;
    pipe->processWizardInput();
    pipe->calculate();

    QVector3D position_ring = position + matrix_rotation * QVector3D(l/4, 0.0, 0.0);
    ring_lower->wizardParams.insert("Position x", position_ring.x());
    ring_lower->wizardParams.insert("Position y", position_ring.y());
    ring_lower->wizardParams.insert("Position z", position_ring.z());
    ring_lower->wizardParams.insert("Angle x", angle_x);
    ring_lower->wizardParams.insert("Angle y", angle_y);
    ring_lower->wizardParams.insert("Angle z", angle_z);
    ring_lower->wizardParams.insert("l", l/2);
    ring_lower->wizardParams.insert("r", t/2);
    ring_lower->wizardParams.insert("s", (t-d)/2);
    ring_lower->wizardParams.insert("alpha", 300.0);
    ring_lower->layer = this->layer;
    ring_lower->processWizardInput();
    ring_lower->calculate();

    ring_upper->wizardParams.insert("Position x", position_ring.x());
    ring_upper->wizardParams.insert("Position y", position_ring.y());
    ring_upper->wizardParams.insert("Position z", position_ring.z());
    ring_upper->wizardParams.insert("Angle x", angle_x);
    ring_upper->wizardParams.insert("Angle y", angle_y);
    ring_upper->wizardParams.insert("Angle z", angle_z);
    ring_upper->wizardParams.insert("l", l/2);
    ring_upper->wizardParams.insert("r", t/2);
    ring_upper->wizardParams.insert("s", (t-d)/2);
    ring_upper->wizardParams.insert("alpha", 300.0);
    ring_upper->layer = this->layer;
    ring_upper->processWizardInput();
    ring_upper->rotateAroundAxis(180.0, QVector3D(1.0, 0.0, 0.0), angle_x, angle_y, angle_z);
    ring_upper->calculate();

    QVector3D position_upper = position + matrix_rotation * QVector3D(l/2, 0.0, h);
    upper->wizardParams.insert("Position x", position_upper.x());
    upper->wizardParams.insert("Position y", position_upper.y());
    upper->wizardParams.insert("Position z", position_upper.z());
    upper->wizardParams.insert("Angle x", angle_x);
    upper->wizardParams.insert("Angle y", angle_y);
    upper->wizardParams.insert("Angle z", angle_z);
    upper->wizardParams.insert("l", h - t/2);
    upper->wizardParams.insert("d", d3);
    upper->wizardParams.insert("s",  d3/2);
    upper->layer = this->layer;
    upper->processWizardInput();
    upper->rotateAroundAxis(90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    upper->calculate();

    this->boundingBox = pipe->boundingBox;
    this->boundingBox.enterVertices(ring_upper->boundingBox.getVertices());
    this->boundingBox.enterVertices(ring_lower->boundingBox.getVertices());
    this->boundingBox.enterVertices(upper->boundingBox.getVertices());
    this->snap_flanges = pipe->snap_flanges;
    this->snap_flanges.append(position_upper);
}

void CAD_HeatCool_ButterflyValveClamped::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    d = wizardParams.value("d").toDouble();
    d3 = wizardParams.value("d3").toDouble();
    h = wizardParams.value("h").toDouble();
    t = wizardParams.value("t").toDouble();
    l = wizardParams.value("l").toDouble();
    s = wizardParams.value("s").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

//void CAD_HeatCool_ButterflyValveClamped::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_HeatCool_ButterflyValveClamped::rotationOfFlange(quint8 num)
{
    if(num == 1)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(180.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else if(num == 2)
    {
        return matrix_rotation;
    }
    else
        return matrix_rotation;
}
