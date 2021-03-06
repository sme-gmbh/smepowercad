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

#include "cad_air_ductypiece.h"

#include "glwidget.h"

CAD_air_ductYpiece::CAD_air_ductYpiece() : CADitem(CADitemTypes::Air_DuctYpiece)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",  200.0);
    wizardParams.insert("a2", 300.0);
    wizardParams.insert("b",  300.0);
    wizardParams.insert("b2", 100.0);
    wizardParams.insert("b3", 100.0);
    wizardParams.insert("l", 1000.0);
    wizardParams.insert("e",    0.0);
    wizardParams.insert("f",    0.0);
    wizardParams.insert("m",  100.0);
    wizardParams.insert("u",   50.0);
    wizardParams.insert("fe",  10.0);
    wizardParams.insert("ff",  10.0);
    wizardParams.insert("s",    1.0);

    endcap_1 = new CAD_basic_duct();
    endcap_2 = new CAD_basic_duct();
    endcap_3 = new CAD_basic_duct();
    flange_1 = new CAD_basic_duct();
    flange_2 = new CAD_basic_duct();
    flange_3 = new CAD_basic_duct();
    subItems.append(endcap_1);
    subItems.append(endcap_2);
    subItems.append(endcap_3);
    subItems.append(flange_1);
    subItems.append(flange_2);
    subItems.append(flange_3);

    arrayBufVertices = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    arrayBufVertices->create();
    arrayBufVertices->setUsagePattern(QOpenGLBuffer::StaticDraw);

    indexBufFaces = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBufFaces->create();
    indexBufFaces->setUsagePattern(QOpenGLBuffer::StaticDraw);

    indexBufLines = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBufLines->create();
    indexBufLines->setUsagePattern(QOpenGLBuffer::StaticDraw);

    processWizardInput();
    calculate();
}

CAD_air_ductYpiece::~CAD_air_ductYpiece()
{

}

QList<CADitemTypes::ItemType> CAD_air_ductYpiece::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Air_Duct);
    flangable_items.append(CADitemTypes::Air_DuctEndPlate);
    flangable_items.append(CADitemTypes::Air_DuctFireDamper);
    flangable_items.append(CADitemTypes::Air_DuctTransitionRectRect);
    flangable_items.append(CADitemTypes::Air_DuctTransitionRectRound);
    flangable_items.append(CADitemTypes::Air_DuctTurn);
    flangable_items.append(CADitemTypes::Air_DuctVolumetricFlowController);
    flangable_items.append(CADitemTypes::Air_DuctYpiece);
    flangable_items.append(CADitemTypes::Air_Filter);
    flangable_items.append(CADitemTypes::Air_HeatExchangerAirAir);
    flangable_items.append(CADitemTypes::Air_HeatExchangerWaterAir);
    flangable_items.append(CADitemTypes::Air_MultiLeafDamper);
    return flangable_items;
}

QImage CAD_air_ductYpiece::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_ductYpiece::iconPath()
{
    return ":/icons/cad_air/cad_air_ductypiece.svg";
}

QString CAD_air_ductYpiece::domain()
{
    return "Air";
}

QString CAD_air_ductYpiece::description()
{
    return "Air|Duct Y-Piece";
}

