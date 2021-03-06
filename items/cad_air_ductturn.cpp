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

#include "cad_air_ductturn.h"

#include "glwidget.h"
#include "logging.h"

CAD_air_ductTurn::CAD_air_ductTurn() : CADitem(CADitemTypes::Air_DuctTurn)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",     200.0);
    wizardParams.insert("b",     300.0);
    wizardParams.insert("b2",    300.0);
    wizardParams.insert("r",    1000.0);
    wizardParams.insert("alpha",  90.0);
    wizardParams.insert("l2",     50.0);
    wizardParams.insert("l1",     50.0);
    wizardParams.insert("fe",     10.0);
    wizardParams.insert("ff",     10.0);
    wizardParams.insert("s",       1.0);

    flange_left_duct = new CAD_basic_duct();
    flange_right_duct = new CAD_basic_duct();
    this->subItems.append(flange_left_duct);
    this->subItems.append(flange_right_duct);

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

CAD_air_ductTurn::~CAD_air_ductTurn()
{

}

QList<CADitemTypes::ItemType> CAD_air_ductTurn::flangable_items(int flangeIndex)
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

QImage CAD_air_ductTurn::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_ductTurn::iconPath()
{
    return ":/icons/cad_air/cad_air_ductturn.svg";
}

QString CAD_air_ductTurn::domain()
{
    return "Air";
}

QString CAD_air_ductTurn::description()
{
    return "Air|Duct turn";
}


void CAD_air_ductTurn::calculate()
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

    this->snap_basepoint = this->position;
    this->snap_flanges.append(this->position);

