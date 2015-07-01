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

#include "cad_heatcool_radiatorcompact.h"
#include "glwidget.h"

CAD_HeatCool_RadiatorCompact::CAD_HeatCool_RadiatorCompact() : CADitem(CADitemTypes::HeatCool_RadiatorCompact)
{
    radiator = new CAD_basic_box;
    this->subItems.append(radiator);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",  800.0);
    wizardParams.insert("a2", 600.0);
    wizardParams.insert("b",  150.0);
    wizardParams.insert("l", 1500.0);


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

CAD_HeatCool_RadiatorCompact::~CAD_HeatCool_RadiatorCompact()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_HeatCool_RadiatorCompact::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::HeatCool_Pipe);
    flangable_items.append(CADitemTypes::HeatCool_PipeEndCap);
    flangable_items.append(CADitemTypes::HeatCool_PipeReducer);
    flangable_items.append(CADitemTypes::HeatCool_PipeTeeConnector);
    flangable_items.append(CADitemTypes::HeatCool_PipeTurn);
    return flangable_items;
}

QImage CAD_HeatCool_RadiatorCompact::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_HeatCool_RadiatorCompact::iconPath()
{
    return ":/icons/cad_heatcool/cad_heatcool_radiatorcompact.svg";
}

QString CAD_HeatCool_RadiatorCompact::domain()
{
    return "HeatCool";
}

QString CAD_HeatCool_RadiatorCompact::description()
{
    return "HeatCool|Radiator Compact";
}

void CAD_HeatCool_RadiatorCompact::calculate()
{                
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    QVector3D position_rad = position + matrix_rotation * QVector3D(l/2, 0.0, 0.0);
    radiator->wizardParams.insert("Position x", position_rad.x());
    radiator->wizardParams.insert("Position y", position_rad.y());
    radiator->wizardParams.insert("Position z", position_rad.z());
    radiator->wizardParams.insert("Angle x", angle_x);
    radiator->wizardParams.insert("Angle y", angle_y);
    radiator->wizardParams.insert("Angle z", angle_z);
    radiator->wizardParams.insert("l", l);
    radiator->wizardParams.insert("b", b);
    radiator->wizardParams.insert("a", a);
    radiator->layer = this->layer;
    radiator->processWizardInput();
    radiator->calculate();

    this->boundingBox = radiator->boundingBox;
    this->snap_center = radiator->snap_center;
    this->snap_vertices = radiator->snap_vertices;
    this->snap_flanges.append(position + matrix_rotation * QVector3D(0.0, 0.0, a2/2));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(0.0, 0.0, -a2/2));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, -a2/2));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, a2/2));
}

void CAD_HeatCool_RadiatorCompact::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a = wizardParams.value("a").toDouble();
    a2 = wizardParams.value("a2").toDouble();
    b = wizardParams.value("b").toDouble();
    l = wizardParams.value("l").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

//void CAD_HeatCool_RadiatorCompact::paint(GLWidget *glwidget)
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
//        glwidget->glLineWidth(1.0);

//        indexBufLines->bind();
//        glwidget->glDrawElements(GL_LINES, indexBufLines->size(), GL_UNSIGNED_SHORT, 0);
//        indexBufLines->release();
//     }                          

//     arrayBufVertices->release();
//}

QMatrix4x4 CAD_HeatCool_RadiatorCompact::rotationOfFlange(quint8 num)
{
    if(num == 1 || num == 2)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(180.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else if(num == 3 || num == 4)
    {
        return matrix_rotation;
    }
    else
        return matrix_rotation;
}
