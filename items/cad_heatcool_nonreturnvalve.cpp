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

#include "cad_heatcool_nonreturnvalve.h"
#include "glwidget.h"

CAD_HeatCool_NonReturnValve::CAD_HeatCool_NonReturnValve() : CADitem(CADitemTypes::HeatCool_NonReturnValve)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("d", 150.0);
    wizardParams.insert("h", 150.0);
    wizardParams.insert("l", 200.0);
    wizardParams.insert("s", 10.0);
    wizardParams.insert("fe", 10.0);
    wizardParams.insert("ff", 10.0);

    flange_left = new CAD_basic_pipe();
    flange_right = new CAD_basic_pipe();
    pipe = new CAD_basic_pipe();
    valve = new CAD_basic_pipe();
    this->subItems.append(flange_left);
    this->subItems.append(flange_right);
    this->subItems.append(pipe);
    this->subItems.append(valve);

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

CAD_HeatCool_NonReturnValve::~CAD_HeatCool_NonReturnValve()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_HeatCool_NonReturnValve::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
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

QImage CAD_HeatCool_NonReturnValve::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_HeatCool_NonReturnValve::iconPath()
{
    return ":/icons/cad_heatcool/cad_heatcool_nonreturnvalve.svg";
}

QString CAD_HeatCool_NonReturnValve::domain()
{
    return "HeatCool";
}

QString CAD_HeatCool_NonReturnValve::description()
{
    return "HeatCool|Non Return Valve";
}

void CAD_HeatCool_NonReturnValve::calculate()
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
    flange_left->wizardParams.insert("d", d + 2 * ff);
    flange_left->wizardParams.insert("s",  ff);
    flange_left->layer = this->layer;
    flange_left->processWizardInput();
    flange_left->calculate();

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

    QVector3D position_right = position + matrix_rotation * QVector3D(l -fe, 0.0, 0.0);
    flange_right->wizardParams.insert("Position x", position_right.x());
    flange_right->wizardParams.insert("Position y", position_right.y());
    flange_right->wizardParams.insert("Position z", position_right.z());
    flange_right->wizardParams.insert("Angle x", angle_x);
    flange_right->wizardParams.insert("Angle y", angle_y);
    flange_right->wizardParams.insert("Angle z", angle_z);
    flange_right->wizardParams.insert("l", fe);
    flange_right->wizardParams.insert("d", d + 2 * ff);
    flange_right->wizardParams.insert("s",  ff);
    flange_right->layer = this->layer;
    flange_right->processWizardInput();
    flange_right->calculate();

    QVector3D position_valve = position + matrix_rotation * QVector3D(l/2, 0.0, h);
    valve->wizardParams.insert("Position x", position_valve.x());
    valve->wizardParams.insert("Position y", position_valve.y());
    valve->wizardParams.insert("Position z", position_valve.z());
    valve->wizardParams.insert("Angle x", angle_x);
    valve->wizardParams.insert("Angle y", angle_y);
    valve->wizardParams.insert("Angle z", angle_z);
    valve->wizardParams.insert("l", h);
    valve->wizardParams.insert("d", d);
    valve->wizardParams.insert("s",  d/2);
    valve->layer = this->layer;
    valve->processWizardInput();
    valve->rotateAroundAxis(90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    valve->calculate();

    this->boundingBox = flange_left->boundingBox;
    this->boundingBox.enterVertices(flange_right->boundingBox.getVertices());
    this->boundingBox.enterVertices(valve->boundingBox.getVertices());
    this->boundingBox.enterVertices(pipe->boundingBox.getVertices());

    this->snap_flanges = pipe->snap_flanges;
}

void CAD_HeatCool_NonReturnValve::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    d = wizardParams.value("d").toDouble();
    h = wizardParams.value("h").toDouble();
    l = wizardParams.value("l").toDouble();
    s = wizardParams.value("s").toDouble();
    fe = wizardParams.value("fe").toDouble();
    ff = wizardParams.value("ff").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

//void CAD_HeatCool_NonReturnValve::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_HeatCool_NonReturnValve::rotationOfFlange(quint8 num)
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
