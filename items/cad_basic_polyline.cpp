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

#include "cad_basic_polyline.h"
#include "itemdb.h"
#include "glwidget.h"
#include <QDebug>
#include <QPen>

CAD_basic_polyline::CAD_basic_polyline() : CADitem(CADitemTypes::Basic_Polyline)
{
    this->widthByBlock = false;
    this->widthByLayer = true;

    processWizardInput();
    calculate();
}

CAD_basic_polyline::~CAD_basic_polyline()
{

}

QList<CADitemTypes::ItemType> CAD_basic_polyline::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_basic_polyline::wizardImage()
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

QString CAD_basic_polyline::iconPath()
{
    return ":/icons/cad_basic/cad_basic_polyline.svg";
}

QString CAD_basic_polyline::domain()
{
    return "Basic";
}

QString CAD_basic_polyline::description()
{
    return "Basic|Polyline";
}

void CAD_basic_polyline::calculate()
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

    int i = 0;

    foreach(Vertex vertex, vertices)
    {
        if (i == 0)
        {
//            min_x = vertex.pos.x();
//            min_y = vertex.pos.y();
//            min_z = vertex.pos.z();
//            max_x = vertex.pos.x();
//            max_y = vertex.pos.y();
//            max_z = vertex.pos.z();
            this->position = vertex.pos;
        }
//        else
//        {
//            min_x = qMin(min_x, vertex.pos.x());
//            min_y = qMin(min_y, vertex.pos.y());
//            min_z = qMin(min_z, vertex.pos.z());
//            max_x = qMax(max_x, vertex.pos.x());
//            max_y = qMax(max_y, vertex.pos.y());
//            max_z = qMax(max_z, vertex.pos.z());
//        }
        this->snap_vertices.append(vertex.pos);
        this->boundingBox.enterVertex(vertex.pos);
        i++;
    }

    //    this->snap_center = ((M3dVector)(M3dVector(p1) *0.5 + M3dVector(p2) * 0,5)).toM3dPoint();
}

void CAD_basic_polyline::processWizardInput()
{

}

void CAD_basic_polyline::paint(GLWidget *glwidget)
{
    QColor color_pen_tmp = getColorPen();

    QVector3D p1 = QVector3D();
    QVector3D p2 = QVector3D();
    glwidget->glBegin(GL_LINES);
    foreach (CAD_basic_polyline::Vertex vertex, vertices)
    {
        if (p1 == p2)
        {
            p1 = vertex.pos;
        }
        else
        {
            p2 = vertex.pos;

            qreal penWidth = 1.0;
            if (widthByLayer)
            {
                penWidth = layer->width / 100.0;
            }
            else if (widthByBlock)
            {

            }
            else
            {
                penWidth = vertex.widthStart;
            }

            // Default width setting
            if (penWidth < 1.0)
                penWidth = 1.0;

            glwidget->glLineWidth(penWidth);
            glwidget->setPaintingColor(color_pen_tmp);

            glwidget->glVertex3f((GLfloat)p1.x(), (GLfloat)p1.y(), (GLfloat)p1.z());
            glwidget->glVertex3f((GLfloat)p2.x(), (GLfloat)p2.y(), (GLfloat)p2.z());

            p1 = p2;
        }
    }
    glwidget->glEnd();
}
