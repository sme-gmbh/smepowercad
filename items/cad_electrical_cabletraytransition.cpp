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

#include "cad_electrical_cabletraytransition.h"
#include "glwidget.h"

CAD_Electrical_CabletrayTransition::CAD_Electrical_CabletrayTransition() : CADitem(CADitemTypes::Electrical_CabletrayTransition)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 100.0);        // Höhe
    wizardParams.insert("b", 300.0);        // Breite
    wizardParams.insert("l", 500.0);        // Länge des geneigten Stücks (in x-Richtung)
    wizardParams.insert("l1", 100.0);       // Länge des ebenen Stücks
    wizardParams.insert("i", 100.0);        // Versatz in z-Richtung
    wizardParams.insert("s", 10.0);         // Wandstärke

    end_lower = new CAD_electrical_cabletray();
    end_upper = new CAD_electrical_cabletray();
    floor = new CAD_basic_box();
    left = new CAD_basic_box();
    right = new CAD_basic_box();
    this->subItems.append(end_lower);
    this->subItems.append(end_upper);
    this->subItems.append(floor);
    this->subItems.append(left);
    this->subItems.append(right);

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

CAD_Electrical_CabletrayTransition::~CAD_Electrical_CabletrayTransition()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Electrical_CabletrayTransition::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
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

QImage CAD_Electrical_CabletrayTransition::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");   
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Electrical_CabletrayTransition::iconPath()
{
    return ":/icons/cad_electrical/cad_electrical_cabletraytransition.svg";
}

QString CAD_Electrical_CabletrayTransition::domain()
{
    return "Electrical";
}

QString CAD_Electrical_CabletrayTransition::description()
{
    return "Electrical|Cabletray Transition";
}

