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

#include "cad_air_ductteeconnector.h"
#include "itemdb.h"
#include "glwidget.h"

CAD_air_ductTeeConnector::CAD_air_ductTeeConnector() : CADitem(CADitemTypes::Air_DuctTeeConnector)
{
    endcap_1 = new CAD_basic_duct();
    endcap_2 = new CAD_basic_duct();
    endcap_3 = new CAD_basic_duct();
    flange_1 = new CAD_basic_duct();
    flange_2 = new CAD_basic_duct();
    flange_3 = new CAD_basic_duct();

    this->subItems.append(endcap_1);
    this->subItems.append(endcap_2);
    this->subItems.append(endcap_3);
    this->subItems.append(flange_1);
    this->subItems.append(flange_2);
    this->subItems.append(flange_3);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",  200.0);
    wizardParams.insert("b",  300.0);
    wizardParams.insert("b2", 300.0);
    wizardParams.insert("b3", 500.0);
    wizardParams.insert("l", 2600.0);

    wizardParams.insert("r1", 500.0);
    wizardParams.insert("r2", 500.0);

    wizardParams.insert("m", 1000.0);
    wizardParams.insert("n", 1100.0);

    wizardParams.insert("e",   0.0);
    wizardParams.insert("u",  500.0);
    wizardParams.insert("fe",  10.0);
    wizardParams.insert("ff",  10.0);
    wizardParams.insert("s",   10.0);

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

CAD_air_ductTeeConnector::~CAD_air_ductTeeConnector()
{

}

QList<CADitemTypes::ItemType> CAD_air_ductTeeConnector::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Air_Duct);
    flangable_items.append(CADitemTypes::Air_DuctEndPlate);
    flangable_items.append(CADitemTypes::Air_DuctFireDamper);
    flangable_items.append(CADitemTypes::Air_DuctTeeConnector);
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

QImage CAD_air_ductTeeConnector::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_ductTeeConnector::iconPath()
{
    return ":/icons/cad_air/cad_air_ductteeconnector.svg";
}

QString CAD_air_ductTeeConnector::domain()
{
    return "Air";
}

QString CAD_air_ductTeeConnector::description()
{
    return "Air|Duct T-Connector";
}

