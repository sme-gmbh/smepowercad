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

#include "caditem.h"
#include "layer.h"

CADitem::CADitem(CADitemTypes::ItemType type)
{
    this->type = type;
    this->layer = NULL;
    this->highlight = false;
    this->selected = false;
    this->index = 0;
    this->color_brush = Qt::transparent;
    this->color_pen = Qt::transparent;
    this->isMaintenanceArea = false;
    angle_x = 0.0;
    angle_y = 0.0;
    angle_z = 0.0;
    position = QVector3D(0.0, 0.0, 0.0);
    matrix_rotation.setToIdentity();
}

QColor CADitem::getColorPen()
{
    QColor color_pen_tmp = this->color_pen;

    if (color_pen_tmp == Qt::transparent)   // BYLAYER
    {
        color_pen_tmp = layer->pen.color();
    }
    else if (color_pen_tmp.value() < 50)
        color_pen_tmp = Qt::white;

    if (highlight || selected)
    {
        if (color_pen_tmp.value() > 127)
        {
            color_pen_tmp.setHsv(color_pen_tmp.hsvHue(), color_pen_tmp.hsvSaturation(), color_pen_tmp.value() - 100);
        }
        else
        {
            color_pen_tmp.setHsv(color_pen_tmp.hsvHue(), color_pen_tmp.hsvSaturation(), color_pen_tmp.value() + 100);
        }
    }

    return color_pen_tmp;
}

QColor CADitem::getColorBrush()
{
    QColor color_brush_tmp = this->color_brush;

    if (color_brush_tmp == Qt::transparent)   // BYLAYER
    {
        color_brush_tmp = layer->brush.color();
    }
    else if (color_brush_tmp.value() < 50)
        color_brush_tmp = Qt::white;

    if (highlight || selected)
    {
        if (color_brush_tmp.value() > 127)
        {
            color_brush_tmp.setHsv(color_brush_tmp.hsvHue(), color_brush_tmp.hsvSaturation(), color_brush_tmp.value() - 100);
        }
        else
        {
            color_brush_tmp.setHsv(color_brush_tmp.hsvHue(), color_brush_tmp.hsvSaturation(), color_brush_tmp.value() + 100);
        }
    }

    return color_brush_tmp;
}

void CADitem::setLayer(Layer *layer)
{
    this->layer = layer;
    this->setLayer_processItems(this->subItems);
}

void CADitem::setLayer_processItems(QList<CADitem *> subItems)
{
    foreach (CADitem* item, subItems)
    {
        item->layer = this->layer;
        this->setLayer_processItems(item->subItems);
    }
}

void CADitem::setID(quint64 id)
{
    this->id = id;
    this->setID_processItems(this->subItems);
}

void CADitem::setID_processItems(QList<CADitem*> subItems)
{
    foreach (CADitem* item, subItems)
    {
        item->id = this->id;
        this->setID_processItems(item->subItems);
    }
}

void CADitem::serialOut(QByteArray* out)
{
    QList<QString> keys = wizardParams.keys();

    *out += "Item" + QString().setNum(this->type).toUtf8() + ";" + layer->name.toUtf8() + ";" + QString().setNum(id) + ";";

    foreach (QString key, keys)
    {
        *out += key + ":";
        QVariant value = wizardParams.value(key);
        if (value.type() == QVariant::Double)
            *out += value.toString();
        *out += ";";
    }

    *out += "\n";
}

bool CADitem::serialIn(QByteArray* in)
{

}

QVector3D CADitem::anglesFromVector(QVector3D vector)
{
    qreal angle_x = 0.0;
    qreal angle_y = 0.0;
    qreal angle_z = 0.0;

    //optimize until difference angle ist smaller than 10E-6°
    quint32 i = 1;
    //tbd: we need an algorithm with faster convergence rate
    //at this moment we just terminate after 10000 steps
    while(difference(vector, angle_x, angle_y, angle_z) > 10E-6 && i < 10000)
    {
        //optimize angle_x
        if( difference(vector, angle_x + alpha(i), angle_y, angle_z) < difference(vector, angle_x, angle_y, angle_z))
            angle_x = angle_x + alpha(i);
        else if( difference(vector, angle_x - alpha(i), angle_y, angle_z) < difference(vector, angle_x, angle_y, angle_z))
            angle_x = angle_x - alpha(i);
        //optimize angle_y
        if( difference(vector, angle_x, angle_y + alpha(i), angle_z) < difference(vector, angle_x, angle_y, angle_z))
            angle_y = angle_y + alpha(i);
        else if( difference(vector, angle_x, angle_y - alpha(i), angle_z) < difference(vector, angle_x, angle_y, angle_z))
            angle_y = angle_y - alpha(i);
        //optimize angle_z
        if( difference(vector, angle_x, angle_y, angle_z + alpha(i)) < difference(vector, angle_x, angle_y, angle_z))
            angle_z = angle_z + alpha(i);
        else if( difference(vector, angle_x, angle_y, angle_z - alpha(i)) < difference(vector, angle_x, angle_y, angle_z))
            angle_z = angle_z - alpha(i);
        i++;
    }
    return QVector3D(angle_x, angle_y, angle_z);
}

qreal CADitem::difference(QVector3D vec, qreal angle_x, qreal angle_y, qreal angle_z)
{
    QMatrix4x4 matrix_difference;
    matrix_difference.setToIdentity();
    matrix_difference.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_difference.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_difference.rotate(angle_z, 0.0, 0.0, 1.0);
    QVector3D a = matrix_difference * QVector3D(1.0, 0.0, 0.0);
    qreal phi = acos(a.dotProduct(vec, a) / (vec.length() * a.length())) / PI * 180.0;
    return phi;
}

qreal CADitem::alpha(quint32 i)
{
    return 45.0 / i;
}

void CADitem::rotateAroundAxis(qreal angle, QVector3D axis, qreal angle_x, qreal angle_y, qreal angle_z)
{
    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
    matrix_rotation.rotate(angle, QVector3D(axis));
}

CADitemTypes::ItemType CADitem::getType()
{
    return type;
}
