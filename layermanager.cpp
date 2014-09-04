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
    this->move(rightScreenRect.topLeft());
    if (desktopWidget.screenCount() > 1) this->resize(this->width(), rightScreenRect.height());
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
    QTreeWidgetItem* item = layerMap.value(layer);
    if (layer->on)
    {
        item->setText(1, "true");
        item->setBackgroundColor(1, QColor(0, 255, 0, 80));
    }
    else
    {
        item->setText(1, "false");
        item->setBackgroundColor(1, QColor(255, 0, 0, 80));
    }

    if (layer->writable)
    {
        item->setText(2, "true");
        item->setBackgroundColor(2, QColor(0, 0, 255, 80));
    }
    else
    {
        item->setText(2, "false");
        item->setBackgroundColor(2, QColor(255, 0, 0, 80));
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
