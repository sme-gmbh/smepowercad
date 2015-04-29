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

#ifndef ITEMDB_H
#define ITEMDB_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QByteArray>
#include <QDataStream>
#include <QSvgRenderer>
#include <QPainter>
#include <QIODevice>
#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include <QDebug>

#include "layer.h"

#include "caditemheaderincludes.h"
#include "caditemtypes.h"
#include "restorepoint.h"


class ItemDB : public QObject
{
    Q_OBJECT

public:
    explicit ItemDB(QObject *parent = 0);
    ~ItemDB();
    QList<Layer*> layers;
    bool layerSoloActive;

    QList<RestorePoint*> restorePoints;

    int getNumberOfItemTypes();
    QList<QString> getDomains();
    QList<int> getItemTypesByDomain(QString domain);
    QString getItemDescriptionByItemType(CADitemTypes::ItemType type);
    CADitemTypes::ItemType getItemTypeByItemDescription(QString description);
    QString getIconPathByItemType(CADitemTypes::ItemType type);
    QPixmap getIconByItemType(CADitemTypes::ItemType type, QSize size);


    Layer *addLayer(QString layerName, QString parentLayerName = QString());
    Layer *addLayer(QString layerName, Layer* parentLayer);
    bool moveLayer(QString layerName, QString newParentLayerName, quint32 position);
    bool renameLayer(QString layerName, QString newLayerName);
    bool renameLayer(Layer* layer, QString newLayerName);
    bool deleteLayer(Layer* layer);
    Layer* getLayerByName(QString layerName);
    Layer* getTopLevelLayer();
    bool isLayerValid(Layer* layer);

    void addItem(CADitem* item, QString LayerName);
    void addItem(CADitem* item, Layer* layer);
    void deleteItem(CADitem* item);
    bool deleteItem(quint64 id);
    void deleteItems(QList<CADitem*> items);
    bool changeLayerOfItem(CADitem* item, Layer* newLayer);
    bool changeLayerOfItem(quint64 id, QString newLayerName);
    CADitem *createItem(CADitemTypes::ItemType type);
    CADitem *drawItem(Layer *layer, CADitemTypes::ItemType type);
    CADitem *drawItem(QString layerName, CADitemTypes::ItemType type);
    CADitem *getItemById(quint64 id);
    bool modifyItem(quint64 &id, QString &key, QString &value);
    CADitem* drawItem_withRestorePoint(Layer *layer, CADitemTypes::ItemType type, WizardParams wizardParams);
    void deleteItems_withRestorePoint(QList<CADitem*> items);
    void modifyItem_withRestorePoint(CADitem* item, WizardParams newParams);
    void setRestorePoint();
    void makeRestore();

    void itemAdded(CADitem* item);
    void itemModified(CADitem* item);
    void itemDeleted(CADitem* item);

    QByteArray network_newLayer(QMap<QString, QString> data);
    QByteArray network_modifyLayer(QMap<QString, QString> data);
    QByteArray network_moveLayer(QMap<QString, QString> data);
    QByteArray network_deleteLayer(QMap<QString, QString> data);

    QByteArray network_getAll();
    QByteArray network_getItem(quint64 id);
    QByteArray network_newItem(quint32 type, QMap<QString, QString> data);
    QByteArray network_modifyItem(quint64 id, QMap<QString, QString> data);
    QByteArray network_changeLayerOfItem(quint64 id, QMap<QString, QString> data);
    QByteArray network_deleteItem(quint64 id);

    bool file_storeDB(QString filename);
    void file_storeDB_processLayers(QDomDocument document, QDomElement parentElement, QList<Layer*> layers);
    void file_storeDB_processItems(QDomDocument document, QDomElement parentElement, QList<CADitem*> items);
    bool file_loadDB(QString filename, QString *error);
    void file_loadDB_parseDomElement(QDomElement element, Layer* currentLayer, bool mapByDescription, QMap<int, QString> *file_itemDescriptionByItemType, QString *error);

private:
    QList<QString> domains;
    QMap<QString, int> itemTypesByDomain;
    QMap <int, QString> iconPathByItemType;
    QMap <int, QString> itemDescriptionByItemType;
    QMap <QString, int> itemTypeByItemDescription;
    void deriveDomainsAndItemTypes();

    Layer* topLevelLayer;
    QMap<QString, Layer*> layerMap;
    QMap<quint64, CADitem*> itemMap;
    CADitemTypes::ItemType activeDrawCommand;
    quint64 currentItemId;
    void network_getAll_processLayers(QList<Layer *> layers, QByteArray *answer);
    void network_getAll_processItems(QList<CADitem *> items, QByteArray* answer);

signals:
    void signal_layerAdded(Layer* newLayer, Layer* parentLayer);
    void signal_layerChanged(Layer* layer);
    void signal_layerMoved(Layer* layer);
    void signal_layerDeleted(Layer* layer);
    void signal_itemAdded(CADitem* item, Layer* layer);
    void signal_itemDeleted(CADitem* item);
//    void signal_itemModified(CADitem* item, Layer* layer);
    void signal_layerManagerUpdateNeeded();
    void signal_repaintNeeded();

public slots:

};

#endif // ITEMDB_H
