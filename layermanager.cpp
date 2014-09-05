#include "layermanager.h"
#include "ui_layermanager.h"

LayerManager::LayerManager(QWidget *parent, Layer* topLevelLayer) :
    QDockWidget(parent),
    ui(new Ui::LayerManager)
{
    this->topLevelLayer = topLevelLayer;
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

    icon_layerOn = QPixmap(":/icons/check.svg").scaledToWidth(24, Qt::SmoothTransformation);
    icon_layerOff = QPixmap(":/icons/hide_layer.svg").scaledToWidth(24, Qt::SmoothTransformation);
    icon_pencilOn = QPixmap(":/icons/pencil.svg").scaledToWidth(24, Qt::SmoothTransformation);
    icon_pencilOff = QPixmap(":/icons/pencil_off.svg").scaledToWidth(24, Qt::SmoothTransformation);
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
        colorDialog.setWindowTitle("Fillcolor of layer " + layer->name);
        colorDialog.setStyleSheet("color: rgb(150,150,150);");
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
        colorDialog.setWindowTitle("Linecolor of layer " + layer->name);
        colorDialog.setStyleSheet("color: rgb(150,150,150);");
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
