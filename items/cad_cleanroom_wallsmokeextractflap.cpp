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

#include "cad_cleanroom_wallsmokeextractflap.h"

CAD_Cleanroom_WallSmokeExtractFlap::CAD_Cleanroom_WallSmokeExtractFlap() : CADitem(CADitemTypes::Cleanroom_WallSmokeExtractFlap)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 2500.0);
    wizardParams.insert("b",  100.0);
    wizardParams.insert("l", 2500.0);
    wizardParams.insert("s", 400.0);
    wizardParams.insert("t", 400.0);
    wizardParams.insert("alpha", -30.0);

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

CAD_Cleanroom_WallSmokeExtractFlap::~CAD_Cleanroom_WallSmokeExtractFlap()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Cleanroom_WallSmokeExtractFlap::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Cleanroom_WallOverflowGrate);
    flangable_items.append(CADitemTypes::Cleanroom_WallPanel);
    flangable_items.append(CADitemTypes::Cleanroom_WallSmokeExtractFlap);
    return flangable_items;
}

QImage CAD_Cleanroom_WallSmokeExtractFlap::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");   
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Cleanroom_WallSmokeExtractFlap::iconPath()
{
    return ":/icons/cad_cleanroom/cad_cleanroom_wallsmokeextractflap.svg";
}

QString CAD_Cleanroom_WallSmokeExtractFlap::domain()
{
    return "Cleanroom";
}

QString CAD_Cleanroom_WallSmokeExtractFlap::description()
{
    return "Cleanroom|Wall Smoke Extract Flap";
}

void CAD_Cleanroom_WallSmokeExtractFlap::calculate()
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

    QVector3D position_flap;
    if(alpha > 0.0)
    {
        QMatrix4x4 matrix_flap;
        matrix_flap.setToIdentity();
        matrix_flap.rotate(alpha, 1.0, 0.0, 0.0);
        position_flap = position + matrix_rotation * (QVector3D(0.5 * l, 0.5 * b, 0.5 * (a + s)) + matrix_flap * QVector3D(0.0, 0.0, -0.5 * s));
    }
    else
    {
        QMatrix4x4 matrix_flap;
        matrix_flap.setToIdentity();
        matrix_flap.rotate(alpha, 1.0, 0.0, 0.0);
        position_flap = position + matrix_rotation * (QVector3D(0.5 * l, 0.5 * b, 0.5 * (a - s)) + matrix_flap * QVector3D(0.0, 0.0, 0.5 * s));
    }
    flap->wizardParams.insert("Position x", position_flap.x());
    flap->wizardParams.insert("Position y", position_flap.y());
    flap->wizardParams.insert("Position z", position_flap.z());
    flap->wizardParams.insert("Angle x", angle_x);
    flap->wizardParams.insert("Angle y", angle_y);
    flap->wizardParams.insert("Angle z", angle_z);
    flap->wizardParams.insert("l", t);
    flap->wizardParams.insert("b", b);
    flap->wizardParams.insert("a", s);
    flap->layer = this->layer;
    flap->processWizardInput();
    flap->rotateAroundAxis(alpha, QVector3D(1.0, 0.0, 0.0), angle_x, angle_y, angle_z);
    flap->calculate();

    QVector3D position_panel = position + matrix_rotation * QVector3D(0.5 * l, -0.5 * b, 0.5 * a);
    panel->wizardParams.insert("Position x", position_panel.x());
    panel->wizardParams.insert("Position y", position_panel.y());
    panel->wizardParams.insert("Position z", position_panel.z());
    panel->wizardParams.insert("Angle x", angle_x);
    panel->wizardParams.insert("Angle y", angle_y);
    panel->wizardParams.insert("Angle z", angle_z);
    panel->wizardParams.insert("a",  a);
    panel->wizardParams.insert("b",  l);
    panel->wizardParams.insert("l",  b);
    panel->wizardParams.insert("s",  (l - t)/2);
    panel->layer = this->layer;
    panel->processWizardInput();
    panel->rotateAroundAxis(90.0, QVector3D(0.0, 0.0, 1.0), angle_x, angle_y, angle_z);
    panel->calculate();

    QVector3D vertices[] = {
        //set Outer Points
        position + matrix_rotation * QVector3D(0.0, -0.5 * b, 0.0),
        position + matrix_rotation * QVector3D(0.0,  0.5 * b, 0.0),
        position + matrix_rotation * QVector3D(l  ,  0.5 * b, 0.0),
        position + matrix_rotation * QVector3D(l  , -0.5 * b, 0.0),

        position + matrix_rotation * QVector3D(0.0, -0.5 * b, a),
        position + matrix_rotation * QVector3D(0.0,  0.5 * b, a),
        position + matrix_rotation * QVector3D(l  ,  0.5 * b, a),
        position + matrix_rotation * QVector3D(l  , -0.5 * b, a),
        //set inner Points
        position + matrix_rotation * QVector3D(0.5 * (l - t), -0.5 * b, 0.5 * (a - s)),
        position + matrix_rotation * QVector3D(0.5 * (l - t),  0.5 * b, 0.5 * (a - s)),
        position + matrix_rotation * QVector3D(0.5 * (l + t),  0.5 * b, 0.5 * (a - s)),
        position + matrix_rotation * QVector3D(0.5 * (l + t), -0.5 * b, 0.5 * (a - s)),

        position + matrix_rotation * QVector3D(0.5 * (l - t), -0.5 * b, 0.5 * (a + s)),
        position + matrix_rotation * QVector3D(0.5 * (l - t),  0.5 * b, 0.5 * (a + s)),
        position + matrix_rotation * QVector3D(0.5 * (l + t),  0.5 * b, 0.5 * (a + s)),
        position + matrix_rotation * QVector3D(0.5 * (l + t), -0.5 * b, 0.5 * (a + s)),
    };

    panel->arrayBufVertices->bind();
    panel->arrayBufVertices->allocate(vertices, sizeof(vertices));

    this->boundingBox.enterVertices(panel->boundingBox.getVertices());
    this->boundingBox.enterVertices(flap->boundingBox.getVertices());
    this->snap_vertices = panel->snap_vertices;
    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, 0.0));
}

void CAD_Cleanroom_WallSmokeExtractFlap::processWizardInput()
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
    t = wizardParams.value("t").toDouble();
    alpha = wizardParams.value("alpha").toDouble();
}

//void CAD_cleanroom_WallSmokeExtractFlap::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Cleanroom_WallSmokeExtractFlap::rotationOfFlange(quint8 num)
{
    Q_UNUSED(num)
    return matrix_rotation;
}
