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

LayerManager::LayerManager(QWidget *parent, Layer* topLevelLayer, ItemDB* itemDB) :
    QDockWidget(parent),
    ui(new Ui::LayerManager)
{
    this->itemDB = itemDB;
    this->topLevelLayer = topLevelLayer;
    this->currentLayer = topLevelLayer;
    this->item_atContextMenuRequest = NULL;
    ui->setupUi(this);
//    this->move(1920, 0);    // tbd.: move to second screen, and not in this quick and dirty way :)

    // Move to right screen
    QDesktopWidget desktopWidget;
    QRect rightScreenRect = desktopWidget.screenGeometry(desktopWidget.numScreens() - 1);
    if (desktopWidget.screenCount() > 1)
    {
        this->resize(this->width(), rightScreenRect.height());
        this->move(rightScreenRect.topLeft());
    }

    ui->treeWidget_layer->setColumnWidth(1, 24);
    ui->treeWidget_layer->setColumnWidth(2, 24);
    ui->treeWidget_layer->setColumnWidth(3, 24);

    icon_layerOn = QPixmap(":/ui/layermanager/icons/check.svg").scaledToWidth(24, Qt::SmoothTransformation);
    icon_layerOff = QPixmap(":/ui/layermanager/icons/hide_layer.svg").scaledToWidth(24, Qt::SmoothTransformation);
    icon_pencilOn = QPixmap(":/ui/layermanager/icons/pencil.svg").scaledToWidth(24, Qt::SmoothTransformation);
    icon_pencilOff = QPixmap(":/ui/layermanager/icons/pencil_off.svg").scaledToWidth(24, Qt::SmoothTransformation);

    menu_noItem = new QMenu(this);
    menu_noItem->addAction(QIcon(), tr("New layer"), this, SLOT(slot_appendNewLayer()));

    menu_onItem = new QMenu(this);
    menu_onItem->addAction(QIcon(), tr("Edit name"), this, SLOT(slot_edit_layerName()));
    menu_onItem->addAction(QIcon(), tr("Edit line width"), this, SLOT(slot_edit_layerLineWidth()));
    menu_onItem->addAction(QIcon(), tr("Edit line type"), this, SLOT(slot_edit_layerLineType()));
    menu_onItem->addSeparator();
    menu_onItem->addAction(QIcon(), tr("Append new Layer"), this, SLOT(slot_appendNewLayer()));
    menu_onItem->addAction(QIcon(), tr("Append new layer as child"), this, SLOT(slot_appendNewLayerAsChild()));
    menu_onItem->addAction(QIcon(), tr("Delete layer"), this, SLOT(slot_deleteLayer()));
}

LayerManager::~LayerManager()
{
    delete ui;
}

void LayerManager::slot_layerAdded(Layer *newLayer, Layer *parentLayer)
{
    QTreeWidgetItem* item;

    if (parentLayer == topLevelLayer)
    {
        QTreeWidgetItem* parentlItem = ui->treeWidget_layer->invisibleRootItem();
        item = new QTreeWidgetItem(parentlItem);
        ui->treeWidget_layer->addTopLevelItem(item);
    }
    else
    {
        QTreeWidgetItem* parentlItem = layerMap.value(parentLayer);
        item = new QTreeWidgetItem(parentlItem);
        layerMap.value(parentLayer)->addChild(item);
    }

    item->setText(0, newLayer->name);

    if (layerMap.isEmpty())
        ui->treeWidget_layer->setCurrentItem(item);
    layerMap.insert(newLayer, item);
    updateLayer(newLayer);
    ui->treeWidget_layer->resizeColumnToContents(0);
}

void LayerManager::slot_layerChanged(Layer *layer)
{
    // First check if we display this layer
    QTreeWidgetItem* item = layerMap.value(layer);
    if (item == NULL)
        return;

    // Then update Treewidget content
    updateLayer(layer);
}

void LayerManager::slot_layerDeleted(Layer *layer)
{
    QTreeWidgetItem* item = layerMap.value(layer);
    layerMap.remove(layer);
    if (item == NULL)
        return;

    QTreeWidgetItem* parentItem = item->parent();
    if (parentItem == NULL)
        parentItem = ui->treeWidget_layer->invisibleRootItem();

    parentItem->removeChild(item);
    delete item;
}

void LayerManager::slot_updateAllLayers()
{
    foreach(Layer* layer, layerMap.keys())
    {
        updateLayer(layer);
    }
    ui->treeWidget_layer->resizeColumnToContents(0);
}