void CAD_air_ductYpiece::calculate()
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

    QVector3D position_e1 = position + matrix_rotation * QVector3D(0.0, 0.0, 0.0);
    QVector3D position_f1 = position + matrix_rotation * QVector3D(0.0, 0.0, 0.0);
    QVector3D position_f2 = position + matrix_rotation * QVector3D(l - fe, b/2 + e - b2/2, a/2 + f - a2/2);
    QVector3D position_f3 = position + matrix_rotation * QVector3D(l - fe, b/2 + e - b2 - m - b3/2, a/2 + f - a2/2);
    QVector3D position_e2 = position + matrix_rotation * QVector3D(l - u, b/2 + e - b2/2, a/2 + f - a2/2);
    QVector3D position_e3 = position + matrix_rotation * QVector3D(l - u, b/2 + e - b2 - m - b3/2, a/2 + f - a2/2);

    endcap_1->wizardParams.insert("Position x", (position_e1.x()));
    endcap_1->wizardParams.insert("Position y", (position_e1.y()));
    endcap_1->wizardParams.insert("Position z", (position_e1.z()));
    endcap_1->wizardParams.insert("Angle x", (angle_x));
    endcap_1->wizardParams.insert("Angle y", (angle_y));
    endcap_1->wizardParams.insert("Angle z", (angle_z));
    endcap_1->wizardParams.insert("l", (u));
    endcap_1->wizardParams.insert("b", (b));
    endcap_1->wizardParams.insert("a", (a));
    endcap_1->wizardParams.insert("s", (s));
    endcap_1->processWizardInput();
    endcap_1->calculate();

    flange_1->wizardParams.insert("Position x", (position_f1.x()));
    flange_1->wizardParams.insert("Position y", (position_f1.y()));
    flange_1->wizardParams.insert("Position z", (position_f1.z()));
    flange_1->wizardParams.insert("Angle x", (angle_x));
    flange_1->wizardParams.insert("Angle y", (angle_y));
    flange_1->wizardParams.insert("Angle z", (angle_z));
    flange_1->wizardParams.insert("l", (fe));
    flange_1->wizardParams.insert("b", (b + 2 * ff));
    flange_1->wizardParams.insert("a", (a + 2 * ff));
    flange_1->wizardParams.insert("s", (ff));
    flange_1->processWizardInput();
    flange_1->calculate();

    endcap_2->wizardParams.insert("Position x", (position_e2.x()));
    endcap_2->wizardParams.insert("Position y", (position_e2.y()));
    endcap_2->wizardParams.insert("Position z", (position_e2.z()));
    endcap_2->wizardParams.insert("Angle x", (angle_x));
    endcap_2->wizardParams.insert("Angle y", (angle_y));
    endcap_2->wizardParams.insert("Angle z", (angle_z));
    endcap_2->wizardParams.insert("l", (u));
    endcap_2->wizardParams.insert("b", (b2));
    endcap_2->wizardParams.insert("a", (a2));
    endcap_2->wizardParams.insert("s", (s));
    endcap_2->processWizardInput();
    endcap_2->calculate();

    flange_2->wizardParams.insert("Position x", (position_f2.x()));
    flange_2->wizardParams.insert("Position y", (position_f2.y()));
    flange_2->wizardParams.insert("Position z", (position_f2.z()));
    flange_2->wizardParams.insert("Angle x", (angle_x));
    flange_2->wizardParams.insert("Angle y", (angle_y));
    flange_2->wizardParams.insert("Angle z", (angle_z));
    flange_2->wizardParams.insert("l", (fe));
    flange_2->wizardParams.insert("b", (b2 + 2 * ff));
    flange_2->wizardParams.insert("a", (a2 + 2 * ff));
    flange_2->wizardParams.insert("s", (ff));
    flange_2->processWizardInput();
    flange_2->calculate();

    endcap_3->wizardParams.insert("Position x", (position_e3.x()));
    endcap_3->wizardParams.insert("Position y", (position_e3.y()));
    endcap_3->wizardParams.insert("Position z", (position_e3.z()));
    endcap_3->wizardParams.insert("Angle x", (angle_x));
    endcap_3->wizardParams.insert("Angle y", (angle_y));
    endcap_3->wizardParams.insert("Angle z", (angle_z));
    endcap_3->wizardParams.insert("l", (u));
    endcap_3->wizardParams.insert("b", (b3));
    endcap_3->wizardParams.insert("a", (a2));
    endcap_3->wizardParams.insert("s", (s));
    endcap_3->processWizardInput();
    endcap_3->calculate();

    flange_3->wizardParams.insert("Position x", (position_f3.x()));
    flange_3->wizardParams.insert("Position y", (position_f3.y()));
    flange_3->wizardParams.insert("Position z", (position_f3.z()));
    flange_3->wizardParams.insert("Angle x", (angle_x));
    flange_3->wizardParams.insert("Angle y", (angle_y));
    flange_3->wizardParams.insert("Angle z", (angle_z));
    flange_3->wizardParams.insert("l", (fe));
    flange_3->wizardParams.insert("b", (b3 + 2 * ff));
    flange_3->wizardParams.insert("a", (a2 + 2 * ff));
    flange_3->wizardParams.insert("s", (ff));
    flange_3->processWizardInput();
    flange_3->calculate();

    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, b/2 + e - b2/2, a/2 + f - a2/2));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, b/2 + e - b2 - m - b3/2, a/2 + f - a2/2));

    splitPoint[0] = position + matrix_rotation * QVector3D(l/2, (b/2 + e - b2 - m/2)/2, (f - a2) / 2);
    splitPoint[1] = position + matrix_rotation * QVector3D(l/2, (b/2 + e - b2 - m/2)/2, (a + f) / 2);
    splitPoint[2] = position + matrix_rotation * QVector3D(l/2, (b/2 + e - b2 - m/2)/2, (f - a2) / 2 + s);
    splitPoint[3] = position + matrix_rotation * QVector3D(l/2, (b/2 + e - b2 - m/2)/2, (a + f) / 2 - s);

    boundingBox.enterVertex(flange_1->pos_bot_1);
    boundingBox.enterVertex(flange_1->pos_bot_2);
    boundingBox.enterVertex(flange_1->pos_bot_3);
    boundingBox.enterVertex(flange_1->pos_bot_4);
    boundingBox.enterVertex(flange_1->pos_top_1);
    boundingBox.enterVertex(flange_1->pos_top_2);
    boundingBox.enterVertex(flange_1->pos_top_3);
    boundingBox.enterVertex(flange_1->pos_top_4);

    boundingBox.enterVertex(flange_2->pos_bot_1);
    boundingBox.enterVertex(flange_2->pos_bot_2);
    boundingBox.enterVertex(flange_2->pos_bot_3);
    boundingBox.enterVertex(flange_2->pos_bot_4);
    boundingBox.enterVertex(flange_2->pos_top_1);
    boundingBox.enterVertex(flange_2->pos_top_2);
    boundingBox.enterVertex(flange_2->pos_top_3);
    boundingBox.enterVertex(flange_2->pos_top_4);


    boundingBox.enterVertex(flange_3->pos_bot_1);
    boundingBox.enterVertex(flange_3->pos_bot_2);
    boundingBox.enterVertex(flange_3->pos_bot_3);
    boundingBox.enterVertex(flange_3->pos_bot_4);
    boundingBox.enterVertex(flange_3->pos_top_1);
    boundingBox.enterVertex(flange_3->pos_top_2);
    boundingBox.enterVertex(flange_3->pos_top_3);
    boundingBox.enterVertex(flange_3->pos_top_4);

    this->snap_vertices.append(endcap_1->pos_bot_2);
    this->snap_vertices.append(endcap_1->pos_bot_3);
    this->snap_vertices.append(endcap_1->pos_top_2);
    this->snap_vertices.append(endcap_1->pos_top_3);
    this->snap_vertices.append(endcap_2->pos_bot_1);
    this->snap_vertices.append(endcap_2->pos_bot_4);
    this->snap_vertices.append(endcap_2->pos_top_1);
    this->snap_vertices.append(endcap_2->pos_top_4);
    this->snap_vertices.append(endcap_3->pos_bot_1);
    this->snap_vertices.append(endcap_3->pos_bot_4);
    this->snap_vertices.append(endcap_3->pos_top_1);
    this->snap_vertices.append(endcap_3->pos_top_4);

    QVector3D vertices[] = {
        splitPoint[0], splitPoint[1],
        endcap_1->pos_bot_1, endcap_1->pos_top_1, endcap_1->pos_bot_4, endcap_1->pos_top_4,
        endcap_2->pos_bot_3, endcap_2->pos_top_3, endcap_2->pos_bot_2, endcap_2->pos_top_2,
        endcap_3->pos_bot_3, endcap_3->pos_top_3, endcap_3->pos_bot_2, endcap_3->pos_top_2,
        splitPoint[2], splitPoint[3],
        endcap_1->inner_pos_bot_1, endcap_1->inner_pos_top_1, endcap_1->inner_pos_bot_4, endcap_1->inner_pos_top_4,
        endcap_2->inner_pos_bot_3, endcap_2->inner_pos_top_3, endcap_2->inner_pos_bot_2, endcap_2->inner_pos_top_2,
        endcap_3->inner_pos_bot_3, endcap_3->inner_pos_top_3, endcap_3->inner_pos_bot_2, endcap_3->inner_pos_top_2,
    };

    static GLushort indicesFaces[] = {
        //facing out
        //positive y side
        2, 12, 3, 13, 0xABCD,
        //middle on positive y side
        10, 0, 11, 1, 0xABCD,
        //middle on negative y side
        0, 8,1,9,   0xABCD,
        //negative y side
        6, 4, 7, 5, 0xABCD,
        //bottom
        12,2, 10,  0, 0xABCD,
        4, 6, 0, 8, 0xABCD,
        4, 0, 2, 0xABCD,
        //top
        3, 13, 1, 11, 0xABCD,
        5,1, 7, 9,  0xABCD,
        3, 1, 5, 0xABCD,

        //facing in
        //positive y side
        26, 16, 27, 17, 0xABCD,
        //middle on positive y side
        14, 24, 15, 25, 0xABCD,
        //middle on negative y side
        22, 14, 23, 15, 0xABCD,
        //negative y side
        18, 20, 19, 21, 0xABCD,
        //bottom
        16, 26, 14, 24, 0xABCD,
        20, 18, 22, 14, 0xABCD,
        14, 18, 16, 0xABCD,
        //top
        27, 17, 25, 15, 0xABCD,
        15, 19, 23, 21, 0xABCD,
        15, 17, 19, 0xABCD
    };

    static GLushort indicesLines[] = {
        //outer
        2, 12,
        10, 0,
        0, 8,
        6, 4,
        3, 13,
        11, 1,
        1, 9,
        7, 5,
        0, 1,
        //inner
        16, 26,
        24, 14,
        14, 22,
        20, 18,
        17, 27,
        25, 15,
        15, 23,
        21, 19,
        14, 15
    };

    arrayBufVertices->bind();
    arrayBufVertices->allocate(vertices, sizeof(vertices));

    indexBufFaces->bind();
    indexBufFaces->allocate(indicesFaces, sizeof(indicesFaces));

    indexBufLines->bind();
    indexBufLines->allocate(indicesLines, sizeof(indicesLines));

}

