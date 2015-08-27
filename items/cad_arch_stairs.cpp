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

#include "cad_arch_stairs.h"
#include "glwidget.h"

CAD_Arch_Stairs::CAD_Arch_Stairs() : CADitem(CADitemTypes::Arch_Stairs)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",  200.0);
    wizardParams.insert("b", 1000.0);
    wizardParams.insert("l",  300.0);
    wizardParams.insert("n",     20);

   
    processWizardInput();
    calculate();
}

CAD_Arch_Stairs::~CAD_Arch_Stairs()
{
//    arrayBufVertices->destroy();
//    indexBufFaces->destroy();
//    indexBufLines->destroy();
//    delete arrayBufVertices;
//    delete indexBufFaces;
//    delete indexBufLines;
}

QList<CADitemTypes::ItemType> CAD_Arch_Stairs::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    
    return flangable_items;
}

QImage CAD_Arch_Stairs::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");
                       
    return image;
}

QString CAD_Arch_Stairs::iconPath()
{
    return ":/icons/cad_arch/cad_arch_stairs.svg";
}

QString CAD_Arch_Stairs::domain()
{
    return "Architecture";
}

QString CAD_Arch_Stairs::description()
{
    return "Architecture|Stairs";
}

void CAD_Arch_Stairs::calculate()
{                
    boundingBox.reset();
                    
    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();
    this->subItems.clear();
                                
    this->snap_basepoint = (position);

    for(int i = 0; i < n; i++)
    {
        CAD_basic_box *step = new CAD_basic_box;
        this->subItems.append(step);
        QVector3D pos = (matrix_rotation * QVector3D(l/2 + i * l, 0.0, a/2 + i * a)) + position;
        step->wizardParams.insert("Position x", (pos.x()));
        step->wizardParams.insert("Position y", (pos.y()));
        step->wizardParams.insert("Position z", (pos.z()));
        step->wizardParams.insert("Angle x", (angle_x));
        step->wizardParams.insert("Angle y", (angle_y));
        step->wizardParams.insert("Angle z", (angle_z));
        step->wizardParams.insert("l", (l));
        step->wizardParams.insert("b", (b));
        step->wizardParams.insert("a", (a));
        step->processWizardInput();
        step->calculate();
        step->layer = this->layer;
        this->boundingBox.enterVertices(step->boundingBox.getVertices());
        this->snap_vertices.append(step->snap_vertices);
    }
}

void CAD_Arch_Stairs::processWizardInput()
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
    n = wizardParams.value("n").toInt();


    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

QMatrix4x4 CAD_Arch_Stairs::rotationOfFlange(quint8 num)
{
    return matrix_rotation;
}

//void CAD_Arch_Stairs::paint(GLWidget *glwidget)
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
