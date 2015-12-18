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

#include "layermanager.h"
#include "ui_layermanager.h"

Q_LOGGING_CATEGORY(layermanager, "powercard.layermanager")

LayerManager::LayerManager(ItemDB *itemDb, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::LayerManager),
    m_itemDb(itemDb),
    m_currentLayer(NULL)
{
    ui->setupUi(this);

    this->setStyleSheet(StylesheetProvider::getInstance()->getStylesheet("LayerManager"));

    QDesktopWidget desktopWidget;
    QRect rightScreenRect = desktopWidget.screenGeometry(desktopWidget.numScreens() -1);
    if (desktopWidget.screenCount() > 1) {
        this->resize(this->width(), rightScreenRect.height());
        this->move(rightScreenRect.topLeft());
    }

    ui->treeView_layer->setItemDelegate(new BackgroundItemDelegate(this));
    ui->treeView_layer->setModel(m_itemDb);
    ui->treeView_layer->setColumnWidth(1, 24);
    ui->treeView_layer->setColumnWidth(2, 24);
    ui->treeView_layer->setColumnWidth(3, 24);
    ui->treeView_layer->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->treeView_layer->header()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->treeView_layer->header()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->treeView_layer->header()->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->treeView_layer->header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->treeView_layer->header()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    ui->treeView_layer->header()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    ui->treeView_layer->header()->setSectionResizeMode(7, QHeaderView::ResizeToContents);

    m_menuNoItem = new QMenu(this);
    m_menuNoItem->addAction(tr("New layer"), this, SLOT(slot_appendNewLayer()));

    m_menuOnItem = new QMenu(this);
    m_menuOnItem->addAction(tr("Edit name"), this, SLOT(slot_editLayerName()));
    m_menuOnItem->addAction(tr("Edit line width"), this, SLOT(slot_editLayerLineWidth()));
    m_menuOnItem->addAction(tr("Edit line type"), this, SLOT(slot_editLayerLineType()));
    m_menuOnItem->addSeparator();
    m_menuOnItem->addAction(tr("Append new Layer"), this, SLOT(slot_appendNewLayer()));
    m_menuOnItem->addAction(tr("Append new Layer as child"), this, SLOT(slot_appendNewLayerAsChild()));
    m_menuOnItem->addAction(tr("Delete layer"), this, SLOT(slot_deleteLayer()));
}

LayerManager::~LayerManager()
{
    delete ui;
}

Layer* LayerManager::getCurrentLayer()
{
    if (!m_currentLayer)
        return m_itemDb->getRootLayer();

    return m_currentLayer;
}

void LayerManager::updateLayer(Layer *layer)
{
//    QModelIndex index;
//    QModelIndexList indexList = m_itemDb->match(m_itemDb->index(0, 0, QModelIndex()), Qt::DisplayRole, layer->name, 1, Qt::MatchRecursive);
//    if (indexList.count() >= 1)
//        index = indexList.first();
//    else
//        index = m_itemDb->index(0, 0, QModelIndex());

    //    emit m_itemDb->dataChanged(index, index);
}

void LayerManager::updateSoloActive()
{
    m_itemDb->layerSoloActive = false;
    updateSoloActive_processLayers(m_itemDb->getRootLayer()->getChildLayers());
    if (m_itemDb->layerSoloActive) {
        setWindowTitle(tr("Layermanager [solo active]"));
    } else {
        setWindowTitle(tr("Layermanager"));
    }
}

void LayerManager::updateSoloActive_processLayers(LayerList layers)
{
    foreach (Layer *layer, layers) {
        if (layer->solo)
            m_itemDb->layerSoloActive = true;
        updateSoloActive_processLayers(layer->getChildLayers());
    }
}

void LayerManager::slot_editLayerName(Layer *layer)
{
   if (!layer) layer = m_layerAtContextMenuRequest;

   QString layerName;
   bool ok;
   do {
       layerName = QInputDialog::getText(this, tr("Rename layer %1").arg(layer->name),
                                         tr("Layer name"), QLineEdit::Normal, layer->name, &ok);

       if (!ok) break;

       if (layerName.isEmpty())
           QMessageBox::warning(this, tr("New layer"), tr("Layer name cannot be empty"));
   } while (layerName.isEmpty());

   if (ok) layer->name = layerName;
}

void LayerManager::slot_editLayerLineWidth(Layer *layer)
{
    if (!layer) layer = m_layerAtContextMenuRequest;

    int lineWidth = QInputDialog::getInt(this, tr("Edit line width of layer %1").arg(layer->name),
                                         tr("New width (pixel)"), layer->lineWidth, 1, 10);
    layer->lineWidth = lineWidth;
}

void LayerManager::slot_editLayerLineType(Layer *layer)
{
    if (!layer) layer = m_layerAtContextMenuRequest;

    QStringList items = QStringList();
    for (int i = 0; i < layer->metaEnum_lineType.keyCount(); i++) {
        items.append(layer->metaEnum_lineType.key(i));
    }
    Layer::LineType lineType = (Layer::LineType)items.indexOf(
                QInputDialog::getItem(this, tr("Edit line type of layer %1").arg(layer->name),
                                      tr("New Linetype"), items, layer->lineType, false));
    layer->lineType = lineType;
}

