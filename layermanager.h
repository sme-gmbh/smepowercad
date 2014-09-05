#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QDockWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDesktopWidget>
#include <QColorDialog>
#include <QMap>
#include <QLabel>
#include "layer.h"

namespace Ui {
class LayerManager;
}

class LayerManager : public QDockWidget
{
    Q_OBJECT

public:
    explicit LayerManager(QWidget *parent, Layer *topLevelLayer);
    ~LayerManager();
    void updateAllLayers();
    void updateLayer(Layer* layer);

private:
    Ui::LayerManager *ui;
    Layer* topLevelLayer;
    QMap<Layer*, QTreeWidgetItem*> layerMap;

public slots:
    void slot_layerAdded(Layer* newLayer, Layer* parentLayer);
private slots:
    void on_treeWidget_layer_itemClicked(QTreeWidgetItem *item, int column);

signals:
    void signal_repaintNeeded();
};

#endif // LAYERMANAGER_H
