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

#include "cad_air_ducttransitionrectround.h"
#include "itemdb.h"
#include "glwidget.h"

CAD_air_ductTransitionRectRound::CAD_air_ductTransitionRectRound() : CADitem(CADitemTypes::Air_DuctTransitionRectRound)
{
    this->flange_rect = new CAD_basic_duct();
    this->flange_round =new CAD_basic_pipe();
    this->endcap_rect = new CAD_basic_duct();
    this->endcap_round =new CAD_basic_pipe();
    this->subItems.append(flange_rect);
    this->subItems.append(flange_round);
    this->subItems.append(endcap_rect);
    this->subItems.append(endcap_round);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("b",  300.0);
    wizardParams.insert("a",  200.0);
    wizardParams.insert("l", 1000.0);
    wizardParams.insert("d",  200.0);
    wizardParams.insert("e",   00.0);
    wizardParams.insert("f",   00.0);
    wizardParams.insert("u",   50.0);
    wizardParams.insert("fe",  10.0);
    wizardParams.insert("ff",  10.0);
    wizardParams.insert("s",   10.0);

    arrayBufVertices = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    arrayBufVertices.create();
    arrayBufVertices.setUsagePattern(QOpenGLBuffer::StaticDraw);

    indexBufFaces = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBufFaces.create();
    indexBufFaces.setUsagePattern(QOpenGLBuffer::StaticDraw);

    indexBufLines = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBufLines.create();
    indexBufLines.setUsagePattern(QOpenGLBuffer::StaticDraw);

    processWizardInput();
    calculate();
}

CAD_air_ductTransitionRectRound::~CAD_air_ductTransitionRectRound()
{

}

QList<CADitemTypes::ItemType> CAD_air_ductTransitionRectRound::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Air_Pipe);
    flangable_items.append(CADitemTypes::Air_PipeEndCap);
    flangable_items.append(CADitemTypes::Air_PipeFireDamper);
    flangable_items.append(CADitemTypes::Air_PipeReducer);
    flangable_items.append(CADitemTypes::Air_PipeSilencer);
    flangable_items.append(CADitemTypes::Air_PipeTeeConnector);
    flangable_items.append(CADitemTypes::Air_PipeTurn);
    flangable_items.append(CADitemTypes::Air_PipeVolumetricFlowController);
    return flangable_items;
}

QImage CAD_air_ductTransitionRectRound::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_ductTransitionRectRound::iconPath()
{
    return ":/icons/cad_air/cad_air_ducttransitionrectround.svg";
}

QString CAD_air_ductTransitionRectRound::domain()
{
    return "Air";
}

QString CAD_air_ductTransitionRectRound::description()
{
    return "Air|Duct transition rect/round";
}

