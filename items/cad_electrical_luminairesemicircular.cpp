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

#include "cad_electrical_luminairesemicircular.h"
#include "glwidget.h"

CAD_Electrical_LuminaireSemicircular::CAD_Electrical_LuminaireSemicircular() : CADitem(CADitemTypes::Electrical_LuminaireSemicircular)
{
    mount = new CAD_basic_pipe;
    hemi = new CAD_Basic_Hemisphere;
    this->subItems.append(mount);
    this->subItems.append(hemi);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a1", 50.0);        // Höhe des Sockels
    wizardParams.insert("a2", 50.0);        // Höhe der Rundung
    wizardParams.insert("d1", 200.0);       // Durchmesser des Sockels
    wizardParams.insert("d2", 250.0);       // Durchmesser der Rundung


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

CAD_Electrical_LuminaireSemicircular::~CAD_Electrical_LuminaireSemicircular()
{
//    arrayBufVertices.destroy();
//    indexBufFaces.destroy();
//    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_Electrical_LuminaireSemicircular::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Electrical_LuminaireSemicircular::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Electrical_LuminaireSemicircular::iconPath()
{
    return ":/icons/cad_electrical/cad_electrical_luminairesemicircular.svg";
}

QString CAD_Electrical_LuminaireSemicircular::domain()
{
    return "Electrical";
}

QString CAD_Electrical_LuminaireSemicircular::description()
{
    return "Electrical|Luminaire Semicircular";
}

void CAD_Electrical_LuminaireSemicircular::calculate()
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

    mount->wizardParams.insert("Position x", position.x());
    mount->wizardParams.insert("Position y", position.y());
    mount->wizardParams.insert("Position z", position.z());
    mount->wizardParams.insert("Angle x", angle_x);
    mount->wizardParams.insert("Angle y", angle_y);
    mount->wizardParams.insert("Angle z", angle_z);
    mount->wizardParams.insert("l", a1);
    mount->wizardParams.insert("d", d1);
    mount->wizardParams.insert("s", d1/2);
    mount->layer = this->layer;
    mount->processWizardInput();
    mount->calculate();

    qreal r = ((a2 * a2) + (d2 * d2 / 4)) / (2 * a2);
    qreal psi = asin(d2 / (2*r)) / PI * 180;
    QVector3D position_hemi = position + matrix_rotation * QVector3D(-r + a1 + a2, 0.0, 0.0);
    hemi->wizardParams.insert("Position x", position_hemi.x());
    hemi->wizardParams.insert("Position y", position_hemi.y());
    hemi->wizardParams.insert("Position z", position_hemi.z());
    hemi->wizardParams.insert("Angle x", angle_x);
    hemi->wizardParams.insert("Angle y", angle_y);
    hemi->wizardParams.insert("Angle z", angle_z);
    hemi->wizardParams.insert("Radius", r);
    hemi->wizardParams.insert("Alpha", psi);
    hemi->layer = this->layer;
    hemi->processWizardInput();
    hemi->calculate();

    this->boundingBox.enterVertices(mount->boundingBox.getVertices());
    this->boundingBox.enterVertices(hemi->boundingBox.getVertices());
    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.0, 0.0, - a1 - a2));
}

void CAD_Electrical_LuminaireSemicircular::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a1 = wizardParams.value("a1").toDouble();
    a2 = wizardParams.value("a2").toDouble();
    d1 = wizardParams.value("d1").toDouble();
    d2 = wizardParams.value("d2").toDouble();
}

//void CAD_Electrical_LuminaireSemicircular::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Electrical_LuminaireSemicircular::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}