void CAD_Electrical_CabletrayTransition::calculate()
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

    end_lower->wizardParams.insert("Position x", position.x());
    end_lower->wizardParams.insert("Position y", position.y());
    end_lower->wizardParams.insert("Position z", position.z());
    end_lower->wizardParams.insert("Angle x", angle_x);
    end_lower->wizardParams.insert("Angle y", angle_y);
    end_lower->wizardParams.insert("Angle z", angle_z);
    end_lower->wizardParams.insert("a", a);
    end_lower->wizardParams.insert("b", b);
    end_lower->wizardParams.insert("l", l1);
    end_lower->wizardParams.insert("s", s);
    end_lower->layer = this->layer;
    end_lower->processWizardInput();
    end_lower->calculate();

    QVector3D position_up = position + matrix_rotation * QVector3D(l1 + l, 0.0, i);
    end_upper->wizardParams.insert("Position x", position_up.x());
    end_upper->wizardParams.insert("Position y", position_up.y());
    end_upper->wizardParams.insert("Position z", position_up.z());
    end_upper->wizardParams.insert("Angle x", angle_x);
    end_upper->wizardParams.insert("Angle y", angle_y);
    end_upper->wizardParams.insert("Angle z", angle_z);
    end_upper->wizardParams.insert("a", a);
    end_upper->wizardParams.insert("b", b);
    end_upper->wizardParams.insert("l", l1);
    end_upper->wizardParams.insert("s", s);
    end_upper->layer = this->layer;
    end_upper->processWizardInput();
    end_upper->calculate();

    QVector3D position_flr = position + matrix_rotation * QVector3D(l1 + l/2, 0.0, i/2 + s/2);
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

    QVector3D vertices_flr[] = {
        position + matrix_rotation * QVector3D(l1, -b/2, 0.0),
        position + matrix_rotation * QVector3D(l + l1, -b/2, i),
        position + matrix_rotation * QVector3D(l +l1, b/2, i),
        position + matrix_rotation * QVector3D(l1, b/2, 0.0),
        position + matrix_rotation * QVector3D(l1, -b/2, s),
        position + matrix_rotation * QVector3D(l + l1, -b/2, i + s),
        position + matrix_rotation * QVector3D(l + l1, b/2, i + s),
        position + matrix_rotation * QVector3D(l1, b/2, s)
    };
    floor->arrayBufVertices->bind();
    floor->arrayBufVertices->allocate(vertices_flr, sizeof(vertices_flr));

    QVector3D position_lft = position + matrix_rotation * QVector3D(l/2, -b/2 + s/2, (a+s)/2);
    left->wizardParams.insert("Position x", position_lft.x());
    left->wizardParams.insert("Position y", position_lft.y());
    left->wizardParams.insert("Position z", position_lft.z());
    left->wizardParams.insert("Angle x", angle_x);
    left->wizardParams.insert("Angle y", angle_y);
    left->wizardParams.insert("Angle z", angle_z);

    left->wizardParams.insert("l", l);
    left->wizardParams.insert("b", s);
    left->wizardParams.insert("a", a-s);
    left->layer = this->layer;
    left->processWizardInput();
    left->calculate();

    QVector3D vertices_lft[] = {
        position + matrix_rotation * QVector3D(l1, b/2 - s, s),
        position + matrix_rotation * QVector3D(l1 + l, b/2 - s, s + i),
        position + matrix_rotation * QVector3D(l1 + l, b/2, s + i),
        position + matrix_rotation * QVector3D(l1, b/2, s),
        position + matrix_rotation * QVector3D(l1, b/2 - s, a),
        position + matrix_rotation * QVector3D(l1 + l, b/2 - s, a + i),
        position + matrix_rotation * QVector3D(l1 + l, b/2, a + i),
        position + matrix_rotation * QVector3D(l1, b/2, a)
    };
    left->arrayBufVertices->bind();
    left->arrayBufVertices->allocate(vertices_lft, sizeof(vertices_lft));

    QVector3D position_rgt = position + matrix_rotation * QVector3D(l/2, +b/2 - s/2, (a+s)/2);
    right->wizardParams.insert("Position x", position_rgt.x());
    right->wizardParams.insert("Position y", position_rgt.y());
    right->wizardParams.insert("Position z", position_rgt.z());
    right->wizardParams.insert("Angle x", angle_x);
    right->wizardParams.insert("Angle y", angle_y);
    right->wizardParams.insert("Angle z", angle_z);

    right->wizardParams.insert("l", l);
    right->wizardParams.insert("b", s);
    right->wizardParams.insert("a", a-s);
    right->layer = this->layer;
    right->processWizardInput();
    right->calculate();

    QVector3D vertices_rgt[] = {
        position + matrix_rotation * QVector3D(l1 + l, -b/2 + s, s + i),
        position + matrix_rotation * QVector3D(l1, -b/2 + s, s),
        position + matrix_rotation * QVector3D(l1, -b/2, s),
        position + matrix_rotation * QVector3D(l1 + l, -b/2, s + i),
        position + matrix_rotation * QVector3D(l1 + l, -b/2 + s, a + i),
        position + matrix_rotation * QVector3D(l1, -b/2 + s, a),
        position + matrix_rotation * QVector3D(l1, -b/2, a),
        position + matrix_rotation * QVector3D(l1 + l, -b/2, a + i),
    };
    right->arrayBufVertices->bind();
    right->arrayBufVertices->allocate(vertices_rgt, sizeof(vertices_rgt));

    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l + 2 * l1, 0.0, i));

    this->snap_center.append(floor->snap_center);
    this->snap_center.append(left->snap_center);
    this->snap_center.append(right->snap_center);
    this->snap_center.append(end_lower->snap_center);
    this->snap_center.append(end_upper->snap_center);

    this->snap_vertices.append(floor->snap_vertices);
    this->snap_vertices.append(left->snap_vertices);
    this->snap_vertices.append(right->snap_vertices);
    this->snap_vertices.append(end_lower->snap_vertices);
    this->snap_vertices.append(end_upper->snap_vertices);

    this->boundingBox.enterVertices(floor->boundingBox.getVertices());
    this->boundingBox.enterVertices(left->boundingBox.getVertices());
    this->boundingBox.enterVertices(right->boundingBox.getVertices());
    this->boundingBox.enterVertices(end_lower->boundingBox.getVertices());
    this->boundingBox.enterVertices(end_upper->boundingBox.getVertices());
}

void CAD_Electrical_CabletrayTransition::processWizardInput()
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
    l1 = wizardParams.value("l1").toDouble();
    i = wizardParams.value("i").toDouble();
    s = wizardParams.value("s").toDouble();
}

//void CAD_Electrical_CabletrayTransition::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Electrical_CabletrayTransition::rotationOfFlange(quint8 num)
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
