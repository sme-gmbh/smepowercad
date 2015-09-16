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

#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QColorDialog>
#include <QDesktopWidget>
#include <QDockWidget>
#include <QInputDialog>
#include <QLabel>
#include <QMap>
#include <QMenu>
#include <QMessageBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>

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
    void updateLayer(Layer* layer);
    Layer* getCurrentLayer();
    bool isSoloActive();

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

    void updateSoloActive();
    void updateSoloActive_processLayers(QList<Layer*> layers);

public slots:
    void slot_updateAllLayers();
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
