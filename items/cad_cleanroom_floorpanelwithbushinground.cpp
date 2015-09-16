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

#include "cad_cleanroom_floorpanelwithbushinground.h"

CAD_Cleanroom_FloorPanelWithBushingRound::CAD_Cleanroom_FloorPanelWithBushingRound() : CADitem(CADitemTypes::Cleanroom_FloorPanelWithBushingRound)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("h",   20.0);
    wizardParams.insert("g", 600.0);
    wizardParams.insert("l", 600.0);
    wizardParams.insert("e", 100.0);
    wizardParams.insert("f", 100.0);
    wizardParams.insert("d", 200.0);

    panel = new CAD_basic_box();
    bush_up = new CAD_basic_circle();
    arc = new CAD_basic_arc();
    this->subItems.append(panel);
    this->subItems.append(bush_up);
    this->subItems.append(arc);

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

CAD_Cleanroom_FloorPanelWithBushingRound::~CAD_Cleanroom_FloorPanelWithBushingRound()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Cleanroom_FloorPanelWithBushingRound::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Cleanroom_FloorPanelWithBushingRound::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");   
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Cleanroom_FloorPanelWithBushingRound::iconPath()
{
    return ":/icons/cad_cleanroom/cad_cleanroom_floorpanelwithbushinground.svg";
}

QString CAD_Cleanroom_FloorPanelWithBushingRound::domain()
{
    return "Cleanroom";
}

QString CAD_Cleanroom_FloorPanelWithBushingRound::description()
{
    return "Cleanroom|Floor Panel With Bushing Round";
}

void CAD_Cleanroom_FloorPanelWithBushingRound::calculate()
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

    QVector3D position_panel = position + matrix_rotation * QVector3D(0.5 * l, 0.5 * g, 0.5 * h);
    panel->wizardParams.insert("Position x", position_panel.x());
    panel->wizardParams.insert("Position y", position_panel.y());
    panel->wizardParams.insert("Position z", position_panel.z());
    panel->wizardParams.insert("Angle x", angle_x);
    panel->wizardParams.insert("Angle y", angle_y);
    panel->wizardParams.insert("Angle z", angle_z);
    panel->wizardParams.insert("a",  h);
    panel->wizardParams.insert("b",  g);
    panel->wizardParams.insert("l",  l);
    panel->layer = this->layer;
    panel->processWizardInput();
    panel->calculate();

    QVector3D position_bush_up = position + matrix_rotation * QVector3D(e + 0.5 * d, f + 0.5 * d, h + 0.1);
    bush_up->wizardParams.insert("Center x", (position_bush_up.x()));
    bush_up->wizardParams.insert("Center y", (position_bush_up.y()));
    bush_up->wizardParams.insert("Center z", (position_bush_up.z()));
    bush_up->wizardParams.insert("Angle x", (angle_x));
    bush_up->wizardParams.insert("Angle y", (angle_y));
    bush_up->wizardParams.insert("Angle z", (angle_z));
    bush_up->wizardParams.insert("r", (0.5 * d));
    bush_up->processWizardInput();
    bush_up->calculate();
    bush_up->layer = this->layer;

    QVector3D position_arc = position + matrix_rotation * QVector3D(e + 0.6 * d, f + 0.5 * d, h + 0.1);
    arc->wizardParams.insert("Position x", position_arc.x());
    arc->wizardParams.insert("Position y", position_arc.y());
    arc->wizardParams.insert("Position z", position_arc.z());
    arc->wizardParams.insert("Angle x", angle_x);
    arc->wizardParams.insert("Angle y", angle_y);
    arc->wizardParams.insert("Angle z", angle_z);
    arc->wizardParams.insert("r", 0.5 * d);
    arc->wizardParams.insert("alpha", 163.23);// 174.268032=2*arccos(0.05)
    arc->layer = this->layer;
    arc->processWizardInput();
    arc->rotateAroundAxis(-188.42, QVector3D(0.0, 0.0, 1.0), angle_x, angle_y, angle_z); //2.865983983
    arc->calculate();


    this->snap_vertices = panel->snap_vertices;
    this->snap_vertices.append(position + matrix_rotation * QVector3D(e + 0.5 * d, f + 0.5 * d, h));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(e + 0.5 * d, f + 0.5 * d, 0.0));
    this->boundingBox = panel->boundingBox;
}

void CAD_Cleanroom_FloorPanelWithBushingRound::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    h = wizardParams.value("h").toDouble();
    g = wizardParams.value("g").toDouble();
    l = wizardParams.value("l").toDouble();
    e = wizardParams.value("e").toDouble();
    f = wizardParams.value("f").toDouble();
    d = wizardParams.value("d").toDouble();
}

//void CAD_Cleanroom_FloorPanelWithBushingRound::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Cleanroom_FloorPanelWithBushingRound::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}
