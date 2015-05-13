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

#include "cad_sprinkler_pipeendcap.h"
#include "glwidget.h"

CAD_sprinkler_pipeEndCap::CAD_sprinkler_pipeEndCap() : CADitem(CADitemTypes::Sprinkler_PipeEndCap)
{
    endcap_outer = new CAD_Basic_TorisphericalHeadDIN28011;
    endcap_inner = new CAD_Basic_TorisphericalHeadDIN28011;
    this->subItems.append(endcap_outer);
    this->subItems.append(endcap_inner);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);
    wizardParams.insert("d", 150.0);
    wizardParams.insert("l", 100.0);
    wizardParams.insert("s",  10.0);
    wizardParams.insert("fe", 10.0);
    wizardParams.insert("ff", 10.0);

    arrayBufVertices = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    arrayBufVertices.create();
    arrayBufVertices.setUsagePattern(QOpenGLBuffer::StaticDraw);

    indexBufFaces = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBufFaces.create();
    indexBufFaces.setUsagePattern(QOpenGLBuffer::StaticDraw);

    processWizardInput();
    calculate();
}

CAD_sprinkler_pipeEndCap::~CAD_sprinkler_pipeEndCap()
{

}

QList<CADitemTypes::ItemType> CAD_sprinkler_pipeEndCap::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Sprinkler_CompressedAirWaterContainer);
    flangable_items.append(CADitemTypes::Sprinkler_Distribution);
    flangable_items.append(CADitemTypes::Sprinkler_Head);
    flangable_items.append(CADitemTypes::Sprinkler_Pipe);
    flangable_items.append(CADitemTypes::Sprinkler_PipeEndCap);
    flangable_items.append(CADitemTypes::Sprinkler_PipeReducer);
    flangable_items.append(CADitemTypes::Sprinkler_PipeTurn);
    flangable_items.append(CADitemTypes::Sprinkler_Pump);
    flangable_items.append(CADitemTypes::Sprinkler_PipeTeeConnector);
    flangable_items.append(CADitemTypes::Sprinkler_Valve);
    flangable_items.append(CADitemTypes::Sprinkler_WetAlarmValve);
    flangable_items.append(CADitemTypes::Sprinkler_ZoneCheck);

    return flangable_items;
}

QImage CAD_sprinkler_pipeEndCap::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    ;

    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_sprinkler_pipeEndCap::iconPath()
{
    return ":/icons/cad_sprinkler/cad_sprinkler_pipeendcap.svg";
}

QString CAD_sprinkler_pipeEndCap::domain()
{
    return "Sprinkler";
}

QString CAD_sprinkler_pipeEndCap::description()
{
    return "Sprinkler|Pipe End Cap";
}

void CAD_sprinkler_pipeEndCap::calculate()
{
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    endcap_outer->wizardParams.insert("Position x", position.x());
    endcap_outer->wizardParams.insert("Position y", position.y());
    endcap_outer->wizardParams.insert("Position z", position.z());
    endcap_outer->wizardParams.insert("Angle x", angle_x);
    endcap_outer->wizardParams.insert("Angle y", angle_y);
    endcap_outer->wizardParams.insert("Angle z", angle_z);
    endcap_outer->wizardParams.insert("d", d);   // Durchmesser
    qreal h = l - 0.1937742252 * d; // h - (1-sqrt(0.65)) * d
    endcap_outer->wizardParams.insert("h", h);     // Höhe
    endcap_outer->layer = this->layer;
    endcap_outer->processWizardInput();
    endcap_outer->rotateAroundAxis(90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    endcap_outer->calculate();

    endcap_inner->wizardParams.insert("Position x", position.x());
    endcap_inner->wizardParams.insert("Position y", position.y());
    endcap_inner->wizardParams.insert("Position z", position.z());
    endcap_inner->wizardParams.insert("Angle x", angle_x);
    endcap_inner->wizardParams.insert("Angle y", angle_y);
    endcap_inner->wizardParams.insert("Angle z", angle_z);
    endcap_inner->wizardParams.insert("d", d - 2 * s);   // Durchmesser
    h = l - 0.1937742252 * (d - 2*s); // h - (1-sqrt(0.65)) * d
    endcap_inner->wizardParams.insert("h", h);     // Höhe
    endcap_inner->layer = this->layer;
    endcap_inner->processWizardInput();
    endcap_inner->rotateAroundAxis(90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    endcap_inner->calculate();

    QVector3D vertices[32];
    for(int k = 0; k < 16; k++)
        vertices[k] = position + matrix_rotation * QVector3D(0.0, sin(k * PI * 0.125) * d/2, cos(k * PI *0.125) * d/2);
    for(int k = 0; k < 16; k++)
        vertices[16 + k] = position + matrix_rotation * QVector3D(0.0, sin(k * PI * 0.125) * (d/2-s), cos(k * PI *0.125) * (d/2-s));

    GLushort indicesFaces[] =
    {
        16, 0, 17, 1, 18, 2, 19, 3, 20, 4, 21, 5, 22, 6, 23, 7, 24, 8, 25, 9, 26, 10, 27, 11, 28, 12, 29, 13, 30, 14, 31, 15, 16, 0
    };



    arrayBufVertices.bind();
    arrayBufVertices.allocate(vertices, sizeof(vertices));

    indexBufFaces.bind();
    indexBufFaces.allocate(indicesFaces, sizeof(indicesFaces));


    foreach(CADitem *item, subItems)
        this->boundingBox.enterVertices(item->boundingBox.getVertices());
    this->snap_flanges.append(position);
}

void CAD_sprinkler_pipeEndCap::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    d = wizardParams.value("d").toDouble();
    l = wizardParams.value("l").toDouble();
    s = wizardParams.value("s").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

QMatrix4x4 CAD_sprinkler_pipeEndCap::rotationOfFlange(quint8 num)
{
    if(num == 1)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(180.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else
        return matrix_rotation;
}

void CAD_sprinkler_pipeEndCap::paint(GLWidget *glwidget)
{
    QColor color_brush_tmp = getColorBrush();

    glwidget->glEnable(GL_PRIMITIVE_RESTART);
    glwidget->glPrimitiveRestartIndex(0xABCD);

    arrayBufVertices.bind();
    glwidget->shaderProgram->enableAttributeArray(glwidget->shader_vertexLocation);
    glwidget->shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

    if (glwidget->render_solid)
    {
        glwidget->setPaintingColor(color_brush_tmp);

        indexBufFaces.bind();
        glwidget->glDrawElements(GL_TRIANGLE_STRIP, indexBufFaces.size(), GL_UNSIGNED_SHORT, 0);

        indexBufFaces.release();
    }

    arrayBufVertices.release();
}