void CAD_air_ductTeeConnector::calculate()
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

    endcap_1->wizardParams.insert("Position x", position.x());
    endcap_1->wizardParams.insert("Position y", position.y());
    endcap_1->wizardParams.insert("Position z", position.z());
    endcap_1->wizardParams.insert("Angle x", angle_x);
    endcap_1->wizardParams.insert("Angle y", angle_y);
    endcap_1->wizardParams.insert("Angle z", angle_z);
    endcap_1->wizardParams.insert("l", u);
    endcap_1->wizardParams.insert("b", b);
    endcap_1->wizardParams.insert("a", a);
    endcap_1->wizardParams.insert("s", s);
    endcap_1->processWizardInput();
    endcap_1->calculate();

    flange_1->wizardParams.insert("Position x", position.x());
    flange_1->wizardParams.insert("Position y", position.y());
    flange_1->wizardParams.insert("Position z", position.z());
    flange_1->wizardParams.insert("Angle x", angle_x);
    flange_1->wizardParams.insert("Angle y", angle_y);
    flange_1->wizardParams.insert("Angle z", angle_z);
    flange_1->wizardParams.insert("l", fe);
    flange_1->wizardParams.insert("b", b + 2 * ff);
    flange_1->wizardParams.insert("a", a + 2 * ff);
    flange_1->wizardParams.insert("s", ff);
    flange_1->processWizardInput();
    flange_1->calculate();

    QVector3D position_e2 = position + matrix_rotation * QVector3D(l, b/2 - e - b2/2, 0);
    endcap_2->wizardParams.insert("Position x", position_e2.x());
    endcap_2->wizardParams.insert("Position y", position_e2.y());
    endcap_2->wizardParams.insert("Position z", position_e2.z());
    endcap_2->wizardParams.insert("Angle x", angle_x);
    endcap_2->wizardParams.insert("Angle y", angle_y);
    endcap_2->wizardParams.insert("Angle z", angle_z+180);
    endcap_2->wizardParams.insert("l", u);
    endcap_2->wizardParams.insert("b", b2);
    endcap_2->wizardParams.insert("a", a);
    endcap_2->wizardParams.insert("s", s);
    endcap_2->processWizardInput();
    endcap_2->calculate();

    flange_2->wizardParams.insert("Position x", position_e2.x());
    flange_2->wizardParams.insert("Position y", position_e2.y());
    flange_2->wizardParams.insert("Position z", position_e2.z());
    flange_2->wizardParams.insert("Angle x", angle_x);
    flange_2->wizardParams.insert("Angle y", angle_y);
    flange_2->wizardParams.insert("Angle z", angle_z+180);
    flange_2->wizardParams.insert("l", fe);
    flange_2->wizardParams.insert("b", b2 + 2 * ff);
    flange_2->wizardParams.insert("a", a + 2 * ff);
    flange_2->wizardParams.insert("s", ff);
    flange_2->processWizardInput();
    flange_2->calculate();

    QVector3D position_e3 = position + matrix_rotation * QVector3D(n + b3/2, b/2 -e - b2 - m, 0);
    endcap_3->wizardParams.insert("Position x", position_e3.x());
    endcap_3->wizardParams.insert("Position y", position_e3.y());
    endcap_3->wizardParams.insert("Position z", position_e3.z());
    endcap_3->wizardParams.insert("Angle x", angle_x);
    endcap_3->wizardParams.insert("Angle y", angle_y);
    endcap_3->wizardParams.insert("Angle z", angle_z+90);
    endcap_3->wizardParams.insert("l", u);
    endcap_3->wizardParams.insert("b", b3);
    endcap_3->wizardParams.insert("a", a);
    endcap_3->wizardParams.insert("s", s);
    endcap_3->processWizardInput();
    endcap_3->calculate();

    flange_3->wizardParams.insert("Position x", position_e3.x());
    flange_3->wizardParams.insert("Position y", position_e3.y());
    flange_3->wizardParams.insert("Position z", position_e3.z());
    flange_3->wizardParams.insert("Angle x", angle_x);
    flange_3->wizardParams.insert("Angle y", angle_y);
    flange_3->wizardParams.insert("Angle z", angle_z+90);
    flange_3->wizardParams.insert("l", fe);
    flange_3->wizardParams.insert("b", b3 + 2 * ff);
    flange_3->wizardParams.insert("a", a + 2 * ff);
    flange_3->wizardParams.insert("s", ff);
    flange_3->processWizardInput();
    flange_3->calculate();

    snap_flanges.append(position);
    snap_flanges.append(position_e2);
    snap_flanges.append(position_e3);


    QVector3D vertices[104];
    //outer
    vertices[0] = position + matrix_rotation * QVector3D(this->u, -b/2, -a/2);
    vertices[1] = position + matrix_rotation * QVector3D(this->u, -b/2, a/2);

    vertices[24] = position + matrix_rotation * QVector3D(n + b3, -b/2 - r1, -a/2);
    vertices[25]= position + matrix_rotation * QVector3D(n + b3, -b/2 - r1, a/2);

    int x = 2;
    int y = 26;
    QMatrix4x4 matrix_turn;
    for (int i = 0; i < 10; i++)
    {
        qreal angle_turn = 9.0 * i;

        matrix_turn.setToIdentity();
        matrix_turn.rotate(-angle_turn, 0.0, 0.0, 1.0);

        vertices[x] = position + matrix_rotation * (matrix_turn * QVector3D(0.0, r1, -a/2) + QVector3D(n-r1,-b/2 -r1, 0.0));
        vertices[y] = position + matrix_rotation * (matrix_turn * QVector3D(-r2, 0.0, -a/2) + QVector3D(n+b3+r2, b/2 +e-b2-r2, 0.0));
        y++;
        x++;
        vertices[x] = position + matrix_rotation * (matrix_turn * QVector3D(0.0, r1, a/2) + QVector3D(n-r1,-b/2 -r1, 0.0));
        vertices[y] = position + matrix_rotation * (matrix_turn * QVector3D(-r2, 0.0, a/2) + QVector3D(n+b3+r2, b/2 +e-b2-r2, 0.0));
        x++;
        y++;
    }
    vertices[22] = position + matrix_rotation * QVector3D(n, -b/2 - r1, -a/2);
    vertices[23] = position + matrix_rotation * QVector3D(n, -b/2 - r1, a/2);
    vertices[46] = position + matrix_rotation * QVector3D(n + b3 + r2, b/2 - e -b2, -a/2);
    vertices[47] = position + matrix_rotation * QVector3D(n + b3 + r2, b/2 - e -b2, a/2);


    vertices[48] = position + matrix_rotation * QVector3D(u, b/2, a/2);
    vertices[49] = position + matrix_rotation * QVector3D(l - u, b/2 - e, a/2);
    vertices[50] = position + matrix_rotation * QVector3D(l - u, b/2 - e, -a/2);
    vertices[51] = position + matrix_rotation * QVector3D(u, b/2, -a/2);

    //inner
    vertices[52] = position + matrix_rotation * QVector3D(this->u, -b/2 + s, -a/2 + s);
    vertices[53] = position + matrix_rotation * QVector3D(this->u, -b/2 + s, a/2 - s);

    vertices[76] = position + matrix_rotation * QVector3D(n + b3 - s, -b/2 - r1, -a/2 + s);
    vertices[77]= position + matrix_rotation * QVector3D(n + b3 - s, -b/2 - r1, a/2 - s);

    x = 54;
    y = 78;
    for (int i = 0; i < 10; i++)
    {
        qreal angle_turn = 9.0 * i;

        matrix_turn.setToIdentity();
        matrix_turn.rotate(-angle_turn, 0.0, 0.0, 1.0);

        vertices[x] = position + matrix_rotation * (matrix_turn * QVector3D(0.0, r1 + s, -a/2 + s) + QVector3D(n-r1,-b/2 -r1, 0.0));
        vertices[y] = position + matrix_rotation * (matrix_turn * QVector3D(-r2 - s, 0.0, -a/2 + s) + QVector3D(n+b3+r2, b/2 +e-b2-r2, 0.0));
        y++;
        x++;
        vertices[x] = position + matrix_rotation * (matrix_turn * QVector3D(0.0, r1 + s, a/2 - s) + QVector3D(n-r1,-b/2 -r1, 0.0));
        vertices[y] = position + matrix_rotation * (matrix_turn * QVector3D(-r2 - s, 0.0, a/2 - s) + QVector3D(n+b3+r2, b/2 +e-b2-r2, 0.0));
        x++;
        y++;
    }
    vertices[74] = position + matrix_rotation * QVector3D(n + s, -b/2 - r1, -a/2 + s);
    vertices[75] = position + matrix_rotation * QVector3D(n + s, -b/2 - r1, a/2 - s);
    vertices[76] = position + matrix_rotation * QVector3D(n + b3 + r2, b/2 - e -b2 + s, -a/2 + s);
    vertices[77] = position + matrix_rotation * QVector3D(n + b3 + r2, b/2 - e -b2 + s, a/2 - s);


    vertices[100] = position + matrix_rotation * QVector3D(u, b/2 - s, a/2 - s);
    vertices[101] = position + matrix_rotation * QVector3D(l - u, b/2 - e - s, a/2 - s);
    vertices[102] = position + matrix_rotation * QVector3D(l - u, b/2 - e - s, -a/2 + s);
    vertices[103] = position + matrix_rotation * QVector3D(u, b/2 - s, -a/2 + s);


    static GLushort indicesFaces[] = {
        //outer
        //turn negative x
        1,0,3,2,5,4,7,6,9,8,11,10,13,12,15,14,17,16,19,18,21,20,23,22,0xABCD,
        //turn positive x
        25,24,27,26,29,28,31,30,33,32,35,34,37,36,39,38,41,40,43,42,45,44,47,46,0xABCD,
        //back side
        48,49,51,50,0xABCD,
        //bottom
        51,50,0,46,2,44,4,42,6,40,8,38,10,36,12,34,14,32,16,30,18,28,20,26,22,24,0xABCD,
        //top
        49,48,47,1,45,3,43,5,41,7,39,9,37,11,35,13,33,15,31,17,29,19,27,21,25,23,0xABCD,

        //inner
        //turn negeative x
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 0xABCD,
        //turn positive x
        78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 76, 77, 0xABCD,
        //back side
        101, 100, 102, 103, 0xABCD,
        //bottom
        102,103,76,52,96,54,94,56,92,58,90,60,88,62,86,64,84,66,82,68,80,70,78,72,74,0xABCD,
        //top
        100,101,53,77,55,97,57,95,59,93,61,91,63,89,65,87,67,85,69,83,71,81,73,79,75,0xABCD

    };

    static GLushort indicesLines[] = {
        //outer
        0,2,2,4,4,6,6,8,8,10,10,12,12,14,14,16,16,18,18,20,20,22,
        1,3,3,5,5,7,7,9,9,11,11,13,13,15,15,17,17,19,19,21,21,23,
        24,26,26,28,28,30,30,32,32,34,34,36,36,38,38,40,40,42,42,44,44,46,
        25,27,27,29,29,31,31,33,33,35,35,37,37,39,39,41,41,43,43,45,45,47,
        48,49,50,51,
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,
        23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
        //inner
        52, 54, 54, 56, 56, 58, 58, 60, 60, 62, 62, 64, 64, 66, 66, 68, 68, 70, 70, 72, 72, 74,
        53, 55, 55, 57, 57, 59, 59, 61, 61, 63, 63, 65, 65, 67, 67 ,69, 69, 71, 71, 73, 73, 75,
        78, 80, 80, 82, 82, 84, 84, 86, 86, 88, 88, 90, 90, 92, 92, 94, 94, 96, 96, 76,
        79, 81, 81, 83, 83, 85, 85, 87, 87, 89, 89, 91, 91, 93, 93, 95, 95, 97, 97, 77,
        100, 101, 102, 103,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,
        78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,

    };


    arrayBufVertices->bind();
    arrayBufVertices->allocate(vertices, sizeof(vertices));

    indexBufFaces->bind();
    indexBufFaces->allocate(indicesFaces, sizeof(indicesFaces));

    indexBufLines->bind();
    indexBufLines->allocate(indicesLines, sizeof(indicesLines));

    boundingBox.enterVertex(flange_1->pos_bot_1);
    boundingBox.enterVertex(flange_1->pos_top_1);
    boundingBox.enterVertex(flange_1->pos_bot_2);
    boundingBox.enterVertex(flange_1->pos_top_2);
    boundingBox.enterVertex(flange_1->pos_bot_3);
    boundingBox.enterVertex(flange_1->pos_top_3);
    boundingBox.enterVertex(flange_1->pos_bot_4);
    boundingBox.enterVertex(flange_1->pos_top_4);

    boundingBox.enterVertex(flange_2->pos_bot_1);
    boundingBox.enterVertex(flange_2->pos_top_1);
    boundingBox.enterVertex(flange_2->pos_bot_2);
    boundingBox.enterVertex(flange_2->pos_top_2);
    boundingBox.enterVertex(flange_2->pos_bot_3);
    boundingBox.enterVertex(flange_2->pos_top_3);
    boundingBox.enterVertex(flange_2->pos_bot_4);
    boundingBox.enterVertex(flange_2->pos_top_4);

    boundingBox.enterVertex(flange_3->pos_bot_1);
    boundingBox.enterVertex(flange_3->pos_top_1);
    boundingBox.enterVertex(flange_3->pos_bot_2);
    boundingBox.enterVertex(flange_3->pos_top_2);
    boundingBox.enterVertex(flange_3->pos_bot_3);
    boundingBox.enterVertex(flange_3->pos_top_3);
    boundingBox.enterVertex(flange_3->pos_bot_4);
    boundingBox.enterVertex(flange_3->pos_top_4);

    this->snap_vertices.append(endcap_1->pos_bot_2);
    this->snap_vertices.append(endcap_1->pos_bot_3);
    this->snap_vertices.append(endcap_1->pos_top_2);
    this->snap_vertices.append(endcap_1->pos_top_3);

    this->snap_vertices.append(endcap_2->pos_bot_2);
    this->snap_vertices.append(endcap_2->pos_bot_3);
    this->snap_vertices.append(endcap_2->pos_top_2);
    this->snap_vertices.append(endcap_2->pos_top_3);

    this->snap_vertices.append(endcap_3->pos_bot_2);
    this->snap_vertices.append(endcap_3->pos_bot_3);
    this->snap_vertices.append(endcap_3->pos_top_2);
    this->snap_vertices.append(endcap_3->pos_top_3);


}

