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

#include "cad_heatcool_valvehandwheelgear.h"
#include "glwidget.h"

CAD_HeatCool_ValveHandwheelGear::CAD_HeatCool_ValveHandwheelGear() : CADitem(CADitemTypes::HeatCool_ValveHandwheelGear)
{
    gear = new CAD_basic_box;
    wheel = new CAD_HeatCool_ValveHandwheel;
    this->subItems.append(gear);
    this->subItems.append(wheel);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("d", 120.0);
    wizardParams.insert("h",  60.0);
    wizardParams.insert("l",  40.0);
    wizardParams.insert("a",  70.0);
    wizardParams.insert("b1", 40.0);
    wizardParams.insert("b2", 20.0);

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

CAD_HeatCool_ValveHandwheelGear::~CAD_HeatCool_ValveHandwheelGear()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_HeatCool_ValveHandwheelGear::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_HeatCool_ValveHandwheelGear::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_HeatCool_ValveHandwheelGear::iconPath()
{
    return ":/icons/cad_heatcool/cad_heatcool_valvehandwheelgear.svg";
}

QString CAD_HeatCool_ValveHandwheelGear::domain()
{
    return "HeatCool";
}

QString CAD_HeatCool_ValveHandwheelGear::description()
{
    return "HeatCool|Valve Handwheel Gear";
}

void CAD_HeatCool_ValveHandwheelGear::calculate()
{                
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    QVector3D position_gear = position + matrix_rotation * QVector3D(0.0, 0.0, a/2);
    gear->wizardParams.insert("Position x", position_gear.x());
    gear->wizardParams.insert("Position y", position_gear.y());
    gear->wizardParams.insert("Position z", position_gear.z());
    gear->wizardParams.insert("Angle x", angle_x);
    gear->wizardParams.insert("Angle y", angle_y);
    gear->wizardParams.insert("Angle z", angle_z);
    gear->wizardParams.insert("l", l);
    gear->wizardParams.insert("b", b1);
    gear->wizardParams.insert("a", a);
    gear->layer = this->layer;
    gear->processWizardInput();
    gear->calculate();

    QVector3D position_wheel = position + matrix_rotation * QVector3D(0.0, -b1/2, h);
    wheel->wizardParams.insert("Position x", position_wheel.x());
    wheel->wizardParams.insert("Position y", position_wheel.y());
    wheel->wizardParams.insert("Position z", position_wheel.z());
    wheel->wizardParams.insert("Angle x", angle_x);
    wheel->wizardParams.insert("Angle y", angle_y);
    wheel->wizardParams.insert("Angle z", angle_z);
    wheel->wizardParams.insert("g", d);
    wheel->wizardParams.insert("e", d * 0.1);
    wheel->wizardParams.insert("f", b2);
    wheel->layer = this->layer;
    wheel->processWizardInput();
    wheel->rotateAroundAxis(90.0, QVector3D(1.0, 0.0, 0.0), angle_x, angle_y, angle_z);
    wheel->calculate();

    this->boundingBox = gear->boundingBox;
    this->boundingBox.enterVertices(wheel->boundingBox.getVertices());
    this->snap_flanges.append(position);
}

void CAD_HeatCool_ValveHandwheelGear::processWizardInput()
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
    a = wizardParams.value("a").toDouble();
    b1 = wizardParams.value("b1").toDouble();
    b2 = wizardParams.value("b2").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

//void CAD_HeatCool_ValveHandwheelGear::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_HeatCool_ValveHandwheelGear::rotationOfFlange(quint8 num)
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
