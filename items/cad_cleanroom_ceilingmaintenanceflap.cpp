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

#include "cad_cleanroom_ceilingmaintenanceflap.h"

CAD_Cleanroom_CeilingMaintenanceFlap::CAD_Cleanroom_CeilingMaintenanceFlap() : CADitem(CADitemTypes::Cleanroom_CeilingMaintenanceFlap)
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
    wizardParams.insert("g2", 550.0);
    wizardParams.insert("l2", 550.0);

    panel = new CAD_basic_duct;
    flap = new CAD_basic_box;
    this->subItems.append(panel);
    this->subItems.append(flap);

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

CAD_Cleanroom_CeilingMaintenanceFlap::~CAD_Cleanroom_CeilingMaintenanceFlap()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Cleanroom_CeilingMaintenanceFlap::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Cleanroom_CeilingMaintenanceFlap::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Cleanroom_CeilingMaintenanceFlap::iconPath()
{
    return ":/icons/cad_cleanroom/cad_cleanroom_ceilingmaintenanceflap.svg";
}

QString CAD_Cleanroom_CeilingMaintenanceFlap::domain()
{
    return "Cleanroom";
}

QString CAD_Cleanroom_CeilingMaintenanceFlap::description()
{
    return "Cleanroom|Ceiling Maintenance Flap";
}

void CAD_Cleanroom_CeilingMaintenanceFlap::calculate()
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

    QMatrix4x4 matrix_flap;
    matrix_flap.setToIdentity();
    matrix_flap.rotate(30.0, 0.0, 1.0, 0.0);
    QVector3D position_flap = position + matrix_rotation * (QVector3D((l - l2)/2, g/2, 0.0) + matrix_flap * QVector3D(l2/2, 0.0, 0.0));
    flap->wizardParams.insert("Position x", position_flap.x());
    flap->wizardParams.insert("Position y", position_flap.y());
    flap->wizardParams.insert("Position z", position_flap.z());
    flap->wizardParams.insert("Angle x", angle_x);
    flap->wizardParams.insert("Angle y", angle_y);
    flap->wizardParams.insert("Angle z", angle_z);

    flap->wizardParams.insert("l", l2);
    flap->wizardParams.insert("b", g2);
    flap->wizardParams.insert("a", h);
    flap->layer = this->layer;
    flap->processWizardInput();
    flap->rotateAroundAxis(30.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    flap->calculate();

    QVector3D position_panel = position + matrix_rotation * QVector3D(l/2, g/2, 0.0);
    panel->wizardParams.insert("Position x", position_panel.x());
    panel->wizardParams.insert("Position y", position_panel.y());
    panel->wizardParams.insert("Position z", position_panel.z());
    panel->wizardParams.insert("Angle x", angle_x);
    panel->wizardParams.insert("Angle y", angle_y);
    panel->wizardParams.insert("Angle z", angle_z);
    panel->wizardParams.insert("a",  g);
    panel->wizardParams.insert("b",  l);
    panel->wizardParams.insert("l",  h);
    panel->wizardParams.insert("s",  (g - g2)/2);
    panel->layer = this->layer;
    panel->processWizardInput();
    panel->rotateAroundAxis(-90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    panel->calculate();

    this->boundingBox.enterVertices(panel->boundingBox.getVertices());
    this->boundingBox.enterVertices(flap->boundingBox.getVertices());
    this->snap_vertices = panel->snap_vertices;
}

void CAD_Cleanroom_CeilingMaintenanceFlap::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    h = wizardParams.value("h").toDouble();
    g = wizardParams.value("g").toDouble();
    l = wizardParams.value("l").toDouble();
    g2 = wizardParams.value("g2").toDouble();
    l2 = wizardParams.value("l2").toDouble();

}

//void CAD_cleanroom_CeilingMaintenanceFlap::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Cleanroom_CeilingMaintenanceFlap::rotationOfFlange(quint8 num)
{
    Q_UNUSED(num)
    return matrix_rotation;
}
