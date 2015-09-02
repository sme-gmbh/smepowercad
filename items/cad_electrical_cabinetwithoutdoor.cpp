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

#include "cad_electrical_cabinetwithoutdoor.h"
#include "glwidget.h"

CAD_Electrical_CabinetWithoutDoor::CAD_Electrical_CabinetWithoutDoor() : CADitem(CADitemTypes::Electrical_CabinetWithoutDoor)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 1500.0);       // Höhe Schrank
    wizardParams.insert("a1", 200.0);       // Höhe Sockel
    wizardParams.insert("b", 500.0);        // Tiefe Sockel
    wizardParams.insert("l", 500.0);        // Breite
    wizardParams.insert("i", 100.0);        // Überhang Schrank
    wizardParams.insert("s", 10.0);         // Wandstärke

    cabinet = new CAD_basic_duct();
    back = new CAD_basic_box();
    socket = new CAD_basic_box();
    this->subItems.append(cabinet);
    this->subItems.append(back);
    this->subItems.append(socket);

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

CAD_Electrical_CabinetWithoutDoor::~CAD_Electrical_CabinetWithoutDoor()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Electrical_CabinetWithoutDoor::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Electrical_CabinetWithoutDoor::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");   
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Electrical_CabinetWithoutDoor::iconPath()
{
    return ":/icons/cad_electrical/cad_electrical_cabinetwithoutdoor.svg";
}

QString CAD_Electrical_CabinetWithoutDoor::domain()
{
    return "Electrical";
}

QString CAD_Electrical_CabinetWithoutDoor::description()
{
    return "Electrical|Cabinet Without Door";
}

void CAD_Electrical_CabinetWithoutDoor::calculate()
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

    QVector3D position_cab = position + matrix_rotation * QVector3D(l/2, b/2, a1 + a/2);
    cabinet->wizardParams.insert("Position x", position_cab.x());
    cabinet->wizardParams.insert("Position y", position_cab.y());
    cabinet->wizardParams.insert("Position z", position_cab.z());
    cabinet->wizardParams.insert("Angle x", angle_x);
    cabinet->wizardParams.insert("Angle y", angle_y);
    cabinet->wizardParams.insert("Angle z", angle_z-90.0);

    cabinet->wizardParams.insert("l", (b+i));
    cabinet->wizardParams.insert("b", (l));
    cabinet->wizardParams.insert("a", (a));
    cabinet->wizardParams.insert("s", (s));
    cabinet->layer = this->layer;
    cabinet->processWizardInput();
    cabinet->calculate();

    QVector3D position_sock = position + matrix_rotation * QVector3D(l/2, 0.0, a1/2);
    socket->wizardParams.insert("Position x", position_sock.x());
    socket->wizardParams.insert("Position y", position_sock.y());
    socket->wizardParams.insert("Position z", position_sock.z());
    socket->wizardParams.insert("Angle x", angle_x);
    socket->wizardParams.insert("Angle y", angle_y);
    socket->wizardParams.insert("Angle z", angle_z);

    socket->wizardParams.insert("l", l);
    socket->wizardParams.insert("b", b);
    socket->wizardParams.insert("a", a1);
    socket->layer = this->layer;
    socket->processWizardInput();
    socket->calculate();

    QVector3D position_back = position + matrix_rotation * QVector3D(l/2, b/2 - s/2, a1 + a/2);
    back->wizardParams.insert("Position x", position_back.x());
    back->wizardParams.insert("Position y", position_back.y());
    back->wizardParams.insert("Position z", position_back.z());
    back->wizardParams.insert("Angle x", angle_x);
    back->wizardParams.insert("Angle y", angle_y);
    back->wizardParams.insert("Angle z", angle_z);

    back->wizardParams.insert("l", l - 2*s);
    back->wizardParams.insert("b", s);
    back->wizardParams.insert("a", a - 2*s);
    back->layer = this->layer;
    back->processWizardInput();
    back->calculate();

    this->boundingBox.enterVertices(cabinet->boundingBox.getVertices());
    this->boundingBox.enterVertices(socket->boundingBox.getVertices());
    this->boundingBox.enterVertices(back->boundingBox.getVertices());

    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.0, -0.5*b, 0.0));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(l,   -0.5*b, 0.0));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(l,    0.5*b, 0.0));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.0,  0.5*b, 0.0));

    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.0, -0.5*b - i, a + a1));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(l,   -0.5*b - i, a + a1));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(l,    0.5*b, a + a1));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.0,  0.5*b, a + a1));

    this->snap_vertices.append(position + matrix_rotation * QVector3D(l,    -0.5*b - i, a1));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.0,  -0.5*b - i, a1));
}

void CAD_Electrical_CabinetWithoutDoor::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a = wizardParams.value("a").toDouble();
    a1 = wizardParams.value("a1").toDouble();
    b = wizardParams.value("b").toDouble();
    l = wizardParams.value("l").toDouble();
    i = wizardParams.value("i").toDouble();
    s = wizardParams.value("s").toDouble();

}

//void CAD_Electrical_CabinetWithoutDoor::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Electrical_CabinetWithoutDoor::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}