void CAD_air_ductTeeConnector::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();


    this->a = wizardParams.value("a").toDouble();
    this->b = wizardParams.value("b").toDouble();
    this->b2 = wizardParams.value("b2").toDouble();
    this->e = wizardParams.value("e").toDouble();
    this->b3 = wizardParams.value("b3").toDouble();
    this->l = wizardParams.value("l").toDouble();
    this->m = wizardParams.value("m").toDouble();
    this->n = wizardParams.value("n").toDouble();
    this->r1 = wizardParams.value("r1").toDouble();
    this->r2 = wizardParams.value("r2").toDouble();
    this->u = wizardParams.value("u").toDouble();
    this->ff = wizardParams.value("ff").toDouble();
    this->fe = wizardParams.value("fe").toDouble();
    this->s = wizardParams.value("s").toDouble();

    if(fabs((u + r1 + b) - (e + b2 + m)) > 10E-8)
        qDebug() << "This item can not be drawn! (u + r1 + b) != (e + d + m)";

    if(fabs((n + b3 + r2 + u) - l) > 10E-8)
        qDebug() << "This item can not be drawn! (n + h + r2 + u) != l";

}

void CAD_air_ductTeeConnector::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_air_ductTeeConnector::rotationOfFlange(quint8 num)
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
    else if(num == 3)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(-90.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else
        return matrix_rotation;
}