void CAD_air_ductTransitionRectRound::calculate()
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
    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, b/2 + e - d/2, a/2 + f - d/2 ));

    QVector3D vertices[40];
    //inner <-> outer
    for(int j = 0; j < 2; j++)
    {
        int k = 0;
        //circle
        for(qreal step = 0; step <= 1; step += 0.0625) //16 vertices
        {
            QVector3D rad = QVector3D(0.0, d/2 - j * s, 0.0);
            QMatrix4x4 matrix_turn = QMatrix4x4();
            matrix_turn.setToIdentity();
            matrix_turn.rotate(step * 360, 1.0, 0.0, 0.0);
            vertices[16*j + k]= position + matrix_rotation * (matrix_turn * rad + QVector3D(l - u, b/2 + e - d/2, a/2 + f - d/2 ));
            boundingBox.enterVertex(vertices[16*j + k]);
            k++;

        }
    }
    //inner <-> outer
    for(int j = 0; j < 2; j++)
    {
        vertices[32 + 4*j] = position + matrix_rotation * QVector3D(u,  b/2 - j * s, -a/2 + j * s);
        vertices[32 + 4*j + 1] = position + matrix_rotation * QVector3D(u,  b/2 - j * s,  a/2 - j * s);
        vertices[32 + 4*j + 2] = position + matrix_rotation * QVector3D(u, -b/2 + j * s,  a/2 - j * s);
        vertices[32 + 4*j + 3] = position + matrix_rotation * QVector3D(u, -b/2 + j * s, -a/2 + j * s);
        boundingBox.enterVertex(vertices[32 + 4*j]);
        boundingBox.enterVertex(vertices[32 + 4*j + 1]);
        boundingBox.enterVertex(vertices[32 + 4*j + 2]);
        boundingBox.enterVertex(vertices[32 + 4*j + 3]);
    }
    static GLushort indicesFaces[160];

    //outer
    indicesFaces[0] = 0;
    indicesFaces[1] = 32;
    indicesFaces[2] = 33;
    indicesFaces[3] = 0xABCD;
    indicesFaces[4] = 4;
    indicesFaces[5] = 33;
    indicesFaces[6] = 34;
    indicesFaces[7] = 0xABCD;
    indicesFaces[8] = 8;
    indicesFaces[9] = 34;
    indicesFaces[10] = 35;
    indicesFaces[11] = 0xABCD;
    indicesFaces[12] = 12;
    indicesFaces[13] = 35;
    indicesFaces[14] = 32;
    indicesFaces[15] = 0xABCD;

    //inner
    indicesFaces[16] = 16;
    indicesFaces[17] = 36;
    indicesFaces[18] = 37;
    indicesFaces[19] = 0xABCD;
    indicesFaces[20] = 20;
    indicesFaces[21] = 37;
    indicesFaces[22] = 38;
    indicesFaces[23] = 0xABCD;
    indicesFaces[24] = 24;
    indicesFaces[25] = 38;
    indicesFaces[26] = 39;
    indicesFaces[27] = 0xABCD;
    indicesFaces[28] = 28;
    indicesFaces[29] = 39;
    indicesFaces[30] = 36;
    indicesFaces[31] = 0xABCD;

    //ounter
    for(int i = 0; i < 4; i++)
    {
        indicesFaces[32 + 4*i] = i;
        indicesFaces[33 + 4*i] = i+1;
        indicesFaces[34 + 4*i] = 33;
        indicesFaces[35 + 4*i] = 0xABCD;
    }
    for(int i = 0; i < 4; i++)
    {
        indicesFaces[48 + 4*i] = 4 + i;
        indicesFaces[49 + 4*i] = 5 + i;
        indicesFaces[50 + 4*i] = 34;
        indicesFaces[51 + 4*i] = 0xABCD;
    }
    for(int i = 0; i < 4; i++)
    {
        indicesFaces[64 + 4*i] = 8 + i;
        indicesFaces[65 + 4*i] = 9 + i;
        indicesFaces[66 + 4*i] = 35;
        indicesFaces[67 + 4*i] = 0xABCD;
    }
    for(int i = 0; i < 4; i++)
    {
        indicesFaces[80 + 4*i] = 12 + i;
        indicesFaces[81 + 4*i] = 13 + i;
        indicesFaces[82 + 4*i] = 32;
        indicesFaces[83 + 4*i] = 0xABCD;
    }

    //inner
    for(int i = 0; i < 4; i++)
    {
        indicesFaces[96 + 4*i] = 16 + i;
        indicesFaces[97 + 4*i] = 17 + i;
        indicesFaces[98 + 4*i] = 37;
        indicesFaces[99 + 4*i] = 0xABCD;
    }
    for(int i = 0; i < 4; i++)
    {
        indicesFaces[112 + 4*i] = 20 + i;
        indicesFaces[113 + 4*i] = 21 + i;
        indicesFaces[114 + 4*i] = 38;
        indicesFaces[115 + 4*i] = 0xABCD;
    }
    for(int i = 0; i < 4; i++)
    {
        indicesFaces[128 + 4*i] = 24 + i;
        indicesFaces[129 + 4*i] = 25 + i;
        indicesFaces[130 + 4*i] = 39;
        indicesFaces[131 + 4*i] = 0xABCD;
    }
    for(int i = 0; i < 4; i++)
    {
        indicesFaces[144 + 4*i] = 28 + i;
        indicesFaces[145 + 4*i] = 29 + i;
        indicesFaces[146 + 4*i] = 36;
        indicesFaces[147 + 4*i] = 0xABCD;
    }





    static GLushort indicesLines[32];
    //outer
    indicesLines[0] = 0;
    indicesLines[1] = 32;
    indicesLines[2] = 33;
    indicesLines[3] = 0;
    indicesLines[4] = 4;
    indicesLines[5] = 33;
    indicesLines[6] = 34;
    indicesLines[7] = 4;
    indicesLines[8] = 8;
    indicesLines[9] = 34;
    indicesLines[10] = 35;
    indicesLines[11] = 8;
    indicesLines[12] = 12;
    indicesLines[13] = 35;
    indicesLines[14] = 32;
    indicesLines[15] = 12;

    //inner
    indicesLines[16] = 16;
    indicesLines[17] = 36;
    indicesLines[18] = 37;
    indicesLines[19] = 16;
    indicesLines[20] = 20;
    indicesLines[21] = 37;
    indicesLines[22] = 38;
    indicesLines[23] = 20;
    indicesLines[24] = 24;
    indicesLines[25] = 38;
    indicesLines[26] = 39;
    indicesLines[27] = 24;
    indicesLines[28] = 28;
    indicesLines[29] = 39;
    indicesLines[30] = 36;
    indicesLines[31] = 28;





    arrayBufVertices.bind();
    arrayBufVertices.allocate(vertices, sizeof(vertices));

    indexBufFaces.bind();
    indexBufFaces.allocate(indicesFaces, sizeof(indicesFaces));

    indexBufLines.bind();
    indexBufLines.allocate(indicesLines, sizeof(indicesLines));

    //calculate flanges
    flange_rect->wizardParams.insert("Position x", (position.x()));
    flange_rect->wizardParams.insert("Position y", (position.y()));
    flange_rect->wizardParams.insert("Position z", (position.z()));
    flange_rect->wizardParams.insert("Angle x", (angle_x));
    flange_rect->wizardParams.insert("Angle y", (angle_y));
    flange_rect->wizardParams.insert("Angle z", (angle_z));
    flange_rect->wizardParams.insert("l", (fe));
    flange_rect->wizardParams.insert("b", (b + 2 * ff));
    flange_rect->wizardParams.insert("a", (a + 2 * ff));
    flange_rect->wizardParams.insert("s", (ff));
    flange_rect->layer = this->layer;
    flange_rect->processWizardInput();
    flange_rect->calculate();

    endcap_rect->wizardParams.insert("Position x", (position.x()));
    endcap_rect->wizardParams.insert("Position y", (position.y()));
    endcap_rect->wizardParams.insert("Position z", (position.z()));
    endcap_rect->wizardParams.insert("Angle x", (angle_x));
    endcap_rect->wizardParams.insert("Angle y", (angle_y));
    endcap_rect->wizardParams.insert("Angle z", (angle_z));
    endcap_rect->wizardParams.insert("l", (u));
    endcap_rect->wizardParams.insert("b", (b));
    endcap_rect->wizardParams.insert("a", (a));
    endcap_rect->wizardParams.insert("s", (s));
    endcap_rect->layer = this->layer;
    endcap_rect->processWizardInput();
    endcap_rect->calculate();


    QVector3D position_fr = position + matrix_rotation *  QVector3D(l, b/2 + e - d/2, a/2 + f - d/2 );
    flange_round->wizardParams.insert("Position x", (position_fr.x()));
    flange_round->wizardParams.insert("Position y", (position_fr.y()));
    flange_round->wizardParams.insert("Position z", (position_fr.z()));
    flange_round->wizardParams.insert("Angle x", (angle_x));
    flange_round->wizardParams.insert("Angle y", (angle_y));
    flange_round->wizardParams.insert("Angle z", (angle_z+180));
    flange_round->wizardParams.insert("l", (fe));
    flange_round->wizardParams.insert("d", (d + 2 * ff));
    flange_round->wizardParams.insert("s", (ff));
    flange_round->layer = this->layer;
    flange_round->processWizardInput();
    flange_round->calculate();

    endcap_round->wizardParams.insert("Position x", (position_fr.x()));
    endcap_round->wizardParams.insert("Position y", (position_fr.y()));
    endcap_round->wizardParams.insert("Position z", (position_fr.z()));
    endcap_round->wizardParams.insert("Angle x", (angle_x));
    endcap_round->wizardParams.insert("Angle y", (angle_y));
    endcap_round->wizardParams.insert("Angle z", (angle_z+180));
    endcap_round->wizardParams.insert("l", (u));
    endcap_round->wizardParams.insert("d", (d));
    endcap_round->wizardParams.insert("s", (s));
    endcap_round->layer = this->layer;
    endcap_round->processWizardInput();
    endcap_round->calculate();

    this->snap_vertices.append(rectside[0][0][0]);
    this->snap_vertices.append(rectside[0][0][1]);
    this->snap_vertices.append(rectside[0][0][2]);
    this->snap_vertices.append(rectside[0][0][3]);


    this->snap_vertices.append(roundside[0][0][0]);
    this->snap_vertices.append(roundside[0][0][7]);
    this->snap_vertices.append(roundside[0][0][15]);
    this->snap_vertices.append(roundside[0][0][23]);
}

