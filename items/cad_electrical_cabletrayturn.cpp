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

#include "cad_electrical_cabletrayturn.h"
#include "glwidget.h"

CAD_Electrical_CabletrayTurn::CAD_Electrical_CabletrayTurn() : CADitem(CADitemTypes::Electrical_CabletrayTurn)
{
    floor = new CAD_basic_box;
    side_1 = new CAD_basic_box;
    side_2 = new CAD_basic_box;
    this->subItems.append(floor);
    this->subItems.append(side_1);
    this->subItems.append(side_2);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 100.0);        // Höhe
    wizardParams.insert("b", 300.0);        // Breite
    wizardParams.insert("l", 300.0);        // Länge
    wizardParams.insert("s", 10.0);         // Wandstärke

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

CAD_Electrical_CabletrayTurn::~CAD_Electrical_CabletrayTurn()
{
//    arrayBufVertices.destroy();
//    indexBufFaces.destroy();
//    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_Electrical_CabletrayTurn::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Electrical_Cabletray);
    flangable_items.append(CADitemTypes::Electrical_CabletrayCross);
    flangable_items.append(CADitemTypes::Electrical_CabletrayReducer);
    flangable_items.append(CADitemTypes::Electrical_CabletrayTeeConnector);
    flangable_items.append(CADitemTypes::Electrical_CabletrayTransition);
    flangable_items.append(CADitemTypes::Electrical_CabletrayTurn);
    flangable_items.append(CADitemTypes::Electrical_CabletrayVerticalLadder);
    return flangable_items;
}

QImage CAD_Electrical_CabletrayTurn::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Electrical_CabletrayTurn::iconPath()
{
    return ":/icons/cad_electrical/cad_electrical_cabletrayturn.svg";
}

QString CAD_Electrical_CabletrayTurn::domain()
{
    return "Electrical";
}

QString CAD_Electrical_CabletrayTurn::description()
{
    return "Electrical|Cabletray Turn";
}

void CAD_Electrical_CabletrayTurn::calculate()
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

    QVector3D position_flr = position + matrix_rotation * QVector3D(l/2, 0.0, s/2);
    floor->wizardParams.insert("Position x", position_flr.x());
    floor->wizardParams.insert("Position y", position_flr.y());
    floor->wizardParams.insert("Position z", position_flr.z());
    floor->wizardParams.insert("Angle x", angle_x);
    floor->wizardParams.insert("Angle y", angle_y);
    floor->wizardParams.insert("Angle z", angle_z);

    floor->wizardParams.insert("l", l);
    floor->wizardParams.insert("b", b);
    floor->wizardParams.insert("a", s);
    floor->layer = this->layer;
    floor->processWizardInput();
    floor->calculate();

    QVector3D position_side_1 = position + matrix_rotation * QVector3D(l/2, (s-b)/2, (a+s)/2);
    side_1->wizardParams.insert("Position x", position_side_1.x());
    side_1->wizardParams.insert("Position y", position_side_1.y());
    side_1->wizardParams.insert("Position z", position_side_1.z());
    side_1->wizardParams.insert("Angle x", angle_x);
    side_1->wizardParams.insert("Angle y", angle_y);
    side_1->wizardParams.insert("Angle z", angle_z);

    side_1->wizardParams.insert("l", l);
    side_1->wizardParams.insert("b", s);
    side_1->wizardParams.insert("a", a-s);
    side_1->layer = this->layer;
    side_1->processWizardInput();
    side_1->calculate();

    QVector3D position_side_2 = position + matrix_rotation * QVector3D(l - s/2, s/2, (a+s)/2);
    side_2->wizardParams.insert("Position x", position_side_2.x());
    side_2->wizardParams.insert("Position y", position_side_2.y());
    side_2->wizardParams.insert("Position z", position_side_2.z());
    side_2->wizardParams.insert("Angle x", angle_x);
    side_2->wizardParams.insert("Angle y", angle_y);
    side_2->wizardParams.insert("Angle z", angle_z);

    side_2->wizardParams.insert("l", s);
    side_2->wizardParams.insert("b", b-s);
    side_2->wizardParams.insert("a", a-s);
    side_2->layer = this->layer;
    side_2->processWizardInput();
    side_2->calculate();

    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l/2, b/2, 0.0));

    this->snap_center.append(floor->snap_center);
    this->snap_center.append(side_1->snap_center);
    this->snap_center.append(side_2->snap_center);

    this->snap_vertices.append(floor->snap_vertices);
    this->snap_vertices.append(side_1->snap_vertices);
    this->snap_vertices.append(side_2->snap_vertices);

    this->boundingBox.enterVertices(floor->boundingBox.getVertices());
    this->boundingBox.enterVertices(side_1->boundingBox.getVertices());
    this->boundingBox.enterVertices(side_2->boundingBox.getVertices());
}

void CAD_Electrical_CabletrayTurn::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a = wizardParams.value("a").toDouble();
    b = wizardParams.value("b").toDouble();
    l = wizardParams.value("l").toDouble();
    s = wizardParams.value("s").toDouble();
}

//void CAD_Electrical_CabletrayTurn::paint(GLWidget *glwidget)
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
