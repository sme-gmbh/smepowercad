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
#include <QMessageBox>
#include <QDebug>

#include "layer.h"

#include "caditemheaderincludes.h"
#include "caditemtypes.h"


class ItemDB : public QObject
{
    Q_OBJECT

public:
    explicit ItemDB(QObject *parent = 0);
    ~ItemDB();
    QList<Layer*> layers;
    bool layerSoloActive;

    QList<QString> getDomains();
    QList<int> getItemTypesByDomain(QString domain);
    QString getItemDescriptionByItemType(CADitemTypes::ItemType type);
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
    bool file_loadDB(QString filename);
    void file_loadDB_parseDomElement(QDomElement element, Layer* currentLayer);

private:
    QList<QString> domains;
    QMap<QString, int> itemTypesByDomain;
    QMap <int, QString> iconPathByItemType;
    QMap <int, QString> itemDescriptionByItemType;
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
