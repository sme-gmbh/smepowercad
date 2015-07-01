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

#include "cad_sprinkler_valve.h"
#include "glwidget.h"

CAD_sprinkler_valve::CAD_sprinkler_valve() : CADitem(CADitemTypes::Sprinkler_Valve)
{
    pipe = new CAD_basic_pipe;
    flange_left = new CAD_basic_pipe;
    flange_right = new CAD_basic_pipe;
    valve_1 = new CAD_basic_pipe;
    valve_2 = new CAD_basic_pipe;
    this->subItems.append(pipe);
    this->subItems.append(flange_left);
    this->subItems.append(flange_right);
    this->subItems.append(valve_1);
    this->subItems.append(valve_2);

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
}

CAD_sprinkler_valve::~CAD_sprinkler_valve()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_sprinkler_valve::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    if(flangeIndex == 3)
    {
        QList<CADitemTypes::ItemType> flangable_items;
        //handles
        return flangable_items;
    }
    else
    {
        QList<CADitemTypes::ItemType> flangable_items;
        //pipes
        return flangable_items;
    }
}

QImage CAD_sprinkler_valve::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_sprinkler_valve::iconPath()
{
    return ":/icons/cad_sprinkler/cad_sprinkler_valve.svg";
}

QString CAD_sprinkler_valve::domain()
{
    return "Sprinkler";
}

QString CAD_sprinkler_valve::description()
{
    return "Sprinkler|Valve";
}

void CAD_sprinkler_valve::calculate()
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

void CAD_sprinkler_valve::processWizardInput()
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

//void CAD_sprinkler_valve::paint(GLWidget *glwidget)
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
//        glwidget->glLineWidth(1.0);

//        indexBufLines->bind();
//        glwidget->glDrawElements(GL_LINES, indexBufLines->size(), GL_UNSIGNED_SHORT, 0);
//        indexBufLines->release();
//     }

//     arrayBufVertices->release();
//}

QMatrix4x4 CAD_sprinkler_valve::rotationOfFlange(quint8 num)
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