void CAD_air_ductTransitionRectRound::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());

    l = wizardParams.value("l").toDouble();
    b = wizardParams.value("b").toDouble();
    a = wizardParams.value("a").toDouble();
    d = wizardParams.value("d").toDouble();

    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    s = wizardParams.value("s").toDouble();
    ff = wizardParams.value("ff").toDouble();
    fe = wizardParams.value("fe").toDouble();
    u = wizardParams.value("u").toDouble();
    e = wizardParams.value("e").toDouble();
    f = wizardParams.value("f").toDouble();
    d = wizardParams.value("d").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);

}

//void CAD_air_ductTransitionRectRound::paint(GLWidget *glwidget)
//{
//    QColor color_pen = getColorPen();
//    QColor color_brush = getColorBrush();

//    int count_k = 32;
//    if (glwidget->render_solid)
//    {
//        glwidget->setPaintingColor(color_brush);

//        //front face round
//        glwidget->glBegin(GL_QUADS);
//        for(int k = 0; k < count_k - 1; k++)
//        {
//            glwidget->glVertex3f((GLfloat)roundside[0][0][k].x(), (GLfloat)roundside[0][0][k].y(), (GLfloat)roundside[0][0][k].z());
//            glwidget->glVertex3f((GLfloat)roundside[0][1][k].x(), (GLfloat)roundside[0][1][k].y(), (GLfloat)roundside[0][1][k].z());
//            glwidget->glVertex3f((GLfloat)roundside[0][1][k+1].x(), (GLfloat)roundside[0][1][k+1].y(), (GLfloat)roundside[0][1][k+1].z());
//            glwidget->glVertex3f((GLfloat)roundside[0][0][k+1].x(), (GLfloat)roundside[0][0][k+1].y(), (GLfloat)roundside[0][0][k+1].z());
//        }
//        glwidget->glVertex3f((GLfloat)roundside[0][0][count_k - 1].x(), (GLfloat)roundside[0][0][count_k - 1].y(), (GLfloat)roundside[0][0][count_k - 1].z());
//        glwidget->glVertex3f((GLfloat)roundside[0][1][count_k - 1].x(), (GLfloat)roundside[0][1][count_k - 1].y(), (GLfloat)roundside[0][1][count_k - 1].z());
//        glwidget->glVertex3f((GLfloat)roundside[0][1][0].x(), (GLfloat)roundside[0][1][0].y(), (GLfloat)roundside[0][1][0].z());
//        glwidget->glVertex3f((GLfloat)roundside[0][0][0].x(), (GLfloat)roundside[0][0][0].y(), (GLfloat)roundside[0][0][0].z());

