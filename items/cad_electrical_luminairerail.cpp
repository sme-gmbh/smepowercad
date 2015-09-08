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

#include "cad_electrical_luminairerail.h"
#include "glwidget.h"

CAD_Electrical_LuminaireRail::CAD_Electrical_LuminaireRail() : CADitem(CADitemTypes::Electrical_LuminaireRail)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("l", 1000.0);
    wizardParams.insert("g",   50.0);
    wizardParams.insert("h",   50.0);

    rail = new CAD_basic_box();
    this->subItems.append(rail);

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

CAD_Electrical_LuminaireRail::~CAD_Electrical_LuminaireRail()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Electrical_LuminaireRail::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Electrical_LuminaireRail);
    flangable_items.append(CADitemTypes::Electrical_LuminaireRailMounted);
    return flangable_items;
}

QImage CAD_Electrical_LuminaireRail::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Electrical_LuminaireRail::iconPath()
{
    return ":/icons/cad_electrical/cad_electrical_luminairerail.svg";
}

QString CAD_Electrical_LuminaireRail::domain()
{
    return "Electrical";
}

QString CAD_Electrical_LuminaireRail::description()
{
    return "Electrical|Luminaire Rail";
}

void CAD_Electrical_LuminaireRail::calculate()
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

    QVector3D position_rail = position + matrix_rotation * QVector3D(l/2, 0.0, h/2);
    rail->wizardParams.insert("Position x", position_rail.x());
    rail->wizardParams.insert("Position y", position_rail.y());
    rail->wizardParams.insert("Position z", position_rail.z());
    rail->wizardParams.insert("Angle x", angle_x);
    rail->wizardParams.insert("Angle y", angle_y);
    rail->wizardParams.insert("Angle z", angle_z);
    rail->wizardParams.insert("l", l);
    rail->wizardParams.insert("b", g);
    rail->wizardParams.insert("a", h);
    rail->layer = this->layer;
    rail->processWizardInput();
    rail->calculate();

    this->boundingBox.enterVertices(rail->boundingBox.getVertices());

    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, 0.0));
    this->snap_vertices.append(rail->snap_vertices.at(4));
    this->snap_vertices.append(rail->snap_vertices.at(5));
    this->snap_vertices.append(rail->snap_vertices.at(6));
    this->snap_vertices.append(rail->snap_vertices.at(7));
}

void CAD_Electrical_LuminaireRail::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    g = wizardParams.value("g").toDouble();
    h = wizardParams.value("h").toDouble();
    l = wizardParams.value("l").toDouble();
}

QMatrix4x4 CAD_Electrical_LuminaireRail::rotationOfFlange(quint8 num)
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

//void CAD_Electrical_LuminaireRail::paint(GLWidget *glwidget)
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
