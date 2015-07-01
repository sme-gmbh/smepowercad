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

#include "cad_gas_cdatank.h"
#include "glwidget.h"

CAD_Gas_CDATank::CAD_Gas_CDATank() : CADitem(CADitemTypes::Gas_CDATank)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

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

CAD_Gas_CDATank::~CAD_Gas_CDATank()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;

}

QList<CADitemTypes::ItemType> CAD_Gas_CDATank::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Gas_CDABallValve);
    flangable_items.append(CADitemTypes::Gas_CDACompressor);
    flangable_items.append(CADitemTypes::Gas_CDADesiccantDryer);
    flangable_items.append(CADitemTypes::Gas_CDADiaphragmValve);
    flangable_items.append(CADitemTypes::Gas_CDAFilter);
    flangable_items.append(CADitemTypes::Gas_CDAFlowmeter);
    flangable_items.append(CADitemTypes::Gas_CDAHose);
    flangable_items.append(CADitemTypes::Gas_CDAManometer);
    flangable_items.append(CADitemTypes::Gas_CDAMoistureSensor);
    flangable_items.append(CADitemTypes::Gas_CDANonReturnValve);
    flangable_items.append(CADitemTypes::Gas_CDAPipe);
    flangable_items.append(CADitemTypes::Gas_CDAPipeTeeConnector);
    flangable_items.append(CADitemTypes::Gas_CDAPipeTurn);
    flangable_items.append(CADitemTypes::Gas_CDAPressureRegulator);
    flangable_items.append(CADitemTypes::Gas_CDAQuickLockCoupling);
    flangable_items.append(CADitemTypes::Gas_CDARefrigerantDryer);
    flangable_items.append(CADitemTypes::Gas_CDATank);
    flangable_items.append(CADitemTypes::Gas_VACBallValve);
    flangable_items.append(CADitemTypes::Gas_VACDiaphragmValve);
    flangable_items.append(CADitemTypes::Gas_VACFilter);
    flangable_items.append(CADitemTypes::Gas_VACFlowmeter);
    flangable_items.append(CADitemTypes::Gas_VACHose);
    flangable_items.append(CADitemTypes::Gas_VACLiquidSeparator);
    flangable_items.append(CADitemTypes::Gas_VACManometer);
    flangable_items.append(CADitemTypes::Gas_VACNonReturnValve);
    flangable_items.append(CADitemTypes::Gas_VACPipe);
    flangable_items.append(CADitemTypes::Gas_VACPipeTeeConnector);
    flangable_items.append(CADitemTypes::Gas_VACPipeTurn);
    flangable_items.append(CADitemTypes::Gas_VACPump);
    flangable_items.append(CADitemTypes::Gas_VACQuickLockCoupling);
    flangable_items.append(CADitemTypes::Gas_VACTank);
    return flangable_items;
}

QImage CAD_Gas_CDATank::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");     
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Gas_CDATank::iconPath()
{
    return ":/icons/cad_gas/cad_gas_cdatank.svg";
}

QString CAD_Gas_CDATank::domain()
{
    return "Gas";
}

QString CAD_Gas_CDATank::description()
{
    return "Gas|CDA Tank";
}

void CAD_Gas_CDATank::calculate()
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
}

void CAD_Gas_CDATank::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();
}

//void CAD_Gas_CDATank::paint(GLWidget *glwidget)
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

QMatrix4x4 CAD_Gas_CDATank::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}
