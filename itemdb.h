#ifndef ITEMDB_H
#define ITEMDB_H

#include <QObject>
#include <QList>

#include "layer.h"

#include "items/cad_basic_line.h"
#include "items/cad_basic_polyline.h"
#include "items/cad_basic_3Dface.h"

class ItemDB : public QObject
{
    Q_OBJECT
public:
    explicit ItemDB(QObject *parent = 0);
    ~ItemDB();
    QList<Layer*> layers;

    Layer *addLayer(QString layerName, QString parentLayerName = QString());
    Layer* getLayerByName(QString layerName);
    Layer* getTopLevelLayer();

    void addItem(CADitem* item, QString LayerName);

private:
    Layer* topLevelLayer;
    QMap<QString, Layer*> layerMap;

signals:
    void signal_layerAdded(Layer* newLayer, Layer* parentLayer);

public slots:

};

#endif // ITEMDB_H
