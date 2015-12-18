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

#include <QAbstractItemModel>
#include <QList>
#include <QVariant>
#include <QIcon>
#include <QPixmap>
#include <QByteArray>
#include <QDomDocument>
#include <QFile>
#include <QPainter>
#include <QSvgRenderer>

#include "layer.h"
#include "caditemheaderincludes.h"
#include "caditemtypes.h"
#include "restorepoint.h"

Q_DECLARE_LOGGING_CATEGORY(itemdb)

class ItemDB : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ItemDB(QObject *parent = 0);
    ~ItemDB();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    LayerList getLayerList();
    Layer* addLayer(QString name, QString parentLayerName = QString());
    Layer* addLayer(QString name, Layer *parentLayer);
    Layer* insertLayer(QString name, const QModelIndex &parent, int at);
    bool moveLayer(QString layerName, QString newParentLayerName, quint32 position);
    bool renameLayer(QString layerName, QString newLayerName);
    bool renameLayer(Layer *layer, QString newLayerName);
    bool deleteLayer(Layer *layer);
    bool deleteLayerAt(const QModelIndex &parent, const QModelIndex &index);
    QString getIconPathByItemType(CADitemTypes::ItemType type);
    QIcon getIconByItemType(CADitemTypes::ItemType type, QSize size);
    QString getItemDescriptionByItemType(CADitemTypes::ItemType type);
    QList<int> getItemTypesByDomain(QString domain);
    QStringList getDomains();

    Layer* findLayerByName(QString name);

    QList<RestorePoint*> restorePoints_undo;
    QList<RestorePoint*> restorePoints_redo;
    bool layerSoloActive;

    void addItem(CADitem *item, Layer *layer);
    void addItem(CADitem *item, QString layerName);
    void deleteItem(CADitem *item);
    bool deleteItem(quint64 id);
    void deleteItems(QList<CADitem*> items);
    void deleteItems_withRestorePoint(QList<CADitem*> items);
    bool changeLayerOfItem(CADitem *item, Layer *newLayer);
    bool changeLayerOfItem(quint64 id, QString newLayerName);
    CADitem* createItem(CADitemTypes::ItemType type);
    CADitem* drawItem(Layer *layer, CADitemTypes::ItemType type);
    CADitem* drawItem(QString layerName, CADitemTypes::ItemType type);
    CADitem *drawItem_withRestorePoint(Layer *layer, CADitemTypes::ItemType type, WizardParams wizardParams);
    bool modifyItem(quint64 &id, QString key, QString value);
    void modifyItem_withRestorePoint(CADitem *item, WizardParams newParams);
    void setRestorePoint();
    void restore_clearRedo();
    void restore_undo();
    void restore_redo();

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

    bool file_storeDB(const QString filename, QMatrix4x4 projectionMatrix, QMatrix4x4 glSelectMatric, QMatrix4x4 modelviewMatrix, QMatrix4x4 rotationMatrix);
    bool file_loadDB(const QString filename, QString *error, QMatrix4x4 *projectionMatrix, QMatrix4x4 *glSelectMatrix, QMatrix4x4 *modelviewMatrix, QMatrix4x4 *rotationMatrix);
    void deriveDomainsAndItemTypes();

    Layer* getRootLayer();

private:
    QIcon m_iconLayerOn;
    QIcon m_iconLayerOff;
    QIcon m_iconPencilOn;
    QIcon m_iconPencilOff;
    QPixmap m_layerManagerColorIconBG;

    QStringList m_domains;
    QMap<QString, int> m_itemTypesByDomain;
    QMap<int, QString> m_iconPathByItemType;
    QMap<int, QString> m_itemDescriptionByItemType;
    QMap<QString, int> m_itemTypeByItemDescription;

    QMap<quint64, CADitem*> m_itemMap;
    CADitemTypes::ItemType m_activeDrawCommandType;
    quint64 m_currentItemId;


    CADitemTypes::ItemType getItemTypeByItemDescription(QString description);
    CADitem* getItemById(quint64 id);

    int getNumberOfItemTypes();

    void network_getAll_processLayers(LayerList layers, QByteArray *answer);
    void network_getAll_processItems(QList<CADitem*> items, QByteArray *answer);

    void file_loadDB_parseDomElement(QDomElement elem, Layer *layer, bool mapByDescription, QMap<int, QString> *itemDescriptionByItemType, QString *error);
    void file_storeDB_processLayers(QDomDocument doc, QDomElement parentElement, LayerList layers);
    void file_storeDB_processItems(QDomDocument doc, QDomElement parentElement, QList<CADitem*> items);

    Layer *m_rootLayer;

signals:
    void signal_layerAdded(Layer *layer, Layer *parentLayer);
    void signal_layerChanged(Layer *layer);
    void signal_layerMoved(Layer *layer);
    void signal_layerDeleted(Layer *layer);
    void signal_itemAdded(CADitem *item, Layer *layer);
    void signal_itemDeleted(CADitem *item);
    void signal_layerManagerUpdateNeeded();
    void signal_repaintNeeded();
    void signal_itemModified(CADitem *item);
    void signal_dbStatusModified();
    void signal_dbStatusSafe();
};

#endif // ITEMDB_H