//        //outer faces round
//        for(int k = 0; k < count_k - 1; k++)
//        {
//            glwidget->glVertex3f((GLfloat)roundside[0][0][k].x(), (GLfloat)roundside[0][0][k].y(), (GLfloat)roundside[0][0][k].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(), (GLfloat)roundside[1][0][k].y(), (GLfloat)roundside[1][0][k].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k+1].x(), (GLfloat)roundside[1][0][k+1].y(), (GLfloat)roundside[1][0][k+1].z());
//            glwidget->glVertex3f((GLfloat)roundside[0][0][k+1].x(), (GLfloat)roundside[0][0][k+1].y(), (GLfloat)roundside[0][0][k+1].z());
//        }
//        glwidget->glVertex3f((GLfloat)roundside[0][0][count_k - 1].x(), (GLfloat)roundside[0][0][count_k - 1].y(), (GLfloat)roundside[0][0][count_k - 1].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][count_k - 1].x(), (GLfloat)roundside[1][0][count_k - 1].y(), (GLfloat)roundside[1][0][count_k - 1].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][0].x(), (GLfloat)roundside[1][0][0].y(), (GLfloat)roundside[1][0][0].z());
//        glwidget->glVertex3f((GLfloat)roundside[0][0][0].x(), (GLfloat)roundside[0][0][0].y(), (GLfloat)roundside[0][0][0].z());


//        //inner faces round
//        for(int k = 0; k < count_k - 1; k++)
//        {
//            glwidget->glVertex3f((GLfloat)roundside[0][1][k].x(), (GLfloat)roundside[0][1][k].y(), (GLfloat)roundside[0][1][k].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][1][k].x(), (GLfloat)roundside[1][1][k].y(), (GLfloat)roundside[1][1][k].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][1][k+1].x(), (GLfloat)roundside[1][1][k+1].y(), (GLfloat)roundside[1][1][k+1].z());
//            glwidget->glVertex3f((GLfloat)roundside[0][1][k+1].x(), (GLfloat)roundside[0][1][k+1].y(), (GLfloat)roundside[0][1][k+1].z());
//        }
//        glwidget->glVertex3f((GLfloat)roundside[0][1][count_k - 1].x(), (GLfloat)roundside[0][1][count_k - 1].y(), (GLfloat)roundside[0][1][count_k - 1].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][1][count_k - 1].x(), (GLfloat)roundside[1][1][count_k - 1].y(), (GLfloat)roundside[1][1][count_k - 1].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][1][0].x(), (GLfloat)roundside[1][1][0].y(), (GLfloat)roundside[1][1][0].z());
//        glwidget->glVertex3f((GLfloat)roundside[0][1][0].x(), (GLfloat)roundside[0][1][0].y(), (GLfloat)roundside[0][1][0].z());


//        //inner faces rect
//        glwidget->glBegin(GL_QUADS);
//        glwidget->glVertex3f((GLfloat)rectside[0][1][0].x(), (GLfloat)rectside[0][1][0].y(), (GLfloat)rectside[0][1][0].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][1][0].x(), (GLfloat)rectside[1][1][0].y(), (GLfloat)rectside[1][1][0].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][1][1].x(), (GLfloat)rectside[1][1][1].y(), (GLfloat)rectside[1][1][1].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][1].x(), (GLfloat)rectside[0][1][1].y(), (GLfloat)rectside[0][1][1].z());

