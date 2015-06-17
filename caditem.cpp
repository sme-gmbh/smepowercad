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
    this->arrayBufVertices = NULL;
    this->indexBufFaces = NULL;
    this->indexBufLines = NULL;
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

void CADitem::rotateAroundAxis(qreal angle, QVector3D axis, qreal angle_x, qreal angle_y, qreal angle_z)
{
    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
    matrix_rotation.rotate(angle, QVector3D(axis));
    QVector3D angles = MAngleCalculations().anglesFromMatrix(matrix_rotation);
    this->wizardParams.insert("Angle x", (angles.x()));
    this->wizardParams.insert("Angle y", (angles.y()));
    this->wizardParams.insert("Angle z", (angles.z()));
    this->processWizardInput();
    this->calculate();
}

WizardParams CADitem::rotateAroundPoint(QVector3D center, qreal rot_x, qreal rot_y, qreal rot_z)
{
    //calculate new position of object
    QVector3D vec = this->position - center;
    QMatrix4x4 m;
    m.setToIdentity();
    m.rotate(rot_x, 1.0, 0.0, 0.0);
    m.rotate(rot_y, 0.0, 1.0, 0.0);
    m.rotate(rot_z, 0.0, 0.0, 1.0);
    QVector3D pos = center + m * vec;
    WizardParams newParams;
    newParams.insert("Position x", pos.x());
    newParams.insert("Position y", pos.y());
    newParams.insert("Position z", pos.z());
    //rotate the object itself
    matrix_rotation = matrix_rotation * m;
    QVector3D angles = MAngleCalculations().anglesFromMatrix(matrix_rotation);
    newParams.insert("Angle x", (angles.x()));
    newParams.insert("Angle y", (angles.y()));
    newParams.insert("Angle z", (angles.z()));
    //do the same for subitems
    return newParams;
}

CADitemTypes::ItemType CADitem::getType()
{
    return type;
}

QList<MTriangle> CADitem::getTriangles()
{
    QList<MTriangle> l;
    l.append(triangleListFromIndexedBuffers());

    foreach(CADitem *item, subItems)
        l.append(item->getTriangles());
    return l;
}

QList<MTriangle> CADitem::triangleListFromIndexedBuffers()
{
    QList<MTriangle> list;
    if((indexBufFaces != NULL) && (arrayBufVertices != NULL))
    {
        //reads the index buffer into dataFaces
        indexBufFaces->bind();
        int numberOfIndices = indexBufFaces->size() / sizeof(GLushort);
        void* dataRawFaces = malloc(indexBufFaces->size());
        indexBufFaces->read(0, dataRawFaces, indexBufFaces->size());
        GLushort* dataFaces = (GLushort*) dataRawFaces;
        indexBufFaces->release();

        //reads the vertex buffer into dataVertices
        arrayBufVertices->bind();
        void* dataRawVertices = malloc(arrayBufVertices->size());
        arrayBufVertices->read(0, dataRawVertices, arrayBufVertices->size());
        QVector3D* dataVertices = (QVector3D*) dataRawVertices;
        arrayBufVertices->release();

        //fill list
        for(int i = 2; i < numberOfIndices; i++)
        {
            if(dataFaces[i] == 0xABCD)  //0xABCD = primitive restart index
            {
                i += 2;
                continue;
            }
            MTriangle triangle = MTriangle(dataVertices[dataFaces[i-2]], dataVertices[dataFaces[i-1]], dataVertices[dataFaces[i]]);
            list.append(triangle);
        }
        delete dataRawVertices;
        delete dataRawFaces;
    }
    return list;
}