//    matrix_rotation.setToIdentity();
//    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
//    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
//    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);

    flange_left_duct->wizardParams.insert("Position x", (position.x()));
    flange_left_duct->wizardParams.insert("Position y", (position.y()));
    flange_left_duct->wizardParams.insert("Position z", (position.z()));
    flange_left_duct->wizardParams.insert("Angle x", (angle_x));
    flange_left_duct->wizardParams.insert("Angle y", (angle_y));
    flange_left_duct->wizardParams.insert("Angle z", (angle_z));
    flange_left_duct->wizardParams.insert("l", (fe));
    flange_left_duct->wizardParams.insert("b", (b+2*ff));
    flange_left_duct->wizardParams.insert("a", (a+2*ff));
    flange_left_duct->wizardParams.insert("s", (ff));
    flange_left_duct->processWizardInput();
    flange_left_duct->calculate();

    // Vertex data
    int index = 0;
    //    int count_a = 11;
    //    int count_b = 21;
    QVector3D position_rfd = QVector3D();
    QVector3D vertices[104];
    for(int w = 0; w <= 1; w++)
    {
        //left endcap outer vertices
        if(w == 0)
        {
            vertices[index] = matrix_rotation * QVector3D(0.0, this->b/2, -this->a/2) + position;
            index++;
            vertices[index] = matrix_rotation * QVector3D(0.0, this->b/2, this->a/2) + position;
            index++;
            vertices[index] = matrix_rotation * QVector3D(0.0, -this->b/2, this->a/2) + position;
            index++;
            vertices[index] = matrix_rotation * QVector3D(0.0, -this->b/2, -this->a/2) + position;
            index++;
        }
        //left endcap inner vertices
        else
        {
            vertices[index] = matrix_rotation * QVector3D(0.0, this->b/2 - this->s, -this->a/2 + this->s) + position;
            index++;
            vertices[index] = matrix_rotation * QVector3D(0.0, this->b/2 - this->s, this->a/2 - this->s) + position;
            index++;
            vertices[index] = matrix_rotation * QVector3D(0.0, -this->b/2 + this->s, this->a/2 - this->s) + position;
            index++;
            vertices[index] = matrix_rotation * QVector3D(0.0, -this->b/2 + this->s, -this->a/2 + this->s) + position;
            index++;
        }
        //now we start with the rotation:
        for(qreal i = 0.0; i <= 1.01; i += 0.1)
        {
            qreal angle = -this->alpha * i;
            QMatrix4x4 matrix_angle;
            matrix_angle.setToIdentity();
            matrix_angle.rotate(angle, 0.0, 0.0, 1.0);

            //rotation outer vertices
            if(w == 0)
            {
                if(this->b2 >= this->b)
                {
                    vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->b, -this->a/2)
                                                         + QVector3D(this->l1 + this->b2 - this->b, -this->r - this->b/2, 0.0)) + position;
                    index++;
                    vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->b, this->a/2)
                                                         + QVector3D(this->l1 + this->b2 - this->b, -this->r - this->b/2, 0.0)) + position;
                    index++;
                    vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r, this->a/2)
                                                         + QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)) + position;
                    index++;
                    vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r, -this->a/2)
                                                         + QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)) + position;
                    index++;
                }
                else
                {
                    vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->b2, -this->a/2)
                                                         + QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)
                                                         + QVector3D(0.0, this->b - this->b2, 0.0)) + position;
                    index++;
                    vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->b2, this->a/2)
                                                         + QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)
                                                         + QVector3D(0.0, this->b - this->b2, 0.0)) + position;
                    index++;
                    vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r, this->a/2)
                                                         + QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)) + position;
                    index++;
                    vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r, -this->a/2)
                                                         + QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)) + position;
                    index++;
                }
            }
            //rotation inner vertices
            else
            {
                if(this->b2 >= this->b)
                {
                    vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->b - this->s, -this->a/2 + this->s)
                                                         +QVector3D(this->l1 + this->b2 - this->b, -this->r - this->b/2, 0.0)) + position;
                    index++;
                    vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->b - this->s, this->a/2 - this->s)
                                                         +QVector3D(this->l1 + this->b2 - this->b, -this->r - this->b/2, 0.0)) + position;
                    index++;
                    vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->s, this->a/2 - this->s)
                                                         +QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)) + position;
                    index++;
                    vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->s, -this->a/2 + this->s)
                                                         +QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)) + position;
                    index++;
                }
                else
                {
                    vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->b2 - this->s, -this->a/2 + this->s)
                                                         + QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)
                                                         + QVector3D(0.0, this->b - this->b2, 0.0)) + position;
                    index++;
                    vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->b2 - this->s, this->a/2 - this->s)
                                                         +QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)
                                                         + QVector3D(0.0, this->b - this->b2, 0.0)) + position;
                    index++;
                    vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->s, this->a/2 - this->s)
                                                         +QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)) + position;
                    index++;
                    vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->s, -this->a/2 + this->s)
                                                         +QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)) + position;
                    index++;
                }
            }
        }
        //we're done now with rotation

        qreal angle = -this->alpha;
        QMatrix4x4 matrix_angle;
        matrix_angle.setToIdentity();
        matrix_angle.rotate(angle, 0.0, 0.0, 1.0);
        QVector3D offset = matrix_angle * QVector3D(this->l2, 0.0, 0.0);

        //endcap outer vertices
        if(w == 0)
        {
            if(this->b2 >= this->b)
            {
                vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->b, -this->a/2)
                                                     + QVector3D(this->l1 + this->b2 - this->b, -this->r - this->b/2, 0.0)
                                                     + offset) + position;
                index++;
                vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->b, this->a/2)
                                                     + QVector3D(this->l1 + this->b2 - this->b, -this->r - this->b/2, 0.0)
                                                     + offset) + position;
                index++;
                vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r, this->a/2)
                                                     + QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)
                                                     + offset) + position;
                index++;
                vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r, -this->a/2)
                                                     + QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)
                                                     + offset) + position;
                index++;
            }
            else
            {
                vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->b2, -this->a/2)
                                                     + QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)
                                                     + offset) + position;
                index++;
                vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->b2, this->a/2)
                                                     + QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)
                                                     + offset) + position;
                index++;
                vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r, this->a/2)
                                                     + QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)
                                                     + offset) + position;
                index++;
                vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r, -this->a/2)
                                                     + QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)
                                                     + offset) + position;
                index++;
            }
        }
        //endcap inner vertices
        else
        {
            if(this->b2 >= this->b)
            {
                vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->b - this->s, -this->a/2 + this->s)
                                                     +QVector3D(this->l1 + this->b2 - this->b, -this->r - this->b/2, 0.0)
                                                     + offset) + position;
                QVector3D lp1 = vertices[index];
                index++;
                vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->b - this->s, this->a/2 - this->s)
                                                     +QVector3D(this->l1 + this->b2 - this->b, -this->r - this->b/2, 0.0)
                                                     + offset) + position;
                index++;
                vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->s, this->a/2 - this->s)
                                                     +QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)
                                                     + offset) + position;
                QVector3D lp2 = vertices[index];
                index++;
                vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->s, -this->a/2 + this->s)
                                                     +QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)
                                                     + offset) + position;
                index++;
                position_rfd = (lp1 + lp2)/2;
                snap_flanges.append(position_rfd);
            }
            else
            {
                vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->b - this->s, -this->a/2 + this->s)
                                                     +QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)
                                                     + QVector3D(-this->b + this->b2, 0.0, 0.0)
                                                     + offset) + position;
                QVector3D lp1 = vertices[index];
                index++;
                vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->b - this->s, this->a/2 - this->s)
                                                     +QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)
                                                     + QVector3D(-this->b + this->b2, 0.0, 0.0)
                                                     + offset) + position;
                index++;
                vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->s, this->a/2 - this->s)
                                                     +QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)
                                                     + offset) + position;
                 QVector3D lp2 = vertices[index];
                index++;
                vertices[index] = matrix_rotation * (matrix_angle * QVector3D(0.0, this->r + this->s, -this->a/2 + this->s)
                                                     +QVector3D(this->l1 + 0.0, -this->r - this->b/2, 0.0)
                                                     + offset) + position;
                index++;
                position_rfd = (lp1 + lp2) / 2;
                snap_flanges.append((lp1 + lp2)/2);
            }
        }
    }

    flange_right_duct->wizardParams.insert("Position x", (position_rfd.x()));
    flange_right_duct->wizardParams.insert("Position y", (position_rfd.y()));
    flange_right_duct->wizardParams.insert("Position z", (position_rfd.z()));
    flange_right_duct->wizardParams.insert("Angle x", (angle_x));
    flange_right_duct->wizardParams.insert("Angle y", (angle_y));
    flange_right_duct->wizardParams.insert("Angle z", (angle_z));
    flange_right_duct->wizardParams.insert("l", (fe));
    flange_right_duct->wizardParams.insert("b", (b2+2*ff));
    flange_right_duct->wizardParams.insert("a", (this->a+2*ff));
    flange_right_duct->wizardParams.insert("s", (ff));
    flange_right_duct->processWizardInput();
    flange_right_duct->rotateAroundAxis(-alpha + 180.0, QVector3D(0.0, 0.0, 1.0), angle_x, angle_y, angle_z);
    flange_right_duct->calculate();

    boundingBox.enterVertex(flange_left_duct->pos_bot_1);
    boundingBox.enterVertex(flange_left_duct->pos_bot_2);
    boundingBox.enterVertex(flange_left_duct->pos_bot_3);
    boundingBox.enterVertex(flange_left_duct->pos_bot_4);
    boundingBox.enterVertex(flange_left_duct->pos_top_1);
    boundingBox.enterVertex(flange_left_duct->pos_top_2);
    boundingBox.enterVertex(flange_left_duct->pos_top_3);
    boundingBox.enterVertex(flange_left_duct->pos_top_4);


    boundingBox.enterVertex(flange_right_duct->pos_bot_1);
    boundingBox.enterVertex(flange_right_duct->pos_bot_2);
    boundingBox.enterVertex(flange_right_duct->pos_bot_3);
    boundingBox.enterVertex(flange_right_duct->pos_bot_4);
    boundingBox.enterVertex(flange_right_duct->pos_top_1);
    boundingBox.enterVertex(flange_right_duct->pos_top_2);
    boundingBox.enterVertex(flange_right_duct->pos_top_3);
    boundingBox.enterVertex(flange_right_duct->pos_top_4);

    this->snap_vertices.append(flange_left_duct->inner_pos_bot_2);
    this->snap_vertices.append(flange_left_duct->inner_pos_bot_3);
    this->snap_vertices.append(flange_left_duct->inner_pos_top_2);
    this->snap_vertices.append(flange_left_duct->inner_pos_top_3);

    this->snap_vertices.append(flange_right_duct->inner_pos_bot_1);
    this->snap_vertices.append(flange_right_duct->inner_pos_bot_4);
    this->snap_vertices.append(flange_right_duct->inner_pos_top_1);
    this->snap_vertices.append(flange_right_duct->inner_pos_top_4);


    for(int i = 0; i < 104; i++)
        boundingBox.enterVertex(vertices[i]);

    //238
    GLushort indicesFaces[238];
    for(int i = 0; i < 238; i++)
        indicesFaces[i] = 0xABCD;
    //outer faces
    for(int i = 0; i < 13; i++)
    {
        indicesFaces[2*i] = 4*i + 1;
        indicesFaces[2*i + 1] = 4*i + 2;
    }
    indicesFaces[26] = 0xABCD;
    for(int i = 0; i < 13; i++)
    {
        indicesFaces[27 + 2*i] = 4*i + 2;
        indicesFaces[27 + 2*i + 1] = 4*i + 3;
    }
    indicesFaces[53] = 0xABCD;
    for(int i = 0; i < 13; i++)
    {
        indicesFaces[54 + 2*i] = 4*i + 3;
        indicesFaces[54 + 2*i + 1] = 4*i;
    }
    indicesFaces[80] = 0xABCD;
    for(int i = 0; i < 13; i++)
    {
        indicesFaces[81 + 2*i] = 4*i;
        indicesFaces[81 + 2*i + 1] = 4*i + 1;
    }
    indicesFaces[107] = 0xABCD;
    //inner faces
    for(int i = 0; i < 13; i++)
    {
        indicesFaces[108 + 2*i] = 52 + 4*i + 2;
        indicesFaces[108 + 2*i + 1] = 52 + 4*i + 1;
    }
    indicesFaces[134] = 0xABCD;
    for(int i = 0; i < 13; i++)
    {
        indicesFaces[135 + 2*i] = 52 + 4*i + 3;
        indicesFaces[135 + 2*i + 1] = 52 + 4*i + 2;
    }
    indicesFaces[161] = 0xABCD;
    for(int i = 0; i < 13; i++)
    {
        indicesFaces[162 + 2*i] = 52 + 4*i;
        indicesFaces[162 + 2*i + 1] = 52 + 4*i + 3;
    }
    indicesFaces[188] = 0xABCD;
    for(int i = 0; i < 13; i++)
    {
        indicesFaces[189 + 2*i] = 52 + 4*i + 1;
        indicesFaces[189 + 2*i + 1] = 52 + 4*i;
    }
    indicesFaces[215] = 0xABCD;
    //front and back faces;
    GLushort frontAndBack[] =  {
        100, 48, 101, 49, 102, 50, 103, 51, 100, 48, 0xABCD,
        0, 52, 1, 53, 2, 54, 3, 55, 0, 52, 0xABCD
//        52, 0, 53, 1, 54, 2, 55, 3, 52, 0, 0xABCD
    };
    for(int i = 0; i < 22; i++)
        indicesFaces[216 + i] = frontAndBack[i];


    GLushort indicesLines[400];
    for(int i = 0; i < 28; i++)
    {
        indicesLines[8*i] = 4*i;
        indicesLines[8*i + 1] = 4*i + 1;
        indicesLines[8*i + 2] = 4*i + 1;
        indicesLines[8*i + 3] = 4*i + 2;
        indicesLines[8*i + 4] = 4*i + 2;
        indicesLines[8*i + 5] = 4*i + 3;
        indicesLines[8*i + 6] = 4*i + 3;
        indicesLines[8*i + 7] = 4*i;
    }
    for(int i = 0; i < 12; i++)
    {
        indicesLines[208 + 8*i] = 4*i;
        indicesLines[208 + 8*i + 1] = 4*i + 4;
        indicesLines[208 + 8*i + 2] = 4*i + 1;
        indicesLines[208 + 8*i + 3] = 4*i + 5;
        indicesLines[208 + 8*i + 4] = 4*i + 2;
        indicesLines[208 + 8*i + 5] = 4*i + 6;
        indicesLines[208 + 8*i + 6] = 4*i + 3;
        indicesLines[208 + 8*i + 7] = 4*i + 7;
    }
    for(int i = 0; i < 12; i++)
    {
        indicesLines[304 + 8*i] = 52 + 4*i;
        indicesLines[304 + 8*i + 1] = 52 + 4*i + 4;
        indicesLines[304 + 8*i + 2] = 52 + 4*i + 1;
        indicesLines[304 + 8*i + 3] = 52 + 4*i + 5;
        indicesLines[304 + 8*i + 4] = 52 + 4*i + 2;
        indicesLines[304 + 8*i + 5] = 52 + 4*i + 6;
        indicesLines[304 + 8*i + 6] = 52 + 4*i + 3;
        indicesLines[304 + 8*i + 7] = 52 + 4*i + 7;
    }


    arrayBufVertices->bind();
    arrayBufVertices->allocate(vertices, sizeof(vertices));

    indexBufFaces->bind();
    indexBufFaces->allocate(indicesFaces, sizeof(indicesFaces));

    indexBufLines->bind();
    indexBufLines->allocate(indicesLines, sizeof(indicesLines));


}



void CAD_air_ductTurn::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());

    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    s = wizardParams.value("s").toDouble();
    ff = wizardParams.value("ff").toDouble();
    fe = wizardParams.value("fe").toDouble();

    r = wizardParams.value("r").toDouble();
    b = wizardParams.value("b").toDouble();
    b2 = wizardParams.value("b2").toDouble();
    a = wizardParams.value("a").toDouble();
    alpha = wizardParams.value("alpha").toDouble();
    l1 = wizardParams.value("l1").toDouble();
    l2 = wizardParams.value("l2").toDouble();
    //    b2 = b;

    if(abs(alpha - 90) > 10E-8 && abs(b - b2) > 10E-8)
        qCDebug(powercad) << "Different width for input and output is only acceptable if angle = 90°";

}

void CAD_air_ductTurn::paint(GLWidget *glwidget)
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


QMatrix4x4 CAD_air_ductTurn::rotationOfFlange(quint8 num)
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
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(-alpha, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else
        return matrix_rotation;
}
