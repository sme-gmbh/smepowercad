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

#include "cad_electrical_cabletrayverticalladder.h"
#include "glwidget.h"

CAD_Electrical_CabletrayVerticalLadder::CAD_Electrical_CabletrayVerticalLadder() : CADitem(CADitemTypes::Electrical_CabletrayVerticalLadder)
{
    left = new CAD_basic_box;
    right = new CAD_basic_box;
    this->subItems.append(left);
    this->subItems.append(right);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 100.0);    // Breite (in y-Richtung)
    wizardParams.insert("a2", 10.0);    // Abstand der Sprossen vom Rand
    wizardParams.insert("a3", 10.0);    // Breite der Sprossen (in y-Richtung)
    wizardParams.insert("b", 100.0);    // Tiefe (in x-Richtung)
    wizardParams.insert("b2", 10.0);    // Tife der Randteile
    wizardParams.insert("l", 1000.0);   // Höhe (in z-Richtung)
    wizardParams.insert("l2", 150.0);   // Abstand der Sprossen (in z-Richtung)
    wizardParams.insert("l3", 10.0);    // Höhe der Sprossen (in z-Richtung
    wizardParams.insert("n", 5);        // Anzahl der Sprossen

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

CAD_Electrical_CabletrayVerticalLadder::~CAD_Electrical_CabletrayVerticalLadder()
{
//    arrayBufVertices.destroy();
//    indexBufFaces.destroy();
//    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_Electrical_CabletrayVerticalLadder::flangable_items()
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

QImage CAD_Electrical_CabletrayVerticalLadder::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Electrical_CabletrayVerticalLadder::iconPath()
{
    return ":/icons/cad_electrical/cad_electrical_cabletrayverticalladder.svg";
}

QString CAD_Electrical_CabletrayVerticalLadder::domain()
{
    return "Electrical";
}

QString CAD_Electrical_CabletrayVerticalLadder::description()
{
    return "Electrical|Cabletray Vertical Ladder";
}

void CAD_Electrical_CabletrayVerticalLadder::calculate()
{
    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
                
    boundingBox.reset();
                    
    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();
    this->subItems.clear();
    this->subItems.append(left);
    this->subItems.append(right);
                                
    this->snap_basepoint = (position);

    QVector3D position_lft = position + matrix_rotation * QVector3D(b/2 - b2/2, a/2, l/2);
    left->wizardParams.insert("Position x", position_lft.x());
    left->wizardParams.insert("Position y", position_lft.y());
    left->wizardParams.insert("Position z", position_lft.z());
    left->wizardParams.insert("Angle x", angle_x);
    left->wizardParams.insert("Angle y", angle_y);
    left->wizardParams.insert("Angle z", angle_z);

    left->wizardParams.insert("Size x", b2);
    left->wizardParams.insert("Size y", a);
    left->wizardParams.insert("Size z", l);
    left->layer = this->layer;
    left->processWizardInput();
    left->calculate();

    QVector3D position_rgt = position + matrix_rotation * QVector3D(-b/2 + b2/2, a/2, l/2);
    right->wizardParams.insert("Position x", position_rgt.x());
    right->wizardParams.insert("Position y", position_rgt.y());
    right->wizardParams.insert("Position z", position_rgt.z());
    right->wizardParams.insert("Angle x", angle_x);
    right->wizardParams.insert("Angle y", angle_y);
    right->wizardParams.insert("Angle z", angle_z);

    right->wizardParams.insert("Size x", b2);
    right->wizardParams.insert("Size y", a);
    right->wizardParams.insert("Size z", l);
    right->layer = this->layer;
    right->processWizardInput();
    right->calculate();

    for(int i = 0; i < n; i++)
    {
        CAD_basic_box *step = new CAD_basic_box;
        qreal l4 = (l - (n-1) * l2) / 2; //Abstand der Sprossen zum Rand
        QVector3D position_step = position + matrix_rotation * QVector3D(0.0, a2 + a3/2, l4 + i*l2);
        step->wizardParams.insert("Position x", position_step.x());
        step->wizardParams.insert("Position y", position_step.y());
        step->wizardParams.insert("Position z", position_step.z());
        step->wizardParams.insert("Angle x", angle_x);
        step->wizardParams.insert("Angle y", angle_y);
        step->wizardParams.insert("Angle z", angle_z);

        step->wizardParams.insert("Size x", b - 2 * b2);
        step->wizardParams.insert("Size y", a3);
        step->wizardParams.insert("Size z", l3);
        step->layer = this->layer;
        step->processWizardInput();
        step->calculate();
        this->subItems.append(step);
    }

    this->boundingBox.enterVertices(left->boundingBox.getVertices());
    this->boundingBox.enterVertices(right->boundingBox.getVertices());

    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(0.0, 0.0, l));

}

void CAD_Electrical_CabletrayVerticalLadder::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();


    a = wizardParams.value("a").toDouble();
    a2 = wizardParams.value("a2").toDouble();
    a3 = wizardParams.value("a3").toDouble();
    b = wizardParams.value("b").toDouble();
    b2 = wizardParams.value("b2").toDouble();
    l = wizardParams.value("l").toDouble();
    l2 = wizardParams.value("l2").toDouble();
    l3 = wizardParams.value("l3").toDouble();
    n = wizardParams.value("n").toInt();
}

//void CAD_Electrical_CabletrayVerticalLadder::paint(GLWidget *glwidget)
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