#include "itemdb.h"

ItemDB::ItemDB(QObject *parent) :
    QObject(parent)
{
    topLevelLayer = new Layer(this);
    topLevelLayer->name = "$$ToplevelLayer";
    layers.append(topLevelLayer);
}

Layer* ItemDB::addLayer(QString layerName, QString parentLayerName)
{
    // First: check if layer already exists
    if (layerMap.contains(layerName))
        return layerMap.value(layerName);

    // Second: Find parent layer
    Layer* parentLayer = getLayerByName(parentLayerName);

    // Insert Layer in quickfind-map
    Layer* newLayer = new Layer(this);
    newLayer->name = layerName;
    parentLayer->subLayers.append(newLayer);
    layerMap.insert(layerName, newLayer);
    emit signal_layerAdded(newLayer, parentLayer);
    return newLayer;
}

ItemDB::~ItemDB()
{
    // TODO delete all layers incl. sublayers
}

Layer* ItemDB::getLayerByName(QString layerName)
{
    if (layerName.isEmpty())
        return topLevelLayer;
    else
        return layerMap.value(layerName, topLevelLayer);
}

Layer* ItemDB::getTopLevelLayer()
{
    return topLevelLayer;
}

void ItemDB::addItem(CADitem* item, QString LayerName)
{
    Layer* layer = getLayerByName(LayerName);

    layer->items.append(item);
}