void LayerManager::updateLayer(Layer *layer)
{
    QTreeWidgetItem *item = layerMap.value(layer);
    if (layer->on)
    {
        QLabel *label = static_cast<QLabel *>(ui->treeWidget_layer->itemWidget(item, 1));
        if (label == NULL)
        {
            label = new QLabel();
            ui->treeWidget_layer->setItemWidget(item, 1, label);
        }
        label->setPixmap(icon_layerOn);
        item->setBackgroundColor(1, QColor(0, 105, 0));
    }
    else
    {
        QLabel *label = static_cast<QLabel *>(ui->treeWidget_layer->itemWidget(item, 1));
        if (label == NULL)
        {
            label = new QLabel();
            ui->treeWidget_layer->setItemWidget(item, 1, label);
        }
        label->setPixmap(icon_layerOff);
        item->setBackgroundColor(1, QColor(49, 49, 41));
    }

    if (layer->writable)
    {
        QLabel *label = static_cast<QLabel *>(ui->treeWidget_layer->itemWidget(item, 2));
        if (label == NULL)
        {
            label = new QLabel();
            ui->treeWidget_layer->setItemWidget(item, 2, label);
        }
        label->setPixmap(icon_pencilOn);
        item->setBackgroundColor(2, QColor(0, 105, 0));
    }
    else
    {
        QLabel *label = static_cast<QLabel *>(ui->treeWidget_layer->itemWidget(item, 2));
        if (label == NULL)
        {
            label = new QLabel();
            ui->treeWidget_layer->setItemWidget(item, 2, label);
        }
        label->setPixmap(icon_pencilOff);
        item->setBackgroundColor(2, QColor(49, 49, 41));
    }

    if (layer->solo)
    {
        QLabel *label = static_cast<QLabel *>(ui->treeWidget_layer->itemWidget(item, 3));
        if (label == NULL)
        {
            label = new QLabel();
            ui->treeWidget_layer->setItemWidget(item, 3, label);
        }
        label->setPixmap(icon_layerOn);
        item->setBackgroundColor(3, QColor(200, 200, 0));
    }
    else
    {
        QLabel *label = static_cast<QLabel *>(ui->treeWidget_layer->itemWidget(item, 3));
        if (label == NULL)
        {
            label = new QLabel();
            ui->treeWidget_layer->setItemWidget(item, 3, label);
        }
        label->setPixmap(icon_layerOff);
        item->setBackgroundColor(3, QColor(49, 49, 41));
    }

    item->setText(0, layer->name);
    item->setText(4, layer->brush.color().name());
    QPixmap colorPixmap(16, 16);
    colorPixmap.fill(layer->brush.color());
    item->setIcon(4, QIcon(colorPixmap));
    item->setText(5, layer->pen.color().name());
    colorPixmap.fill(layer->pen.color());
    item->setIcon(5, QIcon(colorPixmap));
    item->setText(6, QString().sprintf("%i", layer->width));
    item->setText(7, layer->lineType);
}

Layer *LayerManager::getCurrentLayer()
{
    return currentLayer;
}

bool LayerManager::isSoloActive()
{
    return itemDB->layerSoloActive;
}

void LayerManager::updateSoloActive()
{
    itemDB->layerSoloActive = false;
    updateSoloActive_processLayers(itemDB->getTopLevelLayer()->subLayers);
    if (itemDB->layerSoloActive)
    {
        this->setWindowTitle(tr("Layermanager") + " [solo active]");
    }
    else
    {
        this->setWindowTitle(tr("Layermanager"));
    }
}

void LayerManager::updateSoloActive_processLayers(QList<Layer *> layers)
{
    foreach (Layer* layer, layers)
    {
        if (layer->solo)
            itemDB->layerSoloActive = true;
        updateSoloActive_processLayers(layer->subLayers);
    }
}

