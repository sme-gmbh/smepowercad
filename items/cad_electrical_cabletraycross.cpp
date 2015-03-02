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

#include "cad_electrical_cabletraycross.h"
#include "glwidget.h"

CAD_Electrical_CabletrayCross::CAD_Electrical_CabletrayCross() : CADitem(CADitemTypes::Electrical_CabletrayCross)
{
    cross = new CAD_basic_box;
    ct_1 = new CAD_electrical_cabletray;
    ct_2 = new CAD_electrical_cabletray;
    ct_3 = new CAD_electrical_cabletray;
    ct_4 = new CAD_electrical_cabletray;
    this->subItems.append(cross);
    this->subItems.append(ct_1);
    this->subItems.append(ct_2);
    this->subItems.append(ct_3);
    this->subItems.append(ct_4);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 100.0);        // Höhe
    wizardParams.insert("b", 300.0);        // Breite Kreuzung
    wizardParams.insert("b1", 100.0);       // Endstück in y-Richtung
    wizardParams.insert("l", 300.0);        // Länge Kreuzung
    wizardParams.insert("l1", 100.0);       // Endstück in x-Richtung
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

CAD_Electrical_CabletrayCross::~CAD_Electrical_CabletrayCross()
{
//    arrayBufVertices.destroy();
//    indexBufFaces.destroy();
//    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_Electrical_CabletrayCross::flangable_items()
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

QImage CAD_Electrical_CabletrayCross::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Electrical_CabletrayCross::iconPath()
{
    return ":/icons/cad_electrical/cad_electrical_cabletraycross.svg";
}

QString CAD_Electrical_CabletrayCross::domain()
{
    return "Electrical";
}

QString CAD_Electrical_CabletrayCross::description()
{
    return "Electrical|Cabletray Cross";
}

void CAD_Electrical_CabletrayCross::calculate()
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

    QVector3D position_cross = position + matrix_rotation * QVector3D(l1 + l/2, 0.0, s/2);
    cross->wizardParams.insert("Position x", position_cross.x());
    cross->wizardParams.insert("Position y", position_cross.y());
    cross->wizardParams.insert("Position z", position_cross.z());
    cross->wizardParams.insert("Angle x", angle_x);
    cross->wizardParams.insert("Angle y", angle_y);
    cross->wizardParams.insert("Angle z", angle_z);
    cross->wizardParams.insert("Size x", l);
    cross->wizardParams.insert("Size y", b);
    cross->wizardParams.insert("Size z", s);
    cross->layer = this->layer;
    cross->processWizardInput();
    cross->calculate();

    QVector3D position_ct_1 = position;
    ct_1->wizardParams.insert("Position x", position_ct_1.x());
    ct_1->wizardParams.insert("Position y", position_ct_1.y());
    ct_1->wizardParams.insert("Position z", position_ct_1.z());
    ct_1->wizardParams.insert("Angle x", angle_x);
    ct_1->wizardParams.insert("Angle y", angle_y);
    ct_1->wizardParams.insert("Angle z", angle_z);
    ct_1->wizardParams.insert("a", a);
    ct_1->wizardParams.insert("b", b);
    ct_1->wizardParams.insert("l", l1);
    ct_1->wizardParams.insert("s", s);
    ct_1->layer = this->layer;
    ct_1->processWizardInput();
    ct_1->calculate();

    QVector3D position_ct_2 = position + matrix_rotation * QVector3D(l1 + l/2, b/2 + b1, 0.0);
    ct_2->wizardParams.insert("Position x", position_ct_2.x());
    ct_2->wizardParams.insert("Position y", position_ct_2.y());
    ct_2->wizardParams.insert("Position z", position_ct_2.z());
    ct_2->wizardParams.insert("Angle x", angle_x);
    ct_2->wizardParams.insert("Angle y", angle_y);
    ct_2->wizardParams.insert("Angle z", angle_z-90);
    ct_2->wizardParams.insert("a", a);
    ct_2->wizardParams.insert("b", l);
    ct_2->wizardParams.insert("l", b1);
    ct_2->wizardParams.insert("s", s);
    ct_2->layer = this->layer;
    ct_2->processWizardInput();
    ct_2->calculate();

    QVector3D position_ct_3 = position + matrix_rotation * QVector3D(2*l1 + l, 0.0, 0.0);
    ct_3->wizardParams.insert("Position x", position_ct_3.x());
    ct_3->wizardParams.insert("Position y", position_ct_3.y());
    ct_3->wizardParams.insert("Position z", position_ct_3.z());
    ct_3->wizardParams.insert("Angle x", angle_x);
    ct_3->wizardParams.insert("Angle y", angle_y);
    ct_3->wizardParams.insert("Angle z", angle_z+180);
    ct_3->wizardParams.insert("a", a);
    ct_3->wizardParams.insert("b", b);
    ct_3->wizardParams.insert("l", l1);
    ct_3->wizardParams.insert("s", s);
    ct_3->layer = this->layer;
    ct_3->processWizardInput();
    ct_3->calculate();

    QVector3D position_ct_4 = position + matrix_rotation * QVector3D(l1 + l/2, -b/2 - b1, 0.0);
    ct_4->wizardParams.insert("Position x", position_ct_4.x());
    ct_4->wizardParams.insert("Position y", position_ct_4.y());
    ct_4->wizardParams.insert("Position z", position_ct_4.z());
    ct_4->wizardParams.insert("Angle x", angle_x);
    ct_4->wizardParams.insert("Angle y", angle_y);
    ct_4->wizardParams.insert("Angle z", angle_z+90);
    ct_4->wizardParams.insert("a", a);
    ct_4->wizardParams.insert("b", l);
    ct_4->wizardParams.insert("l", b1);
    ct_4->wizardParams.insert("s", s);
    ct_4->layer = this->layer;
    ct_4->processWizardInput();
    ct_4->calculate();

    this->boundingBox.enterVertices(ct_1->boundingBox.getVertices());
    this->boundingBox.enterVertices(ct_2->boundingBox.getVertices());
    this->boundingBox.enterVertices(ct_3->boundingBox.getVertices());
    this->boundingBox.enterVertices(ct_4->boundingBox.getVertices());

    this->snap_flanges.append(position);
    this->snap_flanges.append(position_ct_2);
    this->snap_flanges.append(position_ct_3);
    this->snap_flanges.append(position_ct_4);
}

void CAD_Electrical_CabletrayCross::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
    a = wizardParams.value("a").toDouble();
    b = wizardParams.value("b").toDouble();
    b1 = wizardParams.value("b1").toDouble();
    l = wizardParams.value("l").toDouble();
    l1 = wizardParams.value("l1").toDouble();
    s = wizardParams.value("s").toDouble();
}

//void CAD_Electrical_CabletrayCross::paint(GLWidget *glwidget)
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
