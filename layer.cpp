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

#include "layer.h"

Layer::Layer(QObject *parent) :
    QObject(parent)
{
    on = true;
    solo = false;
    writable = true;
    width = 0;
    lineType = Undefined;
    parentLayer = NULL;
//    pen = Qt::transparent
//    brush = QBrush(QColor(255, 255, 255));

    const QMetaObject *mo = metaObject();
    metaEnum_lineType = mo->enumerator(mo->indexOfEnumerator("LineType"));
}

bool Layer::isEmpty()
{
    if (items.isEmpty() && subLayers.isEmpty())
        return true;
    else
        return false;
}

void Layer::serialOut(QByteArray *out)
{
    *out += "Layer;" + name.toUtf8() + ";" + pen.color().name().toUtf8() + ";" + brush.color().name().toUtf8() + ";" + QByteArray().setNum(width) + ";" + metaEnum_lineType.valueToKey(lineType);
    if (parentLayer)
        *out += ";" + parentLayer->name.toUtf8();
    *out += "\n";
}

void Layer::serialIn(QByteArray *in)
{

}