//        glwidget->glVertex3f((GLfloat)rectside[0][1][1].x(), (GLfloat)rectside[0][1][1].y(), (GLfloat)rectside[0][1][1].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][1][1].x(), (GLfloat)rectside[1][1][1].y(), (GLfloat)rectside[1][1][1].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][1][2].x(), (GLfloat)rectside[1][1][2].y(), (GLfloat)rectside[1][1][2].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][2].x(), (GLfloat)rectside[0][1][2].y(), (GLfloat)rectside[0][1][2].z());

//        glwidget->glVertex3f((GLfloat)rectside[0][1][2].x(), (GLfloat)rectside[0][1][2].y(), (GLfloat)rectside[0][1][2].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][1][2].x(), (GLfloat)rectside[1][1][2].y(), (GLfloat)rectside[1][1][2].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][1][3].x(), (GLfloat)rectside[1][1][3].y(), (GLfloat)rectside[1][1][3].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][3].x(), (GLfloat)rectside[0][1][3].y(), (GLfloat)rectside[0][1][3].z());

//        glwidget->glVertex3f((GLfloat)rectside[0][1][3].x(), (GLfloat)rectside[0][1][3].y(), (GLfloat)rectside[0][1][3].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][1][3].x(), (GLfloat)rectside[1][1][3].y(), (GLfloat)rectside[1][1][3].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][1][0].x(), (GLfloat)rectside[1][1][0].y(), (GLfloat)rectside[1][1][0].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][0].x(), (GLfloat)rectside[0][1][0].y(), (GLfloat)rectside[0][1][0].z());

//        //outer faces rect
//        glwidget->glVertex3f((GLfloat)rectside[0][0][0].x(), (GLfloat)rectside[0][0][0].y(), (GLfloat)rectside[0][0][0].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][0].x(), (GLfloat)rectside[1][0][0].y(), (GLfloat)rectside[1][0][0].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][1].x(), (GLfloat)rectside[1][0][1].y(), (GLfloat)rectside[1][0][1].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][0][1].x(), (GLfloat)rectside[0][0][1].y(), (GLfloat)rectside[0][0][1].z());

//        glwidget->glVertex3f((GLfloat)rectside[0][0][1].x(), (GLfloat)rectside[0][0][1].y(), (GLfloat)rectside[0][0][1].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][1].x(), (GLfloat)rectside[1][0][1].y(), (GLfloat)rectside[1][0][1].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][2].x(), (GLfloat)rectside[1][0][2].y(), (GLfloat)rectside[1][0][2].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][0][2].x(), (GLfloat)rectside[0][0][2].y(), (GLfloat)rectside[0][0][2].z());

//        glwidget->glVertex3f((GLfloat)rectside[0][0][2].x(), (GLfloat)rectside[0][0][2].y(), (GLfloat)rectside[0][0][2].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][2].x(), (GLfloat)rectside[1][0][2].y(), (GLfloat)rectside[1][0][2].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][3].x(), (GLfloat)rectside[1][0][3].y(), (GLfloat)rectside[1][0][3].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][0][3].x(), (GLfloat)rectside[0][0][3].y(), (GLfloat)rectside[0][0][3].z());

//        glwidget->glVertex3f((GLfloat)rectside[0][0][3].x(), (GLfloat)rectside[0][0][3].y(), (GLfloat)rectside[0][0][3].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][3].x(), (GLfloat)rectside[1][0][3].y(), (GLfloat)rectside[1][0][3].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][0].x(), (GLfloat)rectside[1][0][0].y(), (GLfloat)rectside[1][0][0].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][0][0].x(), (GLfloat)rectside[0][0][0].y(), (GLfloat)rectside[0][0][0].z());

//        //front faces rect
//        glwidget->glVertex3f((GLfloat)rectside[0][0][0].x(), (GLfloat)rectside[0][0][0].y(), (GLfloat)rectside[0][0][0].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][0].x(), (GLfloat)rectside[0][1][0].y(), (GLfloat)rectside[0][1][0].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][1].x(), (GLfloat)rectside[0][1][1].y(), (GLfloat)rectside[0][1][1].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][0][1].x(), (GLfloat)rectside[0][0][1].y(), (GLfloat)rectside[0][0][1].z());

//        glwidget->glVertex3f((GLfloat)rectside[0][0][1].x(), (GLfloat)rectside[0][0][1].y(), (GLfloat)rectside[0][0][1].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][1].x(), (GLfloat)rectside[0][1][1].y(), (GLfloat)rectside[0][1][1].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][2].x(), (GLfloat)rectside[0][1][2].y(), (GLfloat)rectside[0][1][2].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][0][2].x(), (GLfloat)rectside[0][0][2].y(), (GLfloat)rectside[0][0][2].z());

