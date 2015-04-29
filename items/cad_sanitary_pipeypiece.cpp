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

#include "cad_sanitary_pipeypiece.h"
#include "glwidget.h"

CAD_Sanitary_PipeYPiece::CAD_Sanitary_PipeYPiece() : CADitem(CADitemTypes::Sanitary_PipeYPiece)
{
    pipe = new CAD_basic_pipe;
    branch_1 = new CAD_basic_pipe;
    branch_2 = new CAD_basic_pipe;
    reducer = new CAD_Basic_PipeReducer;
    this->subItems.append(pipe);
    this->subItems.append(branch_1);
    this->subItems.append(branch_2);
    this->subItems.append(reducer);

    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("d1",    150.0);
    wizardParams.insert("d2",    100.0);
    wizardParams.insert("iso1",   10.0);
    wizardParams.insert("iso2",   10.0);
    wizardParams.insert("l1",    200.0);
    wizardParams.insert("l2",    150.0);
    wizardParams.insert("alpha",  30.0);
    wizardParams.insert("s",      10.0);

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

CAD_Sanitary_PipeYPiece::~CAD_Sanitary_PipeYPiece()
{
//    arrayBufVertices.destroy();
//    indexBufFaces.destroy();
//    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_Sanitary_PipeYPiece::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;

    flangable_items.append(CADitemTypes::Sanitary_ElectricWaterHeater);
    flangable_items.append(CADitemTypes::Sanitary_EmergencyEyeShower);
    flangable_items.append(CADitemTypes::Sanitary_EmergencyShower);
    flangable_items.append(CADitemTypes::Sanitary_Flange);
    flangable_items.append(CADitemTypes::Sanitary_LiftingUnit);
    flangable_items.append(CADitemTypes::Sanitary_Pipe);
    flangable_items.append(CADitemTypes::Sanitary_PipeEndCap);
    flangable_items.append(CADitemTypes::Sanitary_PipeReducer);
    flangable_items.append(CADitemTypes::Sanitary_PipeTeeConnector);
    flangable_items.append(CADitemTypes::Sanitary_PipeTurn);
    flangable_items.append(CADitemTypes::Sanitary_PipeYPiece);
    flangable_items.append(CADitemTypes::Sanitary_Shower);
    flangable_items.append(CADitemTypes::Sanitary_Sink);
    flangable_items.append(CADitemTypes::Sanitary_WashBasin);
    return flangable_items;
}

QImage CAD_Sanitary_PipeYPiece::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
                    
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Sanitary_PipeYPiece::iconPath()
{
    return ":/icons/cad_sanitary/cad_sanitary_pipeypiece.svg";
}

QString CAD_Sanitary_PipeYPiece::domain()
{
    return "Sanitary";
}

QString CAD_Sanitary_PipeYPiece::description()
{
    return "Sanitary|Pipe Y Piece";
}

void CAD_Sanitary_PipeYPiece::calculate()
{                
    boundingBox.reset();
                    
    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();
                                
    this->snap_basepoint = (position);

    pipe->wizardParams.insert("Position x", position.x());
    pipe->wizardParams.insert("Position y", position.y());
    pipe->wizardParams.insert("Position z", position.z());
    pipe->wizardParams.insert("Angle x", angle_x);
    pipe->wizardParams.insert("Angle y", angle_y);
    pipe->wizardParams.insert("Angle z", angle_z);
    pipe->wizardParams.insert("l", l1 / 2);
    pipe->wizardParams.insert("d", d1 + 2*iso1);
    pipe->wizardParams.insert("s",  s + iso1);
    pipe->layer = this->layer;
    pipe->processWizardInput();
    pipe->calculate();

    QVector3D position_reducer = position + matrix_rotation * QVector3D(l1/2, 0.0, 0.0);
    reducer->wizardParams.insert("Position x", position_reducer.x());
    reducer->wizardParams.insert("Position y", position_reducer.y());
    reducer->wizardParams.insert("Position z", position_reducer.z());
    reducer->wizardParams.insert("Angle x", angle_x);
    reducer->wizardParams.insert("Angle y", angle_y);
    reducer->wizardParams.insert("Angle z", angle_z);
    reducer->wizardParams.insert("l", 2 * l1 / 3);
    reducer->wizardParams.insert("d1", d1 + 2 * iso1);
    reducer->wizardParams.insert("d2", d2 + 2 * iso2);
    reducer->wizardParams.insert("e", 0.0);
    reducer->wizardParams.insert("s", s + iso1);
    reducer->layer = this->layer;
    reducer->processWizardInput();
    reducer->calculate();

    QVector3D position_branch = position + matrix_rotation * QVector3D(l1, 0.0, 0.0);
    branch_1->wizardParams.insert("Position x", position_branch.x());
    branch_1->wizardParams.insert("Position y", position_branch.y());
    branch_1->wizardParams.insert("Position z", position_branch.z());
    branch_1->wizardParams.insert("Angle x", angle_x);
    branch_1->wizardParams.insert("Angle y", angle_y);
    branch_1->wizardParams.insert("Angle z", angle_z);
    branch_1->wizardParams.insert("l", l2);
    branch_1->wizardParams.insert("d", d2 + 2*iso2);
    branch_1->wizardParams.insert("s",  s + iso2);
    branch_1->layer = this->layer;
    branch_1->processWizardInput();
    branch_1->rotateAroundAxis(alpha, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    branch_1->calculate();

    branch_2->wizardParams.insert("Position x", position_branch.x());
    branch_2->wizardParams.insert("Position y", position_branch.y());
    branch_2->wizardParams.insert("Position z", position_branch.z());
    branch_2->wizardParams.insert("Angle x", angle_x);
    branch_2->wizardParams.insert("Angle y", angle_y);
    branch_2->wizardParams.insert("Angle z", angle_z);
    branch_2->wizardParams.insert("l", l2);
    branch_2->wizardParams.insert("d", d2 + 2*iso2);
    branch_2->wizardParams.insert("s",  s + iso2);
    branch_2->layer = this->layer;
    branch_2->processWizardInput();
    branch_2->rotateAroundAxis(-alpha, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    branch_2->calculate();

    foreach(CADitem *item, subItems)
        this->boundingBox.enterVertices(item->boundingBox.getVertices());
    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l1 + cos(alpha / 180.0 * PI) * l2, 0.0, -sin(alpha / 180.0 * PI) * l2));
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l1 + cos(alpha / 180.0 * PI) * l2, 0.0,  sin(alpha / 180.0 * PI) * l2));
}

void CAD_Sanitary_PipeYPiece::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    d1 = wizardParams.value("d1").toDouble();
    d2 = wizardParams.value("d2").toDouble();
    iso1 = wizardParams.value("iso1").toDouble();
    iso2 = wizardParams.value("iso2").toDouble();
    l1 = wizardParams.value("l1").toDouble();
    l2 = wizardParams.value("l2").toDouble();
    alpha = wizardParams.value("alpha").toDouble();
    s = wizardParams.value("s").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

//void CAD_Sanitary_PipeYPiece::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Sanitary_PipeYPiece::rotationOfFlange(quint8 num)
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
        m.rotate(alpha, 0.0, 1.0, 0.0);
        return matrix_rotation * m;
    }
    else if(num == 3)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(-alpha, 0.0, 1.0, 0.0);
        return matrix_rotation * m;
    }
    else
        return matrix_rotation;
}