void LayerManager::on_treeWidget_layer_itemClicked(QTreeWidgetItem *item, int column)
{
    Layer* layer = layerMap.key(item);
    if (itemDB->isLayerValid(layer) == false)
        return;

    switch (column)
    {
    case 0:     // Layer
    {
        break;
    }
    case 1:     // Ein
    {
        layer->on = (!layer->on);
        break;
    }
    case 2:     // R/W
    {
        layer->writable = (!layer->writable);
        break;
    }
    case 3:
    {
        layer->solo = (!layer->solo);
        this->updateSoloActive();
        break;
    }
    case 4:     // Fillcolor
    {
        QColorDialog colorDialog(layer->brush.color(), this);
        colorDialog.setWindowTitle(tr("Fillcolor of layer %1").arg(layer->name));
        colorDialog.setModal(true);
        colorDialog.setOption(QColorDialog::ShowAlphaChannel, true);
        if (colorDialog.exec() == QDialog::Rejected)
            return;
        layer->brush.setColor(colorDialog.currentColor());
        break;
    }
    case 5:     // Linecolor
    {
        QColorDialog colorDialog(layer->pen.color(), this);
        colorDialog.setWindowTitle(tr("Linecolor of layer %1").arg(layer->name));
        colorDialog.setModal(true);
        colorDialog.setOption(QColorDialog::ShowAlphaChannel, true);
        if (colorDialog.exec() == QDialog::Rejected)
            return;
        layer->pen.setColor(colorDialog.currentColor());
        break;
    }
    case 6:     // Linewidth
    {
        break;
    }
    case 7:     // Linetype
    {
        break;
    }
    }

    updateLayer(layer);
    emit signal_repaintNeeded();
}

void LayerManager::on_treeWidget_layer_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    if (current == NULL)
    {
        currentLayer = topLevelLayer;
        return;
    }
    Layer* newCurrentLayer = itemDB->getLayerByName(current->text(0));
    if (newCurrentLayer != NULL)
    {
        this->currentLayer = newCurrentLayer;
    }
    else
    {
        qDebug() << "LayerManager::on_treeWidget_layer_currentItemChanged(): newCurrentLayer is NULL.";
    }
}

void LayerManager::on_treeWidget_layer_customContextMenuRequested(const QPoint &pos)
{
    this->item_atContextMenuRequest = ui->treeWidget_layer->itemAt(pos);

    if (this->item_atContextMenuRequest == NULL)
    {
        this->menu_noItem->popup(QCursor::pos());
    }
    else
    {
        this->menu_onItem->popup(QCursor::pos());
    }
}

void LayerManager::slot_edit_layerName()
{
    Layer* layer = layerMap.key(item_atContextMenuRequest);
    QString layerName = QInputDialog::getText(this, tr("Rename Layer"), tr("Layer name"), QLineEdit::Normal, layer->name);
    itemDB->renameLayer(layer, layerName);
}

void LayerManager::slot_edit_layerLineWidth()
{
    Layer* layer = layerMap.key(item_atContextMenuRequest);
    int lineWidth = QInputDialog::getInt(this, tr("Edit line width"), tr("New width (pixel)"), layer->width, 1, 10);
    itemDB->setLayerLineWidth(layer, lineWidth);
}

void LayerManager::slot_edit_layerLineType()
{
    Layer* layer = layerMap.key(item_atContextMenuRequest);
    QString lineType = QInputDialog::getText(this, tr("Linetype of Layer"), tr("New Linetype"), QLineEdit::Normal, layer->lineType);
    itemDB->setLayerLineType(layer, lineType);
}

void LayerManager::slot_appendNewLayer()
{
    QString layerName = QInputDialog::getText(this, tr("New Layer"), tr("Layer name"));
    if (itemDB->getLayerByName(layerName) != NULL)
    {
        QMessageBox::warning(this, tr("New Layer"), tr("Layer name already in use! Try again and choose a different name."));
        return;
    }
    itemDB->addLayer(layerName);
}

void LayerManager::slot_appendNewLayerAsChild()
{
    QString layerName = QInputDialog::getText(this, tr("New Layer"), tr("Layer name"));
    if (itemDB->getLayerByName(layerName) != NULL)
    {
        QMessageBox::warning(this, tr("New Layer"), tr("Layer name already in use! Try again and choose a different name."));
        return;
    }
    itemDB->addLayer(layerName, layerMap.key(item_atContextMenuRequest));
}

void LayerManager::slot_deleteLayer()
{
    bool success = itemDB->deleteLayer(layerMap.key(item_atContextMenuRequest));
    if (!success)
        QMessageBox::warning(this, tr("Delete Layer"), tr("Unable to delete Layer. May be it is not empty..."));
}

void LayerManager::on_treeWidget_layer_itemExpanded(QTreeWidgetItem *item)
{
    Q_UNUSED(item);
    ui->treeWidget_layer->resizeColumnToContents(0);
}