//        glwidget->glVertex3f((GLfloat)rectside[0][0][2].x(), (GLfloat)rectside[0][0][2].y(), (GLfloat)rectside[0][0][2].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][2].x(), (GLfloat)rectside[0][1][2].y(), (GLfloat)rectside[0][1][2].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][3].x(), (GLfloat)rectside[0][1][3].y(), (GLfloat)rectside[0][1][3].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][0][3].x(), (GLfloat)rectside[0][0][3].y(), (GLfloat)rectside[0][0][3].z());

//        glwidget->glVertex3f((GLfloat)rectside[0][0][3].x(), (GLfloat)rectside[0][0][3].y(), (GLfloat)rectside[0][0][3].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][3].x(), (GLfloat)rectside[0][1][3].y(), (GLfloat)rectside[0][1][3].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][1][0].x(), (GLfloat)rectside[0][1][0].y(), (GLfloat)rectside[0][1][0].z());
//        glwidget->glVertex3f((GLfloat)rectside[0][0][0].x(), (GLfloat)rectside[0][0][0].y(), (GLfloat)rectside[0][0][0].z());
//        glwidget->glEnd();

//        //triangle rect-round-transition
//        glwidget->glBegin(GL_TRIANGLE_FAN);
//        glwidget->glVertex3f((GLfloat)rectside[1][0][1].x(),   (GLfloat)rectside[1][0][1].y(),   (GLfloat)rectside[1][0][1].z());
//        for(int k = 0; k < 8; k++)
//        {
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());

//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_TRIANGLE_FAN);
//        glwidget->glVertex3f((GLfloat)rectside[1][0][2].x(),   (GLfloat)rectside[1][0][2].y(),   (GLfloat)rectside[1][0][2].z());
//        for(int k = 8; k < 16; k++)
//        {
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());

//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_TRIANGLE_FAN);
//        glwidget->glVertex3f((GLfloat)rectside[1][0][3].x(),   (GLfloat)rectside[1][0][3].y(),   (GLfloat)rectside[1][0][3].z());
//        for(int k = 16; k < 24; k++)
//        {
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());
//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_TRIANGLE_FAN);
//        glwidget->glVertex3f((GLfloat)rectside[1][0][0].x(),   (GLfloat)rectside[1][0][0].y(),   (GLfloat)rectside[1][0][0].z());
//        for(int k = 24; k < 32; k++)
//        {
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());
//        }
//        glwidget->glEnd();

//        glwidget->glBegin(GL_QUADS);

//        glwidget->glVertex3f((GLfloat)rectside[1][0][0].x(),   (GLfloat)rectside[1][0][0].y(),   (GLfloat)rectside[1][0][0].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][1].x(),   (GLfloat)rectside[1][0][1].y(),   (GLfloat)rectside[1][0][1].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][0].x(),   (GLfloat)roundside[1][0][0].y(),   (GLfloat)roundside[1][0][0].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][31].x(),   (GLfloat)roundside[1][0][31].y(),   (GLfloat)roundside[1][0][31].z());

//        glwidget->glVertex3f((GLfloat)rectside[1][0][1].x(),   (GLfloat)rectside[1][0][1].y(),   (GLfloat)rectside[1][0][1].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][2].x(),   (GLfloat)rectside[1][0][2].y(),   (GLfloat)rectside[1][0][2].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][8].x(),   (GLfloat)roundside[1][0][8].y(),   (GLfloat)roundside[1][0][8].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][7].x(),   (GLfloat)roundside[1][0][7].y(),   (GLfloat)roundside[1][0][7].z());

//        glwidget->glVertex3f((GLfloat)rectside[1][0][2].x(),   (GLfloat)rectside[1][0][2].y(),   (GLfloat)rectside[1][0][2].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][3].x(),   (GLfloat)rectside[1][0][3].y(),   (GLfloat)rectside[1][0][3].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][16].x(),   (GLfloat)roundside[1][0][16].y(),   (GLfloat)roundside[1][0][16].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][15].x(),   (GLfloat)roundside[1][0][15].y(),   (GLfloat)roundside[1][0][15].z());

//        glwidget->glVertex3f((GLfloat)rectside[1][0][3].x(),   (GLfloat)rectside[1][0][3].y(),   (GLfloat)rectside[1][0][3].z());
//        glwidget->glVertex3f((GLfloat)rectside[1][0][0].x(),   (GLfloat)rectside[1][0][0].y(),   (GLfloat)rectside[1][0][0].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][24].x(),   (GLfloat)roundside[1][0][24].y(),   (GLfloat)roundside[1][0][24].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][23].x(),   (GLfloat)roundside[1][0][23].y(),   (GLfloat)roundside[1][0][23].z());

//        glwidget->glEnd();
//    }

