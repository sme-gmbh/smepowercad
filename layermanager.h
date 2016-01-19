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

#include <QLoggingCategory>
#include <QDockWidget>
#include <QDesktopWidget>
#include <QInputDialog>
#include <QColorDialog>
#include <QLabel>
#include <QMap>
#include <QMenu>
#include <QMessageBox>
#include <QObject>

#include "layer.h"
#include "itemdb.h"
#include "treeviewitemdelegate.h"
#include "stylesheetprovider.h"

Q_DECLARE_LOGGING_CATEGORY(layermanager)

namespace Ui {
class LayerManager;
}

class LayerManager : public QDockWidget
{
    Q_OBJECT

public:
    explicit LayerManager(ItemDB *itemDb, QWidget *parent = 0);
    ~LayerManager();

    Layer* getCurrentLayer();

private:
    Ui::LayerManager *ui;
    ItemDB *m_itemDb;

    QMenu *m_menuNoItem;
    QMenu *m_menuOnItem;
    QModelIndex m_indexAtContextMenuRequest;
    Layer *m_layerAtContextMenuRequest;
    Layer *m_currentLayer;

    void updateLayer(Layer *layer);
    void updateSoloActive();
    void updateSoloActive_processLayers(LayerList layers);

private slots:
    void slot_editLayerName(Layer *layer = 0);
    void slot_editLayerLineWidth(Layer *layer = 0);
    void slot_editLayerLineType(Layer *layer = 0);
    void slot_appendNewLayer();
    void slot_appendNewLayerAsChild();
    void slot_deleteLayer();
    void on_treeView_layer_customContextMenuRequested(const QPoint &pos);
    void on_treeView_layer_activated(const QModelIndex &index);
    void on_treeView_layer_clicked(const QModelIndex &index);
    void on_treeView_layer_doubleClicked(const QModelIndex &index);
    void on_treeView_layer_expanded(const QModelIndex &index);    

public slots:
    void slot_updateAllLayers();
    void slot_layerAdded(Layer *newLayer, Layer *parentLayer);
    void slot_layerChanged(Layer *layer);
    void slot_layerDeleted(Layer *layer);

signals:
    void signal_repaintNeeded();
};

#endif // LAYERMANAGER_H
