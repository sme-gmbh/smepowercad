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

#include "cad_heatcool_valve.h"
#include "glwidget.h"

CAD_HeatCool_Valve::CAD_HeatCool_Valve() : CADitem(CADitemTypes::HeatCool_Valve)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("alpha", 90.0);
    wizardParams.insert("d",  150.0);
    wizardParams.insert("d3", 100.0);
    wizardParams.insert("d4", 150.0);
    wizardParams.insert("fe",  10.0);
    wizardParams.insert("ff",  10.0);
    wizardParams.insert("l",  500.0);
    wizardParams.insert("l2", 300.0);
    wizardParams.insert("l3", 200.0);
    wizardParams.insert("s",   10.0);

    pipe = new CAD_basic_pipe();
    flange_left = new CAD_basic_pipe();
    flange_right = new CAD_basic_pipe();
    valve_1 = new CAD_basic_pipe();
    valve_2 = new CAD_basic_pipe();
    this->subItems.append(pipe);
    this->subItems.append(flange_left);
    this->subItems.append(flange_right);
    this->subItems.append(valve_1);
    this->subItems.append(valve_2);

    processWizardInput();
    calculate();}

CAD_HeatCool_Valve::~CAD_HeatCool_Valve()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_HeatCool_Valve::flangable_items(int flangeIndex)
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

QImage CAD_HeatCool_Valve::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");  
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_HeatCool_Valve::iconPath()
{
    return ":/icons/cad_heatcool/cad_heatcool_valve.svg";
}

QString CAD_HeatCool_Valve::domain()
{
    return "HeatCool";
}

QString CAD_HeatCool_Valve::description()
{
    return "HeatCool|Valve";
}

void CAD_HeatCool_Valve::calculate()
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

    QVector3D position_flange = position + matrix_rotation * QVector3D(l - fe, 0.0, 0.0);
    flange_right->wizardParams.insert("Position x", position_flange.x());
    flange_right->wizardParams.insert("Position y", position_flange.y());
    flange_right->wizardParams.insert("Position z", position_flange.z());
    flange_right->wizardParams.insert("Angle x", angle_x);
    flange_right->wizardParams.insert("Angle y", angle_y);
    flange_right->wizardParams.insert("Angle z", angle_z);
    flange_right->wizardParams.insert("l", fe);
    flange_right->wizardParams.insert("d", d + 2 * ff);
    flange_right->wizardParams.insert("s",  ff);
    flange_right->layer = this->layer;
    flange_right->processWizardInput();
    flange_right->calculate();

    QVector3D position_v1 = position + matrix_rotation * QVector3D(l/2, 0.0, 0.0);
    valve_1->wizardParams.insert("Position x", position_v1.x());
    valve_1->wizardParams.insert("Position y", position_v1.y());
    valve_1->wizardParams.insert("Position z", position_v1.z());
    valve_1->wizardParams.insert("Angle x", angle_x);
    valve_1->wizardParams.insert("Angle y", angle_y);
    valve_1->wizardParams.insert("Angle z", angle_z);
    valve_1->wizardParams.insert("l", l2);
    valve_1->wizardParams.insert("d", d4);
    valve_1->wizardParams.insert("s",  d4/2);
    valve_1->layer = this->layer;
    valve_1->processWizardInput();
    valve_1->rotateAroundAxis(-alpha, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    valve_1->calculate();

    QVector3D position_v2 = position + matrix_rotation * QVector3D(l/2 + cos(alpha / 180.0 * PI) * l2, 0.0, sin(alpha / 180.0 * PI) * l2);
    valve_2->wizardParams.insert("Position x", position_v2.x());
    valve_2->wizardParams.insert("Position y", position_v2.y());
    valve_2->wizardParams.insert("Position z", position_v2.z());
    valve_2->wizardParams.insert("Angle x", angle_x);
    valve_2->wizardParams.insert("Angle y", angle_y);
    valve_2->wizardParams.insert("Angle z", angle_z);
    valve_2->wizardParams.insert("l", l3);
    valve_2->wizardParams.insert("d", d3);
    valve_2->wizardParams.insert("s",  d3/2);
    valve_2->layer = this->layer;
    valve_2->processWizardInput();
    valve_2->rotateAroundAxis(-alpha, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    valve_2->calculate();

    this->boundingBox = flange_left->boundingBox;
    this->boundingBox.enterVertices(flange_right->boundingBox.getVertices());
    this->boundingBox.enterVertices(valve_1->boundingBox.getVertices());
    this->boundingBox.enterVertices(valve_2->boundingBox.getVertices());

    this->snap_flanges = pipe->snap_flanges;
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l / 2 + cos(alpha / 180.0 * PI) * (l2 + l3), 0.0, sin(alpha / 180.0 * PI) * (l2 + l3)));
}

void CAD_HeatCool_Valve::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();


    alpha = wizardParams.value("alpha").toDouble();
    d = wizardParams.value("d").toDouble();
    d3 = wizardParams.value("d3").toDouble();
    d4 = wizardParams.value("d4").toDouble();
    fe = wizardParams.value("fe").toDouble();
    ff = wizardParams.value("ff").toDouble();
    l = wizardParams.value("l").toDouble();
    l2  =wizardParams.value("l2").toDouble();
    l3  =wizardParams.value("l3").toDouble();
    s  =wizardParams.value("s").toDouble();
}

//void CAD_HeatCool_Valve::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_HeatCool_Valve::rotationOfFlange(quint8 num)
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
