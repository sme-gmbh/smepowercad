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

    circle = new CAD_basic_circle();
    line = new CAD_basic_line();
    arrow_1 = new CAD_basic_line();
    arrow_2 = new CAD_basic_line();
    this->subItems.append(circle);
    this->subItems.append(arrow_1);
    this->subItems.append(arrow_2);
    this->subItems.append(line);
   
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
    this->subItems.append(circle);
    this->subItems.append(arrow_1);
    this->subItems.append(arrow_2);
    this->subItems.append(line);
    this->snap_basepoint = (position);


    QVector3D pos_circle = (matrix_rotation * QVector3D(l/4, 0.0, a + 0.1)) + position;
    circle->wizardParams.insert("Center x", (pos_circle.x()));
    circle->wizardParams.insert("Center y", (pos_circle.y()));
    circle->wizardParams.insert("Center z", (pos_circle.z()));
    circle->wizardParams.insert("Angle x", (angle_x));
    circle->wizardParams.insert("Angle y", (angle_y));
    circle->wizardParams.insert("Angle z", (angle_z));
    circle->wizardParams.insert("r", (0.25 * l));
    circle->color_brush = Qt::black;
    circle->processWizardInput();
    circle->calculate();
    circle->layer = this->layer;

    //paint first step
    CAD_basic_box *step = new CAD_basic_box;
    this->subItems.append(step);
    QVector3D pos = (matrix_rotation * QVector3D(l/2, 0.0, a/2)) + position;
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
    this->snap_vertices.append(step->snap_vertices.at(0));
    this->snap_vertices.append(step->snap_vertices.at(3));

    //paint arrow
    QVector3D pos_start = position + matrix_rotation * QVector3D(0.5 * l, 0.0, a);
    QVector3D pos_end = position + matrix_rotation * QVector3D(l, 0.0, a);
    line->wizardParams.insert("Position x1", pos_start.x());
    line->wizardParams.insert("Position y1", pos_start.y());
    line->wizardParams.insert("Position z1", pos_start.z());
    line->wizardParams.insert("Position x2", pos_end.x());
    line->wizardParams.insert("Position y2", pos_end.y());
    line->wizardParams.insert("Position z2", pos_end.z());
    line->wizardParams.insert("Width", 1.0);
    line->processWizardInput();
    line->calculate();
    line->layer = this->layer;

    for(int i = 1; i < n; i++)
    {
        //paint step
        CAD_basic_box *step = new CAD_basic_box();
        this->subItems.append(step);
        QVector3D pos = (matrix_rotation * QVector3D(l/2 + i * l, 0.0, i * a)) + position;
        step->wizardParams.insert("Position x", (pos.x()));
        step->wizardParams.insert("Position y", (pos.y()));
        step->wizardParams.insert("Position z", (pos.z()));
        step->wizardParams.insert("Angle x", (angle_x));
        step->wizardParams.insert("Angle y", (angle_y));
        step->wizardParams.insert("Angle z", (angle_z));
        step->wizardParams.insert("l", (l));
        step->wizardParams.insert("b", (b));
        step->wizardParams.insert("a", (2*a));
        step->processWizardInput();
        step->calculate();
        step->layer = this->layer;
        this->boundingBox.enterVertices(step->boundingBox.getVertices());

        //paint arrow
        CAD_basic_line *line = new CAD_basic_line();
        this->subItems.append(line);
        QVector3D pos_start = position + matrix_rotation * QVector3D(i*l, 0.0, a + i*a);
        QVector3D pos_end = position + matrix_rotation * QVector3D(l + i*l, 0.0, a + i*a);
        line->wizardParams.insert("Position x1", pos_start.x());
        line->wizardParams.insert("Position y1", pos_start.y());
        line->wizardParams.insert("Position z1", pos_start.z());
        line->wizardParams.insert("Position x2", pos_end.x());
        line->wizardParams.insert("Position y2", pos_end.y());
        line->wizardParams.insert("Position z2", pos_end.z());
        line->wizardParams.insert("Width", 1.0);
        line->processWizardInput();
        line->calculate();
        line->layer = this->layer;

        //we use some snap vertices of last step
        if(i == n-1)
        {
            this->snap_vertices.append(step->snap_vertices.at(5));
            this->snap_vertices.append(step->snap_vertices.at(6));
            this->snap_center.append(step->snap_center.at(5));
        }
        // we don't paint lines on the backside of the stairs
        if(i >= 1)
        {
            static GLushort indicesLines[] = {
                0, 1,
                2, 3,
                4, 5,
                5, 6,
                6, 7,
                7, 4,
                0, 4,
                1, 5,
                2, 6,
                3, 7
            };
            step->indexBufLines->bind();
            step->indexBufLines->allocate(indicesLines, sizeof(indicesLines));
        }
    }

    //paint arrow tips
    QVector3D pos_start_1 = position + matrix_rotation * QVector3D(n*l - l, b/2, n*a);
    QVector3D pos_end_1 = position + matrix_rotation * QVector3D(n*l, 0.0, n*a);
    arrow_1->wizardParams.insert("Position x1", pos_start_1.x());
    arrow_1->wizardParams.insert("Position y1", pos_start_1.y());
    arrow_1->wizardParams.insert("Position z1", pos_start_1.z());
    arrow_1->wizardParams.insert("Position x2", pos_end_1.x());
    arrow_1->wizardParams.insert("Position y2", pos_end_1.y());
    arrow_1->wizardParams.insert("Position z2", pos_end_1.z());
    arrow_1->wizardParams.insert("Width", 1.0);
    arrow_1->processWizardInput();
    arrow_1->calculate();
    arrow_1->layer = this->layer;

    QVector3D pos_start_2 = position + matrix_rotation * QVector3D(n*l - l, -b/2, n*a);
    arrow_2->wizardParams.insert("Position x1", pos_start_2.x());
    arrow_2->wizardParams.insert("Position y1", pos_start_2.y());
    arrow_2->wizardParams.insert("Position z1", pos_start_2.z());
    arrow_2->wizardParams.insert("Position x2", pos_end_1.x());
    arrow_2->wizardParams.insert("Position y2", pos_end_1.y());
    arrow_2->wizardParams.insert("Position z2", pos_end_1.z());
    arrow_2->wizardParams.insert("Width", 1.0);
    arrow_2->processWizardInput();
    arrow_2->calculate();
    arrow_2->layer = this->layer;
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