//    if (glwidget->render_outline)
//    {
//        glwidget->setPaintingColor(color_pen);
//        glwidget->glLineWidth(1.0);

//        //        //front face round
//        //        for(int k = 0; k < count_k - 1; k++)
//        //        {
//        //            glBegin(GL_LINE_LOOP);
//        //            glVertex3f((GLfloat)roundside[0][0][k].x(), (GLfloat)roundside[0][0][k].y(), (GLfloat)roundside[0][0][k].z());
//        //            glVertex3f((GLfloat)roundside[0][1][k].x(), (GLfloat)roundside[0][1][k].y(), (GLfloat)roundside[0][1][k].z());
//        //            glVertex3f((GLfloat)roundside[0][1][k+1].x(), (GLfloat)roundside[0][1][k+1].y(), (GLfloat)roundside[0][1][k+1].z());
//        //            glVertex3f((GLfloat)roundside[0][0][k+1].x(), (GLfloat)roundside[0][0][k+1].y(), (GLfloat)roundside[0][0][k+1].z());

//        //        }
//        //        glVertex3f((GLfloat)roundside[0][0][count_k - 1].x(), (GLfloat)roundside[0][0][count_k - 1].y(), (GLfloat)roundside[0][0][count_k - 1].z());
//        //        glVertex3f((GLfloat)roundside[0][1][count_k - 1].x(), (GLfloat)roundside[0][1][count_k - 1].y(), (GLfloat)roundside[0][1][count_k - 1].z());
//        //        glVertex3f((GLfloat)roundside[0][1][0].x(), (GLfloat)roundside[0][1][0].y(), (GLfloat)roundside[0][1][0].z());
//        //        glVertex3f((GLfloat)roundside[0][0][0].x(), (GLfloat)roundside[0][0][0].y(), (GLfloat)roundside[0][0][0].z());
//        //        glEnd();

//        //outer faces round
//        for(int k = 0; k < count_k - 1; k++)
//        {
//            glwidget->glBegin(GL_LINE_LOOP);
//            glwidget->glVertex3f((GLfloat)roundside[0][0][k].x(), (GLfloat)roundside[0][0][k].y(), (GLfloat)roundside[0][0][k].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(), (GLfloat)roundside[1][0][k].y(), (GLfloat)roundside[1][0][k].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k+1].x(), (GLfloat)roundside[1][0][k+1].y(), (GLfloat)roundside[1][0][k+1].z());
//            glwidget->glVertex3f((GLfloat)roundside[0][0][k+1].x(), (GLfloat)roundside[0][0][k+1].y(), (GLfloat)roundside[0][0][k+1].z());
//            glwidget->glEnd();
//        }
//        glwidget->glBegin(GL_LINE_LOOP);
//        glwidget->glVertex3f((GLfloat)roundside[0][0][count_k - 1].x(), (GLfloat)roundside[0][0][count_k - 1].y(), (GLfloat)roundside[0][0][count_k - 1].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][count_k - 1].x(), (GLfloat)roundside[1][0][count_k - 1].y(), (GLfloat)roundside[1][0][count_k - 1].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][0][0].x(), (GLfloat)roundside[1][0][0].y(), (GLfloat)roundside[1][0][0].z());
//        glwidget->glVertex3f((GLfloat)roundside[0][0][0].x(), (GLfloat)roundside[0][0][0].y(), (GLfloat)roundside[0][0][0].z());
//        glwidget->glEnd();

//        //inner faces round
//        for(int k = 0; k < count_k - 1; k++)
//        {
//            glwidget->glBegin(GL_LINE_LOOP);
//            glwidget->glVertex3f((GLfloat)roundside[0][1][k].x(), (GLfloat)roundside[0][1][k].y(), (GLfloat)roundside[0][1][k].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][1][k].x(), (GLfloat)roundside[1][1][k].y(), (GLfloat)roundside[1][1][k].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][1][k+1].x(), (GLfloat)roundside[1][1][k+1].y(), (GLfloat)roundside[1][1][k+1].z());
//            glwidget->glVertex3f((GLfloat)roundside[0][1][k+1].x(), (GLfloat)roundside[0][1][k+1].y(), (GLfloat)roundside[0][1][k+1].z());
//            glwidget->glEnd();
//        }
//        glwidget->glBegin(GL_LINE_LOOP);
//        glwidget->glVertex3f((GLfloat)roundside[0][1][count_k - 1].x(), (GLfloat)roundside[0][1][count_k - 1].y(), (GLfloat)roundside[0][1][count_k - 1].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][1][count_k - 1].x(), (GLfloat)roundside[1][1][count_k - 1].y(), (GLfloat)roundside[1][1][count_k - 1].z());
//        glwidget->glVertex3f((GLfloat)roundside[1][1][0].x(), (GLfloat)roundside[1][1][0].y(), (GLfloat)roundside[1][1][0].z());
//        glwidget->glVertex3f((GLfloat)roundside[0][1][0].x(), (GLfloat)roundside[0][1][0].y(), (GLfloat)roundside[0][1][0].z());
//        glwidget->glEnd();

