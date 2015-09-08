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

#include "cad_cleanroom_ceilinggridfeedthrough.h"
#include "glwidget.h"

CAD_Cleanroom_CeilingGridFeedThrough::CAD_Cleanroom_CeilingGridFeedThrough() : CADitem(CADitemTypes::Cleanroom_CeilingGridFeedThrough)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 50.0);
    wizardParams.insert("b", 20.0);
    wizardParams.insert("l", 50.0);
    wizardParams.insert("d", 10.0);

    grid = new CAD_basic_box();
    circle_up = new CAD_basic_circle();
    circle_low = new CAD_basic_circle();
    this->subItems.append(grid);
    this->subItems.append(circle_up);
    this->subItems.append(circle_low);

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

CAD_Cleanroom_CeilingGridFeedThrough::~CAD_Cleanroom_CeilingGridFeedThrough()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Cleanroom_CeilingGridFeedThrough::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Cleanroom_CeilingCornerPiece);
    flangable_items.append(CADitemTypes::Cleanroom_CeilingJoiningKnot);
    flangable_items.append(CADitemTypes::Cleanroom_CeilingGrid);
    flangable_items.append(CADitemTypes::Cleanroom_CeilingGridFeedThrough);
    flangable_items.append(CADitemTypes::Cleanroom_CeilingTeeJoiningPiece);

    return flangable_items;
}

QImage CAD_Cleanroom_CeilingGridFeedThrough::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");   
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Cleanroom_CeilingGridFeedThrough::iconPath()
{
    return ":/icons/cad_cleanroom/cad_cleanroom_ceilinggridfeedthrough.svg";
}

QString CAD_Cleanroom_CeilingGridFeedThrough::domain()
{
    return "Cleanroom";
}

QString CAD_Cleanroom_CeilingGridFeedThrough::description()
{
    return "Cleanroom|Ceiling Grid Feed Through";
}

void CAD_Cleanroom_CeilingGridFeedThrough::calculate()
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
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, 0.0));
    this->snap_flanges.append(position);

    QVector3D position_box = position + matrix_rotation * QVector3D(l/2, 0.0, a/2);
    grid->wizardParams.insert("Position x", position_box.x());
    grid->wizardParams.insert("Position y", position_box.y());
    grid->wizardParams.insert("Position z", position_box.z());
    grid->wizardParams.insert("Angle x", angle_x);
    grid->wizardParams.insert("Angle y", angle_y);
    grid->wizardParams.insert("Angle z", angle_z);

    grid->wizardParams.insert("l", l);
    grid->wizardParams.insert("b", b);
    grid->wizardParams.insert("a", a);
    grid->layer = this->layer;
    grid->processWizardInput();
    grid->calculate();

    QVector3D pos_circle_low = (matrix_rotation * QVector3D(l * 0.5, 0.0, 0.0)) + position;
    circle_low->wizardParams.insert("Center x", (pos_circle_low.x()));
    circle_low->wizardParams.insert("Center y", (pos_circle_low.y()));
    circle_low->wizardParams.insert("Center z", (pos_circle_low.z()));
    circle_low->wizardParams.insert("Angle x", (angle_x));
    circle_low->wizardParams.insert("Angle y", (angle_y));
    circle_low->wizardParams.insert("Angle z", (angle_z));
    circle_low->wizardParams.insert("r", (0.5 * d));
    circle_low->processWizardInput();
    circle_low->calculate();
    circle_low->rotateAroundAxis(180.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    circle_low->layer = this->layer;

    QVector3D pos_circle_up = (matrix_rotation * QVector3D(l * 0.5, 0.0, a)) + position;
    circle_up->wizardParams.insert("Center x", (pos_circle_up.x()));
    circle_up->wizardParams.insert("Center y", (pos_circle_up.y()));
    circle_up->wizardParams.insert("Center z", (pos_circle_up.z()));
    circle_up->wizardParams.insert("Angle x", (angle_x));
    circle_up->wizardParams.insert("Angle y", (angle_y));
    circle_up->wizardParams.insert("Angle z", (angle_z));
    circle_up->wizardParams.insert("r", (0.5 * d));
    circle_up->processWizardInput();
    circle_up->calculate();
    circle_up->layer = this->layer;

    this->boundingBox = grid->boundingBox;

    this->snap_vertices.append(grid->snap_vertices);
    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.5 * l, 0.0, 0.0));
    this->snap_vertices.append(position + matrix_rotation * QVector3D(0.5 * l, 0.0, a));
}

void CAD_Cleanroom_CeilingGridFeedThrough::processWizardInput()
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
    d = wizardParams.value("d").toDouble();
}

//void CAD_Cleanroom_CeilingGridFeedThrough::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Cleanroom_CeilingGridFeedThrough::rotationOfFlange(quint8 num)
{
    if(num == 2)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(180.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else
        return matrix_rotation;
}
