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

#ifndef LAYER_H
#define LAYER_H

#include <QLoggingCategory>
#include <QObject>
#include <QString>
#include <QPen>
#include <QBrush>
#include <QMetaEnum>

#include "caditem.h"

Q_DECLARE_LOGGING_CATEGORY(layer)

class Layer;
typedef QList<Layer*> LayerList;

class Layer : public QObject
{
    Q_OBJECT
public:
    enum LineType { Undefined, Continuous, Dashed, Dotted };
    Q_ENUMS(LineType)

    explicit Layer(Layer *parentLayer = 0, QObject *parent = 0);
    ~Layer();

    // Model sutff
    void appendChild(Layer *childLayer);
    void insertChild(quint32 position, Layer *layer);
    bool removeChild(Layer *childLayer);
    Layer *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    Layer *parentLayer();
    LayerList getChildLayers();

    LayerList getAllLayers();

    bool isEmpty();

    QString name;
    QPen pen;
    QBrush brush;
    bool isOn;
    bool solo;
    bool isWriteable;
    int lineWidth;
    LineType lineType;
    QMetaEnum metaEnum_lineType;

    void serialOut(QByteArray *out);
    void serialIn(QByteArray *in);

    Layer* findByName(QString name);
    Layer *findByPath(QString path);

    void addItem(CADitem *item);
    void removeItem(CADitem *item);
    QList<CADitem*> getItems();

    QString path() const;
    QString path(QString name) const;

private:
    LayerList m_childLayers;
    Layer *m_parentLayer;

    QList<CADitem*> m_items;
};

#endif // LAYER_H
