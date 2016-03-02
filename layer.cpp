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
#include "layermanager.h"

Layer::Layer(Layer *parentLayer, QObject *parent)
    : QObject(parent),
      m_childLayers(),
      m_parentLayer(parentLayer)
{
    isOn = true;
    isWriteable = true;
    solo = false;
    lineWidth = 0;
    lineType = Continuous;

    if (parentLayer) parentLayer->appendChild(this);
    const QMetaObject *mo = metaObject();
    metaEnum_lineType = mo->enumerator(mo->indexOfEnumerator("LineType"));
}

Layer::~Layer()
{
    qDeleteAll(m_childLayers);
}

void Layer::appendChild(Layer *childLayer)
{
    childLayer->m_parentLayer = this;
    m_childLayers.append(childLayer);
}

void Layer::insertChild(quint32 position, Layer *layer)
{
    layer->m_parentLayer = this;
    m_childLayers.insert(position, layer);
}

bool Layer::removeChild(Layer *childLayer)
{
    return m_childLayers.removeOne(childLayer);
}

Layer *Layer::child(int row)
{
    return m_childLayers.value(row);
}

int Layer::childCount() const
{
    return m_childLayers.count();
}

int Layer::columnCount() const
{
    return 8;
}

QVariant Layer::data(int column) const
{
    if (column == 0) return name;
    else if (column == 1) return isOn;
    else if (column == 2) return isWriteable;
    else if (column == 3) return solo;
    else if (column == 4) return brush.color().name();
    else if (column == 5) return pen.color().name();
    else if (column == 6) return lineWidth;
    else if (column == 7) return metaEnum_lineType.valueToKey(lineType);

    return QVariant();
}

int Layer::row() const
{
    if (m_parentLayer)
        return m_parentLayer->m_childLayers.indexOf(const_cast<Layer*>(this));

    return 0;
}

Layer *Layer::parentLayer()
{
    return m_parentLayer;
}

LayerList Layer::getChildLayers()
{
    return m_childLayers;
}

LayerList Layer::getAllLayers()
{
    LayerList ret = LayerList();
    ret.append(this);

    foreach (Layer *layer, m_childLayers)
        ret.append(layer->getAllLayers());

    return ret;
}

bool Layer::isEmpty()
{
    return (m_childLayers.isEmpty() && m_items.isEmpty());
}

void Layer::serialOut(QByteArray *out)
{
    QString outString = QString("Layer;%1;%2;%3;%4;%5")
            .arg(name)
            .arg(pen.color().name())
            .arg(brush.color().name())
            .arg(lineWidth)
            .arg(metaEnum_lineType.valueToKey(lineType));
    if (m_parentLayer)
        outString += ";" + m_parentLayer->name;
    *out += outString.toUtf8() + "\n";
}

void Layer::serialIn(QByteArray *in)
{

}

Layer *Layer::findByName(QString name)
{
    Layer *ret = NULL;

    foreach (Layer *layer, m_childLayers) {
        if (layer->name == name) return layer;

        ret = layer->findByName(name);
    }

    return ret;
}

Layer *Layer::findByPath(QString path)
{
    Layer *ret = NULL;

    foreach (Layer *layer, m_childLayers) {
        if (layer->path() == path) return layer;

        ret = layer->findByPath(path);
    }

    return ret;
}

void Layer::addItem(CADitem *item)
{
    m_items.append(item);
}

void Layer::removeItem(CADitem *item)
{
    m_items.removeOne(item);
}

QList<CADitem*> Layer::getItems()
{
    return m_items;
}

QString Layer::path() const
{
    QString path = name;

    if (m_parentLayer) {
        if (!m_parentLayer->parentLayer()) return name;

        path.prepend(m_parentLayer->path() + " • ");
    }

    return path;
}

QString Layer::path(QString name) const
{
    QString path = name;
    if (m_parentLayer) path.prepend(this->path() + " • ");

    return path;
}
