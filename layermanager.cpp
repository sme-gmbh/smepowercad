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
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, newLayer->name);

    if (parentLayer == topLevelLayer)
        ui->treeWidget_layer->addTopLevelItem(item);
    else
    {
        layerMap.value(parentLayer)->addChild(item);
    }

    layerMap.insert(newLayer, item);
    updateLayer(newLayer);
    ui->treeWidget_layer->resizeColumnToContents(0);
}

void LayerManager::updateAllLayers()
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

    item->setText(3, layer->brush.color().name());
    QPixmap colorPixmap(16, 16);
    colorPixmap.fill(layer->brush.color());
    item->setIcon(3, QIcon(colorPixmap));
    item->setText(4, layer->pen.color().name());
    colorPixmap.fill(layer->pen.color());
    item->setIcon(4, QIcon(colorPixmap));
    item->setText(5, QString().sprintf("%i", layer->width));
    item->setText(6, layer->lineType);
}

Layer *LayerManager::getCurrentLayer()
{
    return currentLayer;
}

void LayerManager::on_treeWidget_layer_itemClicked(QTreeWidgetItem *item, int column)
{
    Layer* layer = layerMap.key(item);
    if (layer == NULL)
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
    case 3:     // Fillcolor
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
    case 4:     // Linecolor
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
    case 5:     // Linewidth
    {
        break;
    }
    case 6:     // Linetype
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
    Layer* newCurrentLayer = itemDB->getLayerByName(current->text(0));
    if (newCurrentLayer != NULL)
    {
        this->currentLayer = newCurrentLayer;
        qDebug() << "Selected Layer" << newCurrentLayer->name;
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
    // item db rename -> change layermap!
}

void LayerManager::slot_edit_layerLineWidth()
{

}

void LayerManager::slot_edit_layerLineType()
{

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