void CAD_air_ductYpiece::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a  = wizardParams.value("a").toDouble();
    b  = wizardParams.value("b").toDouble();
    a2  = wizardParams.value("a2").toDouble();
    b2  = wizardParams.value("b2").toDouble();
    e = wizardParams.value("e").toDouble();
    f = wizardParams.value("f").toDouble();
    b3 = wizardParams.value("b3").toDouble();
    l = wizardParams.value("l").toDouble();
    m = wizardParams.value("m").toDouble();
    u = wizardParams.value("u").toDouble();
    s = wizardParams.value("s").toDouble();
    ff = wizardParams.value("ff").toDouble();
    fe = wizardParams.value("fe").toDouble();


}

void CAD_air_ductYpiece::paint(GLWidget *glwidget)
{
    QColor color_pen_tmp = getColorPen();
    QColor color_brush_tmp = getColorBrush();

    glwidget->glEnable(GL_PRIMITIVE_RESTART);
    glwidget->glPrimitiveRestartIndex(0xABCD);

    arrayBufVertices->bind();
    glwidget->shaderProgram->enableAttributeArray(glwidget->shader_vertexLocation);
    glwidget->shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

    if (glwidget->render_solid)
    {
        glwidget->setPaintingColor(color_brush_tmp);

        indexBufFaces->bind();
        glwidget->glDrawElements(GL_TRIANGLE_STRIP, indexBufFaces->size(), GL_UNSIGNED_SHORT, 0);

        indexBufFaces->release();
    }

    if (glwidget->render_outline)
    {
        glwidget->setPaintingColor(color_pen_tmp);

        indexBufLines->bind();
        glwidget->glDrawElements(GL_LINES, indexBufLines->size(), GL_UNSIGNED_SHORT, 0);
        indexBufLines->release();
    }

    arrayBufVertices->release();
}

QMatrix4x4 CAD_air_ductYpiece::rotationOfFlange(quint8 num)
{
    if(num == 1)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(180.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else if(num == 2 || num == 3)
    {
        return matrix_rotation;
    }
    else
        return matrix_rotation;
}
