#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QDockWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDesktopWidget>
#include <QColorDialog>
#include <QMap>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QInputDialog>

#include "layer.h"
#include "itemdb.h"

namespace Ui {
class LayerManager;
}

class LayerManager : public QDockWidget
{
    Q_OBJECT

public:
    explicit LayerManager(QWidget *parent, Layer *topLevelLayer, ItemDB *itemDB);
    ~LayerManager();
    void updateAllLayers();
    void updateLayer(Layer* layer);
    Layer* getCurrentLayer();

private:
    Ui::LayerManager *ui;
    ItemDB* itemDB;
    Layer* topLevelLayer;
    Layer* currentLayer;
    QMap<Layer*, QTreeWidgetItem*> layerMap;

    QPixmap icon_layerOn;
    QPixmap icon_layerOff;
    QPixmap icon_pencilOn;
    QPixmap icon_pencilOff;

    QMenu* menu_noItem;
    QMenu* menu_onItem;
    QTreeWidgetItem* item_atContextMenuRequest;

public slots:
    void slot_layerAdded(Layer* newLayer, Layer* parentLayer);
    void slot_layerChanged(Layer* layer);
    void slot_layerDeleted(Layer* layer);
private slots:
    void on_treeWidget_layer_itemClicked(QTreeWidgetItem *item, int column);

    void on_treeWidget_layer_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_treeWidget_layer_customContextMenuRequested(const QPoint &pos);

    void slot_edit_layerName();
    void slot_edit_layerLineWidth();
    void slot_edit_layerLineType();
    void slot_appendNewLayer();
    void slot_appendNewLayerAsChild();
    void slot_deleteLayer();

    void on_treeWidget_layer_itemExpanded(QTreeWidgetItem *item);

signals:
    void signal_repaintNeeded();
};

#endif // LAYERMANAGER_H