//        //inner faces rect
//        glwidget->glBegin(GL_LINE_LOOP);
//        for(int k = 0; k < 4; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[1][0][k].x(),   (GLfloat)rectside[1][0][k].y(),   (GLfloat)rectside[1][0][k].z());
//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINE_LOOP);
//        for(int k = 0; k < 4; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[1][1][k].x(),   (GLfloat)rectside[1][1][k].y(),   (GLfloat)rectside[1][1][k].z());
//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINES);
//        for(int k = 0; k < 4; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[0][1][k].x(),   (GLfloat)rectside[0][1][k].y(),   (GLfloat)rectside[0][1][k].z());
//            glwidget->glVertex3f((GLfloat)rectside[1][1][k].x(),   (GLfloat)rectside[1][1][k].y(),   (GLfloat)rectside[1][1][k].z());
//        }
//        glwidget->glEnd();


//        //outer faces rect
//        glwidget->glBegin(GL_LINE_LOOP);
//        for(int k = 0; k < 4; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[0][0][k].x(),   (GLfloat)rectside[0][0][k].y(),   (GLfloat)rectside[0][0][k].z());
//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINE_LOOP);
//        for(int k = 0; k < 4; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[0][1][k].x(),   (GLfloat)rectside[0][1][k].y(),   (GLfloat)rectside[0][1][k].z());
//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINES);
//        for(int k = 0; k < 4; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[0][0][k].x(),   (GLfloat)rectside[0][0][k].y(),   (GLfloat)rectside[0][0][k].z());
//            glwidget->glVertex3f((GLfloat)rectside[1][0][k].x(),   (GLfloat)rectside[1][0][k].y(),   (GLfloat)rectside[1][0][k].z());
//        }
//        glwidget->glEnd();

//        //inner faces rect
//        glwidget->glBegin(GL_LINE_LOOP);
//        for(int k = 0; k < 4; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[1][0][k].x(),   (GLfloat)rectside[1][0][k].y(),   (GLfloat)rectside[1][0][k].z());
//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINE_LOOP);
//        for(int k = 0; k < 4; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[1][1][k].x(),   (GLfloat)rectside[1][1][k].y(),   (GLfloat)rectside[1][1][k].z());
//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINES);
//        for(int k = 0; k < 4; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[0][1][k].x(),   (GLfloat)rectside[0][1][k].y(),   (GLfloat)rectside[0][1][k].z());
//            glwidget->glVertex3f((GLfloat)rectside[1][1][k].x(),   (GLfloat)rectside[1][1][k].y(),   (GLfloat)rectside[1][1][k].z());
//        }
//        glwidget->glEnd();

//        //rect-round transition

//        glwidget->glBegin(GL_LINES);
//        for(int k = 0; k < 8; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[1][0][1].x(),   (GLfloat)rectside[1][0][1].y(),   (GLfloat)rectside[1][0][1].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());

//        }
//        glwidget->glEnd();
//        glwidget->glBegin(GL_LINES);
//        for(int k = 8; k < 16; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[1][0][2].x(),   (GLfloat)rectside[1][0][2].y(),   (GLfloat)rectside[1][0][2].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());

//        }
//        glwidget->glEnd();

//        glwidget->glBegin(GL_LINES);
//        for(int k = 16; k < 24; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[1][0][3].x(),   (GLfloat)rectside[1][0][3].y(),   (GLfloat)rectside[1][0][3].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());

//        }
//        glwidget->glEnd();

//        glwidget->glBegin(GL_LINES);
//        for(int k = 24; k < 32; k++)
//        {
//            glwidget->glVertex3f((GLfloat)rectside[1][0][0].x(),   (GLfloat)rectside[1][0][0].y(),   (GLfloat)rectside[1][0][0].z());
//            glwidget->glVertex3f((GLfloat)roundside[1][0][k].x(),   (GLfloat)roundside[1][0][k].y(),   (GLfloat)roundside[1][0][k].z());

//        }
//        glwidget->glEnd();
//    }
//}

void CAD_air_ductTransitionRectRound::paint(GLWidget *glwidget)
{
    QColor color_pen_tmp = getColorPen();
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

    if (glwidget->render_outline)
    {
        glwidget->setPaintingColor(color_pen_tmp);
        glwidget->glLineWidth(1.0);

        indexBufLines.bind();
        glwidget->glDrawElements(GL_LINES, indexBufLines.size(), GL_UNSIGNED_SHORT, 0);
        indexBufLines.release();
    }

    arrayBufVertices.release();
}

QMatrix4x4 CAD_air_ductTransitionRectRound::rotationOfFlange(quint8 num)
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
