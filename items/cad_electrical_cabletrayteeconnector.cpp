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

#include "cad_electrical_cabletrayteeconnector.h"
#include "glwidget.h"

CAD_electrical_CabletrayTeeconnector::CAD_electrical_CabletrayTeeconnector() : CADitem(CADitemTypes::Electrical_CabletrayTransition)
{
    floor = new CAD_basic_box();
    side = new CAD_basic_box();
    this->subItems.append(floor);
    this->subItems.append(side);
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

CAD_electrical_CabletrayTeeconnector::~CAD_electrical_CabletrayTeeconnector()
{
//    arrayBufVertices.destroy();
//    indexBufFaces.destroy();
//    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_electrical_CabletrayTeeconnector::flangable_items()
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

QImage CAD_electrical_CabletrayTeeconnector::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_electrical_CabletrayTeeconnector::iconPath()
{
    return ":/icons/cad_electrical/cad_electrical_cabletrayteeconnector.svg";
}

QString CAD_electrical_CabletrayTeeconnector::domain()
{
    return "Electrical";
}

QString CAD_electrical_CabletrayTeeconnector::description()
{
    return "Electrical|Cabletray Teeconnector";
}

void CAD_electrical_CabletrayTeeconnector::calculate()
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
    floor->wizardParams.insert("Size x", l);
    floor->wizardParams.insert("Size y", b);
    floor->wizardParams.insert("Size z", s);
    floor->layer = this->layer;
    floor->processWizardInput();
    floor->calculate();

    QVector3D position_side = position + matrix_rotation * QVector3D(l/2, -b/2 + s/2, a/2 + s/2);
    side->wizardParams.insert("Position x", position_side.x());
    side->wizardParams.insert("Position y", position_side.y());
    side->wizardParams.insert("Position z", position_side.z());
    side->wizardParams.insert("Angle x", angle_x);
    side->wizardParams.insert("Angle y", angle_y);
    side->wizardParams.insert("Angle z", angle_z);
    side->wizardParams.insert("Size x", l);
    side->wizardParams.insert("Size y", s);
    side->wizardParams.insert("Size z", a - s);
    side->layer = this->layer;
    side->processWizardInput();
    side->calculate();

    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l/2, b/2, 0.0));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, 0.0));

    this->snap_center.append(floor->snap_center);
    this->snap_center.append(side->snap_center);

    this->snap_vertices.append(floor->snap_vertices);
    this->snap_vertices.append(side->snap_vertices);

    this->boundingBox.enterVertices(floor->boundingBox.getVertices());
    this->boundingBox.enterVertices(side->boundingBox.getVertices());

}

void CAD_electrical_CabletrayTeeconnector::processWizardInput()
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

//void CAD_electrical_CabletrayTeeconnector::paint(GLWidget *glwidget)
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