void LayerManager::slot_appendNewLayer()
{
    QString layerName;
    bool alreadyExists = false;
    bool ok;

    do {
        if (alreadyExists)
            QMessageBox::warning(this, tr("New layer"), tr("Layer name already in use! Try a different name!"));

        if (layerName.isEmpty() || alreadyExists) {
            do {
                layerName = QInputDialog::getText(this, tr("New layer"), tr("Layer name"), QLineEdit::Normal, NULL, &ok);

                if (!ok) break;
                if (layerName.isEmpty())
                    QMessageBox::warning(this, tr("New layer"), tr("Layer name cannot be empty"));
            } while(layerName.isEmpty());
        }

        if (!ok) break;
    } while (m_itemDb->findLayerByName(layerName) != NULL && (alreadyExists = true));

    if (ok) {
        m_itemDb->insertLayer(layerName, m_itemDb->parent(m_indexAtContextMenuRequest),
                              m_layerAtContextMenuRequest->parentLayer()->childCount());
    }
}

void LayerManager::slot_appendNewLayerAsChild()
{
    QString layerName;
    bool alreadyExists = false;
    bool ok;

    do {
        if (alreadyExists)
            QMessageBox::warning(this, tr("New layer"), tr("Layer name already in use! Try a different name!"));

        if (layerName.isEmpty() || alreadyExists) {
            do {
                layerName = QInputDialog::getText(this, tr("New layer"), tr("Layer name"), QLineEdit::Normal, NULL, &ok);

                if (!ok) break;
                if (layerName.isEmpty())
                    QMessageBox::warning(this, tr("New layer"), tr("Layer name cannot be empty"));
            } while(layerName.isEmpty());
        }

        if (!ok) break;
    } while (m_itemDb->findLayerByName(layerName) != NULL && (alreadyExists = true));

    if (ok) {
        m_itemDb->insertLayer(layerName, m_indexAtContextMenuRequest,
                              m_layerAtContextMenuRequest->childCount());
        ui->treeView_layer->expand(m_indexAtContextMenuRequest);
    }
}

void LayerManager::slot_deleteLayer()
{
    int ret = QMessageBox::question(this, tr("Delete layer"),
                                    tr("Do you want to delete layer %1").arg(m_layerAtContextMenuRequest->name),
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    if (ret != QMessageBox::Yes) return;

    bool success = m_itemDb->deleteLayerAt(m_itemDb->parent(m_indexAtContextMenuRequest),
                                           m_indexAtContextMenuRequest);
    if (!success)
        QMessageBox::warning(this, tr("Delete layer"), tr("Unable to delete layer. Maybe it's not empty."));
}

void LayerManager::on_treeView_layer_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->treeView_layer->indexAt(pos);
    m_indexAtContextMenuRequest = index;
    m_layerAtContextMenuRequest = static_cast<Layer*>(index.internalPointer());

    if (index.isValid()) {
        m_menuOnItem->popup(QCursor::pos());
    } else {
        m_menuNoItem->popup(QCursor::pos());
    }
}

void LayerManager::on_treeView_layer_activated(const QModelIndex &index)
{
    m_currentLayer = static_cast<Layer*>(index.internalPointer());
}

void LayerManager::on_treeView_layer_clicked(const QModelIndex &index)
{
    Layer *layer = static_cast<Layer*>(index.internalPointer());
    m_currentLayer = layer;

    int col = index.column();
    if (col == 1) { // Ein
        layer->isOn = !layer->isOn;
    } else if (col == 2) { // R/W
        layer->isWriteable = !layer->isWriteable;
    } else if (col == 3) { // Solo
        layer->solo = !layer->solo;
        updateSoloActive();
    } else if (col == 4) { // Brush
        QColorDialog colorDialog(layer->brush.color(), this);
        colorDialog.setWindowTitle(tr("Fillcolor of layer %1").arg(layer->name));
        colorDialog.setModal(true);
        colorDialog.setOption(QColorDialog::ShowAlphaChannel, true);
        if (colorDialog.exec() == QDialog::Rejected) {
            return;
        }
        layer->brush.setColor(colorDialog.currentColor());
    } else if (col == 5) { // Pen
        QColorDialog colorDialog(layer->pen.color(), this);
        colorDialog.setWindowTitle(tr("Linecolor of layer %1").arg(layer->name));
        colorDialog.setModal(true);
        colorDialog.setOption(QColorDialog::ShowAlphaChannel, true);
        if (colorDialog.exec() == QDialog::Rejected) {
            return;
        }
        layer->pen.setColor(colorDialog.currentColor());
    }

    emit ui->treeView_layer->dataChanged(index, index);

    // TODO: updateLayer(layer);
    emit signal_repaintNeeded();
}

void LayerManager::on_treeView_layer_doubleClicked(const QModelIndex &index)
{
    Layer *layer = static_cast<Layer*>(index.internalPointer());

    int col = index.column();
    if (col == 0) { // name
        slot_editLayerName(layer);
    } else if (col == 6) { // line width
        slot_editLayerLineWidth(layer);
    } else if (col == 7) { // line type
        slot_editLayerLineType(layer);
    }

    emit ui->treeView_layer->dataChanged(index, index);
}

void LayerManager::on_treeView_layer_expanded(const QModelIndex &index)
{
    Q_UNUSED(index)
    ui->treeView_layer->resizeColumnToContents(0);
}

void LayerManager::slot_updateAllLayers()
{
}

void LayerManager::slot_layerAdded(Layer *newLayer, Layer *parentLayer)
{
//    updateLayer(newLayer);
}

void LayerManager::slot_layerChanged(Layer *layer)
{
//    updateLayer(layer);
}

void LayerManager::slot_layerDeleted(Layer *layer)
{
//    updateLayer(layer);
}
