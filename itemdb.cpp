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

#include "itemdb.h"

Q_LOGGING_CATEGORY(itemdb, "powercad.itemdb")

ItemDB::ItemDB(QObject *parent)
    : QAbstractItemModel(parent),
      m_printscriptTreeModel(new PrintscriptTreeModel(this)),
      m_globalPrintscriptVariables(QMap<QString,QString>()),
      m_currentLayer(NULL)
{
    m_rootLayer = new Layer();
    m_rootLayer->name = "$$ToplevelLayer";
    m_rootLayer->solo = true;
    layerSoloActive = false;

    QPixmap pm = QPixmap(":/ui/layermanager/icons/check.svg").scaledToWidth(20, Qt::SmoothTransformation);
    m_iconLayerOn.addPixmap(pm, QIcon::Normal);
    m_iconLayerOn.addPixmap(pm, QIcon::Selected);

    pm = QPixmap(":/ui/layermanager/icons/hide_layer.svg").scaledToWidth(20, Qt::SmoothTransformation);
    m_iconLayerOff.addPixmap(pm, QIcon::Normal);
    m_iconLayerOff.addPixmap(pm, QIcon::Selected);

    pm = QPixmap(":/ui/layermanager/icons/pencil.svg").scaledToWidth(20, Qt::SmoothTransformation);
    m_iconPencilOn.addPixmap(pm, QIcon::Normal);
    m_iconPencilOn.addPixmap(pm, QIcon::Selected);

    pm = QPixmap(":/ui/layermanager/icons/pencil_off.svg").scaledToWidth(20, Qt::SmoothTransformation);
    m_iconPencilOff.addPixmap(pm, QIcon::Normal);
    m_iconPencilOff.addPixmap(pm, QIcon::Selected);

    m_layerManagerColorIconBG = QPixmap(18, 18);
    m_layerManagerColorIconBG.fill(Qt::white);
    m_currentItemId = 0;
    m_activeDrawCommandType = CADitemTypes::None;
}

ItemDB::~ItemDB()
{
    delete m_rootLayer;
}

QVariant ItemDB::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Layer *layer = static_cast<Layer*>(index.internalPointer());
    int col = index.column();

    if (role == Qt::DisplayRole) {
        return layer->data(col);
    } else if (role == Qt::DecorationRole) {
        bool on = layer->data(col).toBool();

        if (col == 1) return (on ? m_iconLayerOn : m_iconLayerOff);
        else if (col == 2) return (on ? m_iconPencilOn : m_iconPencilOff);
        else if (col == 3) return (on ? m_iconLayerOn : m_iconLayerOff);
        else if (col == 4) {
            QPixmap pm = m_layerManagerColorIconBG.copy();
            QPainter p(&pm);
            p.setBrush(QBrush(layer->brush.color()));
            p.drawRect(1, 1, 15, 15);
            p.end();
            QIcon ic;
            ic.addPixmap(pm, QIcon::Normal);
            ic.addPixmap(pm, QIcon::Selected);
            return ic;
        } else if (col == 5) {
            QPixmap pm = m_layerManagerColorIconBG.copy();
            QPainter p(&pm);
            p.setBrush(QBrush(layer->pen.color()));
            p.drawRect(1, 1, 15, 15);
            p.end();
            QIcon ic;
            ic.addPixmap(pm, QIcon::Normal);
            ic.addPixmap(pm, QIcon::Selected);
            return ic;
        }
    } else if (role == Qt::BackgroundRole) {
        bool on = layer->data(col).toBool();

        if (col == 1) return (on ? QColor(0, 105, 0) : QColor(49, 49, 41));
        else if (col == 2) return (on ? QColor(0, 105, 0) : QColor(49, 49, 41));
        else if (col == 3) return (on ? QColor(200, 200, 0) : QColor(49, 49, 41));
    } else if (role == Qt::UserRole +0) {
        return layer->data(0);
    }
    return QVariant();
}

Qt::ItemFlags ItemDB::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant ItemDB::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) {
            return tr("Layer");
        } else if (section == 3) {
            return tr("S");
        } else if (section == 4) {
            return tr("Fill color");
        } else if (section == 5) {
            return tr("Outline color");
        } else if (section == 6) {
            return tr("Line width");
        } else if (section == 7) {
            return tr("Line type");
        }
    } else if (orientation == Qt::Horizontal && role == Qt::DecorationRole) {
        if (section == 1) {
            return QIcon(":/ui/layermanager/icons/layerOnHeader.svg");
        } else if (section == 2) {
            return QIcon(":/ui/layermanager/icons/layerLockHeader.svg");
        }
    }

    return QVariant();
}

QModelIndex ItemDB::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    Layer *parentLayer;

    if (!parent.isValid())
        parentLayer = m_rootLayer;
    else
        parentLayer = static_cast<Layer*>(parent.internalPointer());

    Layer *childLayer = parentLayer->child(row);
    if (childLayer)
        return createIndex(row, column, childLayer);

    return QModelIndex();
}

QModelIndex ItemDB::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    Layer *childLayer = static_cast<Layer*>(child.internalPointer());
    Layer *parentLayer = childLayer->parentLayer();

    if (parentLayer == m_rootLayer)
        return QModelIndex();

    return createIndex(parentLayer->row(), 0, parentLayer);
}

int ItemDB::rowCount(const QModelIndex &parent) const
{
    Layer *parentLayer;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentLayer = m_rootLayer;
    else
        parentLayer = static_cast<Layer*>(parent.internalPointer());

    return parentLayer->childCount();
}

int ItemDB::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<Layer*>(parent.internalPointer())->columnCount();

    return m_rootLayer->columnCount();
}

LayerList ItemDB::getLayerList()
{
    return m_rootLayer->getAllLayers();
}

Layer* ItemDB::addLayer(QString name, QString parentLayerPath)
{
    Layer *parentLayer = findLayerByPath(parentLayerPath);
    if (!parentLayer)
        parentLayer = m_rootLayer;

    // emit dbstatus modified signal
    return addLayer(name, parentLayer);
}

Layer* ItemDB::addLayer(QString name, Layer *parentLayer)
{
    if (!parentLayer) return NULL;

    // First check if layer already exists
    Layer *layer = findLayerByPath(parentLayer->path(name));
    if (layer)
        return layer;

    QModelIndex parentIndex;
    QModelIndexList indexList = this->match(index(0, 0, QModelIndex()), Qt::DisplayRole, parentLayer->name, 1, Qt::MatchRecursive);
    if (indexList.count() >= 1)
        parentIndex = indexList.first();
    else
        parentIndex = index(0, 0, QModelIndex());

    beginInsertRows(QModelIndex(), parentLayer->childCount(), parentLayer->childCount());
    Layer *newLayer = new Layer(parentLayer);
    endInsertRows();
    newLayer->name = name;

    emit signal_layerAdded(newLayer, parentLayer);
    emit signal_dbStatusModified();
    this->resetInternalData();
    return newLayer;
}

Layer* ItemDB::insertLayer(QString name, const QModelIndex &parent, int at)
{
    Layer *parentLayer = static_cast<Layer*>(parent.internalPointer());

    Layer *newLayer;
    beginInsertRows(parent, at, at);

    if (!parent.isValid())
        newLayer = addLayer(name);
    else
        newLayer = addLayer(name, parentLayer);

    endInsertRows();

    return newLayer;
}

bool ItemDB::moveLayer(Layer *layer, Layer *newParentLayer, quint32 position)
{
    if (!layer)
        return false;

    if (layer == m_rootLayer)
        return false;

    Layer *oldParentLayer = layer->parentLayer();
    if (!oldParentLayer)
        return false;

    if (!newParentLayer)
        return false;

    // Search ModelIndices
    QModelIndex topLevelIndex = this->index(0, 0, QModelIndex());

    QModelIndex index = this->getIndexByLayerPointer(layer, topLevelIndex);
    QModelIndex newParentIndex = this->getIndexByLayerPointer(newParentLayer, topLevelIndex);

    // Do the movement
    return this->moveLayer(index, newParentIndex, position);
}

QModelIndex ItemDB::getIndexByLayerPointer(Layer *layer, QModelIndex parent)
{
    QModelIndex foundIndex = QModelIndex();

    for (int row = 0; this->rowCount(parent); row++)
    {
        QModelIndex currentIndex = parent.child(row, 0);
        Layer *currentLayer = static_cast<Layer*>(currentIndex.internalPointer());

        if (currentLayer == layer)
            return currentIndex;
        else
        {
            foundIndex = this->getIndexByLayerPointer(layer, currentIndex);
            if (foundIndex.isValid())
                return foundIndex;
        }
    }

    return foundIndex;
}

bool ItemDB::moveLayer(QString layerPath, QString newParentLayerPath, quint32 position)
{
    Layer *layer = findLayerByPath(layerPath);
//    if (!layer)
//        return false;

//    if (layer == m_rootLayer)
//        return false;

//    Layer *oldParentLayer = layer->parentLayer();
//    if (!oldParentLayer)
//        return false;

    Layer *newParentLayer = findLayerByPath(newParentLayerPath);
//    if (!newParentLayer)
//        return false;

//    oldParentLayer->removeChild(layer);
//    newParentLayer->insertChild(position, layer);

//    emit signal_layerMoved(layer);
//    emit signal_dbStatusModified();

//    return true;
    return this->moveLayer(layer, newParentLayer, position);
}

bool ItemDB::moveLayer(const QModelIndex &index, QModelIndex newParentIndex, quint32 position)
{
    Layer *layer = static_cast<Layer*>(index.internalPointer());
    Layer *newParentLayer = static_cast<Layer*>(newParentIndex.internalPointer());

    if (layer == NULL)
        return false;

    if (!newParentIndex.isValid()) {
        newParentLayer = m_rootLayer;   // An invalid index means "root index", so set the root layer in that case.
        newParentIndex = QModelIndex();
    }

    Layer *oldParentLayer = layer->parentLayer();
    if (!oldParentLayer)
        return false;

    if (oldParentLayer == newParentLayer && index.row() == (qint32)position)
        return false;

    beginMoveRows(index.parent(), index.row(), index.row(), newParentIndex, position);
    if (oldParentLayer == newParentLayer && index.row() < (qint32)position)
        position--;

    oldParentLayer->removeChild(layer);
    newParentLayer->insertChild(position, layer);
    endMoveRows();

    emit signal_layerMoved(layer);
    emit signal_dbStatusModified();

    return true;
}

bool ItemDB::renameLayer(QString layerPath, QString newLayerName)
{
    Layer *layer = findLayerByPath(layerPath);
    if (!layer)
        return false;

    if (layer == m_rootLayer)
        return false;

    return renameLayer(layer, newLayerName);
}

bool ItemDB::renameLayer(Layer *layer, QString newLayerName)
{
    if (!layer)
        return false;

    if (newLayerName.isEmpty())
        return false;

    layer->name = newLayerName;

    emit signal_layerChanged(layer);
    emit signal_dbStatusModified();

    return true;
}

bool ItemDB::deleteLayer(Layer *layer)
{
    Layer *parentLayer = layer->parentLayer();
    if (!parentLayer)
        return false;

    if (!layer->isEmpty())
        return false;

    if (parentLayer->removeChild(layer)) {
        delete layer;

        emit signal_layerDeleted(layer);
        emit signal_dbStatusModified();

        return true;
    }

    return false;
}

bool ItemDB::deleteLayerAt(const QModelIndex &parent, const QModelIndex &index)
{
    if (!index.isValid())
        return false;

    Layer *layer = static_cast<Layer*>(index.internalPointer());
    Layer *parentLayer = layer->parentLayer();
    if (!parentLayer)
        return false;

    if (!layer->isEmpty())
        return false;

    beginRemoveRows(parent, index.row(), index.row());
    if (parentLayer->removeChild(layer)) {
        delete layer;
        emit signal_layerDeleted(layer);
        emit signal_dbStatusModified();
        endRemoveRows();
        return true;
    }
    endRemoveRows();

    return false;
}

QString ItemDB::getIconPathByItemType(CADitemTypes::ItemType type)
{
    return m_iconPathByItemType.value((int)type);
}

QIcon ItemDB::getIconByItemType(CADitemTypes::ItemType type, QSize size)
{
    QString filename = getIconPathByItemType(type);
    if (QFile(filename).exists()) {
        QSvgRenderer svgRenderer(filename);
        QPixmap pixmap(size);
        QPainter painter(&pixmap);
        svgRenderer.render(&painter);
        return pixmap;
    } else {
        QPixmap pixmap(size);
        pixmap.fill(Qt::yellow);
        return pixmap;
    }
}

QString ItemDB::getItemDescriptionByItemType(CADitemTypes::ItemType type)
{
    return m_itemDescriptionByItemType.value((int)type);
}

QList<int> ItemDB::getItemTypesByDomain(QString domain)
{
    QList<int> values = m_itemTypesByDomain.values(domain);
    qSort(values);
    return values;
}

QStringList ItemDB::getDomains()
{
    return m_domains;
}

Layer *ItemDB::findLayerByName(QString name)
{
    return m_rootLayer->findByName(name);
}

Layer *ItemDB::findLayerByPath(QString path)
{
    return m_rootLayer->findByPath(path);
}

bool ItemDB::isChildOfLayer(Layer *upperLayer, Layer *lowerLayer)
{
    Layer *currentLayer = lowerLayer;

    if (upperLayer == NULL || lowerLayer == NULL)
        return false;

    while (currentLayer != upperLayer)
    {
        currentLayer = currentLayer->parentLayer();
        if (currentLayer == m_rootLayer)
            return false;
    }

    return true;
}

void ItemDB::addItem(CADitem *item, Layer *layer)
{
    if (!layer) {
        qCWarning(itemdb) << "addItem(): layer is NULL";
        return;
    }

    item->setLayer(layer);
    item->setID(m_currentItemId);
    m_itemMap.insert(item->id, item);
    m_currentItemId++;
    layer->addItem(item);
    emit signal_itemAdded(item, layer);
    emit signal_dbStatusModified();
}

void ItemDB::addItem(CADitem *item, QString layerPath)
{
    Layer *layer = findLayerByPath(layerPath);
    if (!layer)
        layer = m_rootLayer;

    addItem(item, layer);
}

void ItemDB::deleteItem(CADitem *item)
{
    Layer *layer = item->layer;
    layer->removeItem(item);
    m_itemMap.remove(item->id);

    foreach (CADitem *subItem, item->subItems) {
        deleteItem(subItem);
    }

    emit signal_itemDeleted(item);
    emit signal_dbStatusModified();

    delete item;
}

bool ItemDB::deleteItem(quint64 id)
{
    CADitem *item = getItemById(id);

    if (!item)
        return false;

    deleteItem(item);

    return true;
}

void ItemDB::deleteItems(QList<CADitem *> items)
{
    foreach (CADitem *item, items)
        deleteItem(item);
}

void ItemDB::deleteItems_withRestorePoint(QList<CADitem *> items)
{
    foreach (CADitem *item, items) {
        restorePoints_undo.append(new RestorePoint(RestorePoint::Restore_ItemDeletion,
                                                   item->layer,
                                                   item->id,
                                                   item->getType(),
                                                   item->wizardParams, item->wizardParams));
        deleteItem(item);
    }
}

bool ItemDB::changeLayerOfItem(CADitem *item, Layer *newLayer)
{
    if (!item)
        return false;

    if (!newLayer)
        return false;

    Layer *oldLayer = item->layer;
    if (!oldLayer)
        return false;

    oldLayer->removeItem(item);
    item->setLayer(newLayer);
    newLayer->addItem(item);

    emit signal_repaintNeeded();
    emit signal_dbStatusModified();

    return true;
}

bool ItemDB::changeLayerOfItem(quint64 id, QString newLayerPath)
{
    CADitem *item = getItemById(id);
    Layer *newLayer = findLayerByPath(newLayerPath);

    return changeLayerOfItem(item, newLayer);
}

bool ItemDB::changeLayerOfItem_withRestorePoint(CADitem *item, Layer *newLayer)
{
    restorePoints_undo.append(new RestorePoint(RestorePoint::Restore_ItemLayerChange,
                                               item->id,
                                               item->layer,
                                               newLayer));
    return changeLayerOfItem(item, newLayer);
}

bool ItemDB::changeLayerOfItems_withRestorePoint(QList<CADitem *> items, Layer *layer)
{

}

CADitem *ItemDB::createItem(CADitemTypes::ItemType type)
{
    CADitem *newItem = NULL;

    switch (type)
    {
    //Air
    case CADitemTypes::Air_Duct:
        newItem = new CAD_air_duct();
        break;
    case CADitemTypes::Air_Pipe:
        newItem = new CAD_air_pipe();
        break;
    case CADitemTypes::Air_DuctFireResistant:
        newItem = new CAD_air_ductFireResistant();
        break;
    case CADitemTypes::Air_DuctTurn:
        newItem = new CAD_air_ductTurn();
        break;
    case CADitemTypes::Air_PipeTurn:
        newItem = new CAD_air_pipeTurn();
        break;
    case CADitemTypes::Air_PipeReducer:
        newItem = new CAD_air_pipeReducer();
        break;
    case CADitemTypes::Air_DuctTeeConnector:
        newItem = new CAD_air_ductTeeConnector();
        break;
    case CADitemTypes::Air_DuctTransitionRectRect:
        newItem = new CAD_air_ductTransitionRectRect();
        break;
    case CADitemTypes::Air_DuctTransitionRectRound:
        newItem = new CAD_air_ductTransitionRectRound();
        break;
    case CADitemTypes::Air_DuctYpiece:
        newItem = new CAD_air_ductYpiece();
        break;
    case CADitemTypes::Air_DuctEndPlate:
        newItem = new CAD_air_ductEndPlate();
        break;
    case CADitemTypes::Air_LinearDiffuser:
        newItem = new CAD_air_lineardiffuser();
        break;
    case CADitemTypes::Air_PipeEndCap:
        newItem = new CAD_air_pipeEndCap();
        break;
    case CADitemTypes::Air_ThrottleValve:
        newItem = new CAD_air_throttleValve();
        break;
    case CADitemTypes::Air_MultiLeafDamper:
        newItem = new CAD_air_multiLeafDamper();
        break;
    case CADitemTypes::Air_PressureReliefDamper:
        newItem = new CAD_air_pressureReliefDamper();
        break;
    case CADitemTypes::Air_PipeFireDamper:
        newItem = new CAD_air_pipeFireDamper();
        break;
    case CADitemTypes::Air_PipeBranch:
        newItem = new CAD_air_pipeBranch();
        break;
    case CADitemTypes::Air_DuctFireDamper:
        newItem = new CAD_air_ductFireDamper();
        break;
    case CADitemTypes::Air_DuctVolumetricFlowController:
        newItem = new CAD_air_ductVolumetricFlowController();
        break;
    case CADitemTypes::Air_PipeVolumetricFlowController:
        newItem = new CAD_air_pipeVolumetricFlowController();
        break;
    case CADitemTypes::Air_HeatExchangerWaterAir:
        newItem = new CAD_air_heatExchangerWaterAir();
        break;
    case CADitemTypes::Air_HeatExchangerAirAir:
        newItem = new CAD_air_heatExchangerAirAir();
        break;
    case CADitemTypes::Air_CanvasFlange:
        newItem = new CAD_air_canvasFlange();
        break;
    case CADitemTypes::Air_Filter:
        newItem = new CAD_air_filter();
        break;
    case CADitemTypes::Air_PipeSilencer:
        newItem = new CAD_air_pipeSilencer();
        break;
    case CADitemTypes::Air_DuctBaffleSilencer:
        newItem = new CAD_air_ductBaffleSilencer();
        break;
    case CADitemTypes::Air_Fan:
        newItem = new CAD_air_fan();
        break;
    case CADitemTypes::Air_Humidifier:
        newItem = new CAD_air_humidifier();
        break;
    case CADitemTypes::Air_EmptyCabinet:
        newItem = new CAD_air_emptyCabinet();
        break;
    case CADitemTypes::Air_EquipmentFrame:
        newItem = new CAD_air_equipmentFrame();
        break;

        //Architecture
    case CADitemTypes::Arch_Beam:
        newItem = new CAD_arch_beam();
        break;
    case CADitemTypes::Arch_BlockOut:
        newItem = new CAD_arch_blockOut();
        break;
    case CADitemTypes::Arch_BoredPile:
        newItem = new CAD_arch_boredPile();
        break;
    case CADitemTypes::Arch_Door:
        newItem = new CAD_arch_door();
        break;
    case CADitemTypes::Arch_Foundation:
        newItem = new CAD_arch_foundation();
        break;
    case CADitemTypes::Arch_Grating:
        newItem = new CAD_arch_grating();
        break;
    case CADitemTypes::Arch_LevelSlab:
        newItem = new CAD_arch_levelSlab();
        break;
    case CADitemTypes::Arch_Support:
        newItem = new CAD_arch_support();
        break;
    case CADitemTypes::Arch_Stairs:
        newItem = new CAD_Arch_Stairs();
        break;
    case CADitemTypes::Arch_Wall_loadBearing:
        newItem = new CAD_arch_wall_loadBearing();
        break;
    case CADitemTypes::Arch_Wall_nonLoadBearing:
        newItem = new CAD_arch_wall_nonLoadBearing();
        break;
    case CADitemTypes::Arch_Window:
        newItem = new CAD_arch_window();
        break;

        //Basic Geometry
    case CADitemTypes::Basic_Arc:
        newItem = new CAD_basic_arc();
        break;
    case CADitemTypes::Basic_Box:
        newItem = new CAD_basic_box();
        break;
    case CADitemTypes::Basic_Cylinder:
        newItem = new CAD_basic_cylinder();
        break;
    case CADitemTypes::Basic_Circle:
        newItem = new CAD_basic_circle();
        break;
    case CADitemTypes::Basic_Duct:
        newItem = new CAD_basic_duct();
        break;
    case CADitemTypes::Basic_Face:
        newItem = new CAD_basic_3Dface();
        break;
    case CADitemTypes::Basic_Flume:
        newItem = new CAD_Basic_Flume();
        break;
    case CADitemTypes::Basic_Gauge:
        newItem = new CAD_Basic_Gauge();
        break;
    case CADitemTypes::Basic_Gauge90Degree:
        newItem = new CAD_Basic_Gauge90Degree();
        break;
    case CADitemTypes::Basic_Hemisphere:
        newItem = new CAD_Basic_Hemisphere();
        break;
    case CADitemTypes::Basic_Line:
        newItem = new CAD_basic_line();
        break;
    case CADitemTypes::Basic_Pipe:
        newItem = new CAD_basic_pipe();
        break;
    case CADitemTypes::Basic_PipeReducer :
        newItem = new CAD_Basic_PipeReducer();
        break;
    case CADitemTypes::Basic_Plane:
        newItem = new CAD_basic_plane();
        break;
    case CADitemTypes::Basic_Point:
        newItem = new CAD_basic_point();
        break;
    case CADitemTypes::Basic_Polyline:
        newItem = new CAD_basic_polyline();
        break;
    case CADitemTypes::Basic_Sphere:
        newItem = new CAD_basic_sphere();
        break;
    case CADitemTypes::Basic_Turn:
        newItem = new CAD_basic_turn();
        break;
    case CADitemTypes::Basic_TorisphericalHeadDIN28011:
        newItem = new CAD_Basic_TorisphericalHeadDIN28011();
        break;
    case CADitemTypes::Basic_TorisphericalHeadDeepDishedDIN28013:
        newItem = new CAD_Basic_TorisphericalHeadDeepDishedDIN28013();
        break;
    case CADitemTypes::Basic_Torus:
        newItem = new CAD_Basic_Torus();
        break;
    case CADitemTypes::Basic_Triangle:
        newItem = new CAD_Basic_Triangle();
        break;


        //Cleanroom
    case CADitemTypes::Cleanroom_CeilingCornerPiece:
        newItem = new CAD_Cleanroom_CeilingCornerPiece();
        break;
    case CADitemTypes::Cleanroom_CeilingFilterFanUnit:
        newItem = new CAD_Cleanroom_CeilingFilterFanUnit();
        break;
    case CADitemTypes::Cleanroom_CeilingGridFeedThrough:
        newItem = new CAD_Cleanroom_CeilingGridFeedThrough();
        break;
    case CADitemTypes::Cleanroom_CeilingGrating:
        newItem = new CAD_Cleanroom_CeilingGrating();
        break;
    case CADitemTypes::Cleanroom_CeilingJoiningKnot:
        newItem = new CAD_Cleanroom_CeilingJoiningKnot();
        break;
    case CADitemTypes::Cleanroom_CeilingMaintenanceFlap:
        newItem = new CAD_Cleanroom_CeilingMaintenanceFlap();
        break;
    case CADitemTypes::Cleanroom_CeilingGrid:
        newItem = new CAD_Cleanroom_CeilingGrid();
        break;
    case CADitemTypes::Cleanroom_CeilingPanel:
        newItem = new CAD_Cleanroom_CeilingPanel();
        break;
    case CADitemTypes::Cleanroom_CeilingSmokeExtractFlap:
        newItem = new CAD_Cleanroom_CeilingSmokeExtractFlap();
        break;
    case CADitemTypes::Cleanroom_CeilingSuspension:
        newItem = new CAD_Cleanroom_CeilingSuspension();
        break;
    case CADitemTypes::Cleanroom_CeilingTeeJoiningPiece:
        newItem = new CAD_Cleanroom_CeilingTeeJoiningPiece();
        break;
    case CADitemTypes::Cleanroom_CeilingVerticalLadder:
        newItem = new CAD_Cleanroom_CeilingVerticalLadder();
        break;
    case CADitemTypes::Cleanroom_ControlButton:
        newItem = new CAD_Cleanroom_ControlButton();
        break;
    case CADitemTypes::Cleanroom_ControlEmergencySwitch:
        newItem = new CAD_Cleanroom_ControlEmergencySwitch();
        break;
    case CADitemTypes::Cleanroom_ControlLedTouchKey:
        newItem = new CAD_Cleanroom_ControlLedTouchKey();
        break;
    case CADitemTypes::Cleanroom_ControlRadarSensor:
        newItem = new CAD_Cleanroom_ControlRadarSensor();
        break;
    case CADitemTypes::Cleanroom_ControlSwitch:
        newItem = new CAD_Cleanroom_ControlSwitch();
        break;
    case CADitemTypes::Cleanroom_DoorFrame:
        newItem = new CAD_Cleanroom_DoorFrame();
        break;
    case CADitemTypes::Cleanroom_DoorSlidingDouble:
        newItem = new CAD_Cleanroom_DoorSlidingDouble();
        break;
    case CADitemTypes::Cleanroom_DoorSlidingSingle:
        newItem = new CAD_Cleanroom_DoorSlidingSingle();
        break;
    case CADitemTypes::Cleanroom_DoorSwingingDouble:
        newItem = new CAD_Cleanroom_DoorSwingingDouble();
        break;
    case CADitemTypes::Cleanroom_DoorSwingingSingle:
        newItem = new CAD_Cleanroom_DoorSwingingSingle();
        break;
    case CADitemTypes::Cleanroom_FloorGrating:
        newItem = new CAD_Cleanroom_FloorGrating();
        break;
    case CADitemTypes::Cleanroom_FloorPanel:
        newItem = new CAD_Cleanroom_FloorPanel();
        break;
    case CADitemTypes::Cleanroom_FloorPanelPerforated:
        newItem = new CAD_Cleanroom_FloorPanelPerforated();
        break;
    case CADitemTypes::Cleanroom_FloorPanelWithBushingRect:
        newItem = new CAD_Cleanroom_FloorPanelWithBushingRect();
        break;
    case CADitemTypes::Cleanroom_FloorPanelWithBushingRound:
        newItem = new CAD_Cleanroom_FloorPanelWithBushingRound();
        break;
    case CADitemTypes::Cleanroom_FloorPanelWithTank:
        newItem = new CAD_Cleanroom_FloorPanelWithTank();
        break;
    case CADitemTypes::Cleanroom_FloorStiffenerDiagonal:
        newItem = new CAD_Cleanroom_FloorStiffenerDiagonal();
        break;
    case CADitemTypes::Cleanroom_FloorStiffenerHorizontal:
        newItem = new CAD_Cleanroom_FloorStiffenerHorizontal();
        break;
    case CADitemTypes::Cleanroom_FloorSupport:
        newItem = new CAD_Cleanroom_FloorSupport();
        break;
    case CADitemTypes::Cleanroom_TagElectricalGrounding:
        newItem = new CAD_Cleanroom_TagElectricalGrounding();
        break;
    case CADitemTypes::Cleanroom_TagLeakageDetector:
        newItem = new CAD_Cleanroom_TagLeakageDetector();
        break;
    case CADitemTypes::Cleanroom_TagSmokeDetector:
        newItem = new CAD_Cleanroom_TagSmokeDetector();
        break;
    case CADitemTypes::Cleanroom_TagFireDetector:
        newItem = new CAD_Cleanroom_TagFireDetector();
        break;
    case CADitemTypes::Cleanroom_VacuumCleanerSocket:
        newItem = new CAD_Cleanroom_VacuumCleanerSocket();
        break;
    case CADitemTypes::Cleanroom_WallOverflowGrate:
        newItem = new CAD_Cleanroom_WallOverflowGrate();
        break;
    case CADitemTypes::Cleanroom_WallPanel:
        newItem = new CAD_Cleanroom_WallPanel();
        break;
    case CADitemTypes::Cleanroom_WallPost:
        newItem = new CAD_Cleanroom_WallPost();
        break;
    case CADitemTypes::Cleanroom_WallSmokeExtractFlap:
        newItem = new CAD_Cleanroom_WallSmokeExtractFlap();
        break;
    case CADitemTypes::Cleanroom_WallStiffenerDiagonal:
        newItem = new CAD_Cleanroom_WallStiffenerDiagonal();
        break;

        //Electrical
    case CADitemTypes::Electrical_BusbarEndFeederUnitDoubleSided:
        newItem = new CAD_Electrical_BusbarEndFeederUnitDoubleSided();
        break;
    case CADitemTypes::Electrical_BusbarEndFeederUnitSingleSided:
        newItem = new CAD_Electrical_BusbarEndFeederUnitSingleSided();
        break;
    case CADitemTypes::Electrical_BusbarTapOffUnit:
        newItem = new CAD_Electrical_BusbarTapOffUnit();
        break;
    case CADitemTypes::Electrical_BusbarWithoutTapoffPoints:
        newItem = new CAD_electrical_busbarwithouttapoffpoints();
        break;
    case CADitemTypes::Electrical_BusbarWithTapoffPoints1Row:
        newItem = new CAD_electrical_busbarwithtapoffpoints1row();
        break;
    case CADitemTypes::Electrical_BusbarWithTapoffPoints2Row:
        newItem = new CAD_electrical_busbarwithtapoffpoints2row();
        break;
    case CADitemTypes::Electrical_Cabinet:
        newItem = new CAD_electrical_cabinet();
        break;
    case CADitemTypes::Electrical_CabinetWithDoorFrontAndBack:
        newItem = new CAD_Electrical_CabinetWithDoorFrontAndBack();
        break;
    case CADitemTypes::Electrical_CabinetWithDoorLeftAndRight:
        newItem = new CAD_Electrical_CabinetWithDoorLeftAndRight();
        break;
    case CADitemTypes::Electrical_CabinetWithoutDoor:
        newItem = new CAD_Electrical_CabinetWithoutDoor();
        break;
    case CADitemTypes::Electrical_Cabletray:
        newItem = new CAD_electrical_cabletray();
        break;
    case CADitemTypes::Electrical_CabletrayCross:
        newItem = new CAD_Electrical_CabletrayCross();
        break;
    case CADitemTypes::Electrical_CabletrayReducer:
        newItem = new CAD_electrical_cabletrayreducer();
        break;
    case CADitemTypes::Electrical_CabletrayTeeConnector:
        newItem = new CAD_electrical_CabletrayTeeconnector();
        break;
    case CADitemTypes::Electrical_CabletrayTransition:
        newItem = new CAD_Electrical_CabletrayTransition();
        break;
    case CADitemTypes::Electrical_CabletrayTurn:
        newItem = new CAD_Electrical_CabletrayTurn();
        break;
    case CADitemTypes::Electrical_CabletrayVerticalLadder:
        newItem = new CAD_Electrical_CabletrayVerticalLadder();
        break;
    case CADitemTypes::Electrical_EquipmentSwitchOrSocket:
        newItem = new CAD_Electrical_EquipmentSwitchOrSocket();
        break;
    case CADitemTypes::Electrical_LuminaireEscapeLighting:
        newItem = new CAD_Electrical_LuminaireEscapeLighting();
        break;
    case CADitemTypes::Electrical_LuminaireRailMounted:
        newItem = new CAD_Electrical_LuminaireRailMounted();
        break;
    case CADitemTypes::Electrical_LuminaireRail:
        newItem = new CAD_Electrical_LuminaireRail();
        break;
    case CADitemTypes::Electrical_LuminaireRecessedMounted:
        newItem = new CAD_Electrical_LuminaireRecessedMounted();
        break;
    case CADitemTypes::Electrical_LuminaireSemicircular:
        newItem = new CAD_Electrical_LuminaireSemicircular();
        break;
    case CADitemTypes::Electrical_LuminaireSurfaceMounted:
        newItem = new CAD_Electrical_LuminaireSurfaceMounted();
        break;
    case CADitemTypes::Electrical_MotorAsynchronous:
        newItem = new CAD_Electrical_MotorAsynchronous();
        break;


//        //Gas
    case CADitemTypes::Gas_ActivatedCarbonAdsorber:
        newItem = new CAD_Gas_ActivatedCarbonAdsorber();
        break;
    case CADitemTypes::Gas_Compressor:
        newItem = new CAD_Gas_Compressor();
        break;
    case CADitemTypes::Gas_DryerDesiccant:
        newItem = new CAD_Gas_DryerDesiccant();
        break;
    case CADitemTypes::Gas_DryerRefrigerant:
        newItem = new CAD_Gas_DryerRefrigerant();
        break;
    case CADitemTypes::Gas_Filter:
        newItem = new CAD_Gas_Filter();
        break;
    case CADitemTypes::Gas_GaugePressure:
        newItem = new CAD_Gas_GaugePressure();
        break;
    case CADitemTypes::Gas_Hose:
        newItem = new CAD_Gas_Hose();
        break;
    case CADitemTypes::Gas_LiquidSeparator:
        newItem = new CAD_Gas_LiquidSeparator();
        break;
    case CADitemTypes::Gas_OilWaterSeparationSystem:
        newItem = new CAD_Gas_OilWaterSeparationSystem();
        break;
    case CADitemTypes::Gas_Pipe:
        newItem = new CAD_Gas_Pipe();
        break;
    case CADitemTypes::Gas_PipeTeeConnector:
        newItem = new CAD_Gas_PipeTeeConnector();
        break;
    case CADitemTypes::Gas_PipeTurn:
        newItem = new CAD_Gas_PipeTurn();
        break;
    case CADitemTypes::Gas_PressureRegulator:
        newItem = new CAD_Gas_PressureRegulator();
        break;
    case CADitemTypes::Gas_Pump:
        newItem = new CAD_Gas_Pump();
        break;
    case CADitemTypes::Gas_QuicklockCoupling:
        newItem = new CAD_Gas_QuicklockCoupling();
        break;
    case CADitemTypes::Gas_SensorFlowrate:
        newItem = new CAD_Gas_SensorFlowrate();
        break;
    case CADitemTypes::Gas_SensorMoisture:
        newItem = new CAD_Gas_SensorMoisture();
        break;
    case CADitemTypes::Gas_Tank:
        newItem = new CAD_Gas_Tank();
        break;
    case CADitemTypes::Gas_ValveBall:
        newItem = new CAD_Gas_ValveBall();
        break;
    case CADitemTypes::Gas_ValveDiaphragm:
        newItem = new CAD_Gas_ValveDiaphragm();
        break;
    case CADitemTypes::Gas_ValveNonReturn:
        newItem = new CAD_Gas_ValveNonReturn();
        break;


        //Heatcool
    case CADitemTypes::HeatCool_32WayValve:
        newItem = new CAD_HeatCool_32WayValve();
        break;
    case CADitemTypes::HeatCool_Adjustvalve:
        newItem = new CAD_heatcool_adjustvalve();
        break;
    case CADitemTypes::HeatCool_BallValve:
        newItem = new CAD_heatcool_ballValve();
        break;
    case CADitemTypes::HeatCool_Boiler:
        newItem = new CAD_heatcool_boiler();
        break;
    case CADitemTypes::HeatCool_ButterflyValveBolted:
        newItem = new CAD_HeatCool_ButterflyValveBolted();
        break;
    case CADitemTypes::HeatCool_ButterflyValveClamped:
        newItem = new CAD_HeatCool_ButterflyValveClamped();
        break;
    case CADitemTypes::HeatCool_Chiller:
        newItem = new CAD_heatcool_chiller();
        break;
    case CADitemTypes::HeatCool_Controlvalve:
        newItem = new CAD_heatcool_controlvalve();
        break;
    case CADitemTypes::HeatCool_CoolingTower:
        newItem = new CAD_heatcool_coolingTower();
        break;
    case CADitemTypes::HeatCool_DirtArrester:
        newItem = new CAD_HeatCool_DirtArrester();
        break;
    case CADitemTypes::HeatCool_ExpansionChamber:
        newItem = new CAD_heatcool_expansionChamber();
        break;
    case CADitemTypes::HeatCool_Filter:
        newItem = new CAD_heatcool_filter();
        break;
    case CADitemTypes::HeatCool_Flange:
        newItem = new CAD_heatcool_flange();
        break;
    case CADitemTypes::HeatCool_Flowmeter:
        newItem = new CAD_heatcool_flowmeter();
        break;
    case CADitemTypes::HeatCool_Gauge:
        newItem = new CAD_HeatCool_Gauge();
        break;
    case CADitemTypes::HeatCool_Gauge90Degree:
        newItem = new CAD_HeatCool_Gauge90Degree();
        break;
    case CADitemTypes::HeatCool_HeatexchangerBolted:
        newItem = new CAD_HeatCool_HeatexchangerBolted();
        break;
    case CADitemTypes::HeatCool_HeatexchangerSoldered:
        newItem = new CAD_HeatCool_HeatexchangerSoldered();
        break;
    case CADitemTypes::HeatCool_NonReturnValve:
        newItem = new CAD_HeatCool_NonReturnValve();
        break;
    case CADitemTypes::HeatCool_NonReturnFlap:
        newItem = new CAD_HeatCool_NonReturnFlap();
        break;
    case CADitemTypes::HeatCool_Pipe:
        newItem = new CAD_heatcool_pipe();
        break;
    case CADitemTypes::HeatCool_PipeEndCap:
        newItem = new CAD_heatcool_pipeEndCap();
        break;
    case CADitemTypes::HeatCool_PipeTurn:
        newItem = new CAD_heatcool_pipeTurn();
        break;
    case CADitemTypes::HeatCool_PipeReducer:
        newItem = new CAD_heatcool_pipeReducer();
        break;
    case CADitemTypes::HeatCool_PipeTeeConnector:
        newItem = new CAD_heatcool_pipeTeeConnector();
        break;
    case CADitemTypes::HeatCool_PumpInline:
        newItem = new CAD_HeatCool_PumpInline();
        break;
    case CADitemTypes::HeatCool_PumpNorm:
        newItem = new CAD_HeatCool_PumpNorm();
        break;
    case CADitemTypes::HeatCool_Radiator:
        newItem = new CAD_heatcool_radiator();
        break;
    case CADitemTypes::HeatCool_RadiatorCompact:
        newItem = new CAD_HeatCool_RadiatorCompact();
        break;
    case CADitemTypes::HeatCool_RadiatorFlange:
        newItem = new CAD_HeatCool_RadiatorFlange();
        break;
    case CADitemTypes::HeatCool_RadiatorFlangeBent:
        newItem = new CAD_HeatCool_RadiatorFlangeBent();
        break;
    case CADitemTypes::HeatCool_RadiatorValve:
        newItem = new CAD_HeatCool_RadiatorValve();
        break;
    case CADitemTypes::HeatCool_SafetyValve:
        newItem = new CAD_heatcool_safetyValve();
        break;
    case CADitemTypes::HeatCool_Sensor:
        newItem = new CAD_heatcool_sensor();
        break;
    case CADitemTypes::HeatCool_StorageBoiler:
        newItem = new CAD_heatcool_storageBoiler();
        break;
    case CADitemTypes::HeatCool_Valve:
        newItem = new CAD_HeatCool_Valve();
        break;
    case CADitemTypes::HeatCool_Valve90Degree:
        newItem = new CAD_HeatCool_Valve90Degree();
        break;
    case CADitemTypes::HeatCool_ValveHandwheel:
        newItem = new CAD_HeatCool_ValveHandwheel();
        break;
    case CADitemTypes::HeatCool_ValveHandwheelGear:
        newItem = new CAD_HeatCool_ValveHandwheelGear();
        break;
    case CADitemTypes::HeatCool_ValveLever:
        newItem = new CAD_HeatCool_ValveLever();
        break;
    case CADitemTypes::HeatCool_ValveMotorRect:
        newItem = new CAD_HeatCool_ValveMotorRect();
        break;
    case CADitemTypes::HeatCool_ValveMotorRound:
        newItem = new CAD_HeatCool_ValveMotorRound();
        break;
    case CADitemTypes::HeatCool_WaterHeater:
        newItem = new CAD_heatcool_waterHeater();
        break;

        //Sanitary
    case CADitemTypes::Sanitary_CleaningPiece:
        newItem = new CAD_Sanitary_CleaningPiece();
        break;
    case CADitemTypes::Sanitary_ElectricWaterHeater:
        newItem = new CAD_Sanitary_ElectricWaterHeater();
        break;
    case CADitemTypes::Sanitary_EmergencyEyeShower:
        newItem = new CAD_Sanitary_EmergencyEyeShower();
        break;
    case CADitemTypes::Sanitary_EmergencyShower:
        newItem = new CAD_Sanitary_EmergencyShower();
        break;
    case CADitemTypes::Sanitary_Flange:
        newItem = new CAD_Sanitary_Flange();
        break;
    case CADitemTypes::Sanitary_LiftingUnit:
        newItem = new CAD_Sanitary_LiftingUnit();
        break;
    case CADitemTypes::Sanitary_Pipe:
        newItem = new CAD_sanitary_pipe();
        break;
    case CADitemTypes::Sanitary_PipeEndCap:
        newItem = new CAD_Sanitary_PipeEndCap();
        break;
    case CADitemTypes::Sanitary_PipeReducer:
        newItem = new CAD_sanitary_pipeReducer();
        break;
    case CADitemTypes::Sanitary_PipeTeeConnector:
        newItem = new CAD_sanitary_pipeTeeConnector();
        break;
    case CADitemTypes::Sanitary_PipeTurn:
        newItem = new CAD_sanitary_pipeTurn();
        break;
    case CADitemTypes::Sanitary_PipeYPiece:
        newItem = new CAD_Sanitary_PipeYPiece();
        break;
    case CADitemTypes::Sanitary_Shower:
        newItem = new CAD_Sanitary_Shower();
        break;
    case CADitemTypes::Sanitary_Sink:
        newItem = new CAD_Sanitary_Sink();
        break;
    case CADitemTypes::Sanitary_WashBasin:
        newItem = new CAD_Sanitary_WashBasin();
        break;

        //Sprinkler
    case CADitemTypes::Sprinkler_CompressedAirWaterContainer:
        newItem = new CAD_sprinkler_compressedAirWaterContainer();
        break;
    case CADitemTypes::Sprinkler_Distribution:
        newItem = new CAD_sprinkler_distribution();
        break;
    case CADitemTypes::Sprinkler_Flange:
        newItem = new CAD_Sprinkler_Flange();
        break;
    case CADitemTypes::Sprinkler_Head:
        newItem = new CAD_sprinkler_head();
        break;
    case CADitemTypes::Sprinkler_Pipe:
        newItem = new CAD_sprinkler_pipe();
        break;
    case CADitemTypes::Sprinkler_Pump:
        newItem = new CAD_sprinkler_pump();
        break;
    case CADitemTypes::Sprinkler_PipeTeeConnector:
        newItem = new CAD_sprinkler_pipeTeeConnector();
        break;
    case CADitemTypes::Sprinkler_Valve:
        newItem = new CAD_sprinkler_valve();
        break;
    case CADitemTypes::Sprinkler_WetAlarmValve:
        newItem = new CAD_sprinkler_wetAlarmValve();
        break;
    case CADitemTypes::Sprinkler_ZoneCheck:
        newItem = new CAD_sprinkler_zoneCheck();
        break;
    case CADitemTypes::Sprinkler_PipeTurn:
        newItem = new CAD_sprinkler_pipeTurn();
        break;
    case CADitemTypes::Sprinkler_PipeEndCap:
        newItem = new CAD_sprinkler_pipeEndCap();
        break;
    case CADitemTypes::Sprinkler_PipeReducer:
        newItem = new CAD_sprinkler_pipeReducer();
        break;

        // Steel
    case CADitemTypes::Steel_BeamI:
        newItem = new CAD_Steel_BeamI();
        break;
    case CADitemTypes::Steel_BeamL:
        newItem = new CAD_Steel_BeamL();
        break;
    case CADitemTypes::Steel_BeamT:
        newItem = new CAD_Steel_BeamT();
        break;
    case CADitemTypes::Steel_BeamU:
        newItem = new CAD_Steel_BeamU();
        break;
    case CADitemTypes::Steel_BeamZ:
        newItem = new CAD_Steel_BeamZ();
        break;


    default:
        qCWarning(itemdb) << "createItem(): unknown item type.";
        return NULL;
    }

    return newItem;

}

CADitem *ItemDB::drawItem(Layer *layer, CADitemTypes::ItemType type)
{
    if (!layer) {
        qCWarning(itemdb) << "drawItem(): layer is NULL";
        return NULL;
    }

    if (type == CADitemTypes::None) {
        qCWarning(itemdb) << "drawItem(): Tried to create a CADitemTypes::None";
        return NULL;
    }

    m_activeDrawCommandType = type;

    CADitem *newItem = createItem(type);
    addItem(newItem, layer);

    return newItem;
}

CADitem *ItemDB::drawItem(QString layerPath, CADitemTypes::ItemType type)
{
    Layer *layer = findLayerByPath(layerPath);
    if (!layer)
        layer = m_rootLayer;

    return drawItem(layer, type);
}

CADitem *ItemDB::drawItem_withRestorePoint(Layer *layer, CADitemTypes::ItemType type, WizardParams wizardParams)
{
    CADitem *item = drawItem(layer, type);
    if (!item)
        return NULL;

    item->wizardParams.insert(wizardParams);
    item->processWizardInput();
    item->calculate();
    restorePoints_undo.append(new RestorePoint(RestorePoint::Restore_ItemCreation,
                                               item->layer,
                                               item->id,
                                               item->getType(),
                                               item->wizardParams, item->wizardParams));

    emit signal_dbStatusModified();

    return item;
}

bool ItemDB::modifyItem(quint64 &id, QString key, QString value)
{
    CADitem *item = getItemById(id);
    if (!item)
        return false;

    QVariant oldValue = item->wizardParams.value(key);

    if (!oldValue.isValid())
        return false;

    switch (oldValue.type()) {
    case QVariant::Double:
        item->wizardParams.insert(key, QVariant(value.toDouble()));
        break;
    case QVariant::String:
        item->wizardParams.insert(key, value);
        break;
    default:
        return false;
    }

    item->processWizardInput();
    item->calculate();
    emit signal_itemModified(item);
    emit signal_dbStatusModified();

    return true;
}

void ItemDB::modifyItem_withRestorePoint(CADitem *item, WizardParams newParams)
{
    if (!item)
        return;

    WizardParams paramsBefore = item->wizardParams;
    item->wizardParams.insert(newParams);
    WizardParams paramsAfter = item->wizardParams;

    restorePoints_undo.append(new RestorePoint(RestorePoint::Restore_WizardParams,
                                               item->layer,
                                               item->id,
                                               item->getType(),
                                               paramsBefore, paramsAfter));

    item->processWizardInput();
    item->calculate();
    emit signal_itemModified(item);
    emit signal_dbStatusModified();
}

void ItemDB::setRestorePoint()
{
    restorePoints_undo.append(new RestorePoint(RestorePoint::Restore_Stoppoint));
    restore_clearRedo();
}

void ItemDB::restore_clearRedo()
{
    qDeleteAll(restorePoints_redo);
    restorePoints_redo.clear();
}

void ItemDB::restore_undo()
{
    RestorePoint *restorePoint;
    emit signal_dbStatusModified();

    forever {
        if (restorePoints_undo.isEmpty()) {
            emit signal_repaintNeeded();
            return;
        }

        restorePoint = restorePoints_undo.takeLast();
        restorePoints_redo.prepend(restorePoint);

        switch (restorePoint->getType()) {
        case RestorePoint::Restore_ItemCreation: {
            deleteItem(restorePoint->itemID);
        }
            break;

        case RestorePoint::Restore_ItemDeletion: {
            quint64 currentItemId_shadow = m_currentItemId;
            m_currentItemId = restorePoint->itemID;
            CADitem *newItem = drawItem(restorePoint->layerBefore, restorePoint->itemType);
            m_currentItemId = currentItemId_shadow;

            newItem->wizardParams = restorePoint->wizardParamsBefore;
            newItem->processWizardInput();
            newItem->calculate();
        }
            break;

        case RestorePoint::Restore_ItemLayerChange: {
            CADitem *item = getItemById(restorePoint->itemID);
            if(!item)
                continue;
            changeLayerOfItem(item, restorePoint->layerBefore);
        }
            break;

        case RestorePoint::Restore_WizardParams: {
            CADitem *item = getItemById(restorePoint->itemID);
            if (!item) continue;
            item->wizardParams.insert(restorePoint->wizardParamsBefore);
            item->processWizardInput();
            item->calculate();
        }
            break;

        case RestorePoint::Restore_Stoppoint: {
            emit signal_repaintNeeded();
            return;
        }
            break;
        }
    }
}

void ItemDB::restore_redo()
{
    RestorePoint *restorePoint;
    emit signal_dbStatusModified();

    if(!restorePoints_redo.isEmpty())
    {
        restorePoint = restorePoints_redo.takeFirst();
        restorePoints_undo.append(restorePoint);
    }

    forever {
        if (restorePoints_redo.isEmpty()) {
            emit signal_repaintNeeded();
            return;
        }

        restorePoint = restorePoints_redo.takeFirst();
        restorePoints_undo.append(restorePoint);

        switch (restorePoint->getType()) {
        case RestorePoint::Restore_ItemCreation: {
            quint64 currentItemId_shadow = m_currentItemId;
            m_currentItemId = restorePoint->itemID;
            CADitem *newItem = drawItem(restorePoint->layerBefore, restorePoint->itemType);
            m_currentItemId = currentItemId_shadow;

            newItem->wizardParams = restorePoint->wizardParamsBefore;
            newItem->processWizardInput();
            newItem->calculate();
        }
            break;

        case RestorePoint::Restore_ItemDeletion: {
            deleteItem(restorePoint->itemID);
        }
            break;

        case RestorePoint::Restore_ItemLayerChange: {
            CADitem *item = getItemById(restorePoint->itemID);
            if(!item)
                continue;
            changeLayerOfItem(item, restorePoint->layerAfter);
        }
            break;

        case RestorePoint::Restore_WizardParams: {
            CADitem *item = getItemById(restorePoint->itemID);
            if (!item) continue;
            item->wizardParams.insert(restorePoint->wizardParamsAfter);
            item->processWizardInput();
            item->calculate();
        }
            break;

        case RestorePoint::Restore_Stoppoint: {
        }
            break;
        }

        if (restorePoints_redo.length() >= 1 && restorePoints_redo.at(0)->getType() == RestorePoint::Restore_Stoppoint) {
            emit signal_repaintNeeded();
            return;
        }
    }
}

QByteArray ItemDB::network_newLayer(QMap<QString, QString> data)
{
    QString newLayerPath = data.value("newLayer");
    if (findLayerByPath(newLayerPath))
        return "Error: Layer already exists.\n";

    QString parentLayerName = data.value("parentLayer");

    Layer *layer = addLayer(newLayerPath, parentLayerName);
    // TODO: set layer properties

    return "Ok\n"; // TODO: Broadcast response
}

QByteArray ItemDB::network_modifyLayer(QMap<QString, QString> data)
{
    QString layerPath = data.value("Layer");
    Layer *layer = findLayerByPath(layerPath);
    if (!layer || layer == m_rootLayer)
        return "Error: Layer does not exists. Unable to modify it.\n";

    QByteArray answer;
    bool repaintNeeded = false;

    if (data.contains("pen")) {
        QColor color = QColor(data.value("pen"));
        layer->pen.setColor(color);
        repaintNeeded = true;
    }
    if (data.contains("brush")) {
        QColor color = QColor(data.value("brush"));
        layer->brush.setColor(color);
        repaintNeeded = true;
    }
    if (data.contains("lineWidth")) {
        layer->lineWidth = data.value("lineWidth").toInt();
        repaintNeeded = true;
    }
    if (data.contains("lineType")) {
        layer->lineType = (Layer::LineType)data.value("lineType").toInt();
        repaintNeeded = true;
    }
    if (data.contains("name")) {
        bool result = renameLayer(layer, data.value("name"));
        if (!result)
            answer += "Error: Unable to rename Layer.\n";
    }

    signal_layerChanged(layer);
    if (repaintNeeded)
        emit signal_repaintNeeded();

    if (answer.isEmpty())
        answer = "Ok\n";

    return answer;

}

QByteArray ItemDB::network_moveLayer(QMap<QString, QString> data)
{
    QString layerPath = data.value("Layer");
    Layer *layer = findLayerByPath(layerPath);
    if (layer == NULL)
        return "Error: Layer does not exist. Unable to move it.\n";

    QString newParentLayerName = data.value("newParent");
    quint32 pos = data.value("Pos").toUInt();

    bool result = moveLayer(layerPath, newParentLayerName, pos);
    if (!result)
        return "Error: Unable to move layer.\n";

    return "Ok\n"; // TODO: broadcast response
}

QByteArray ItemDB::network_deleteLayer(QMap<QString, QString> data)
{
    QString layerPath = data.value("Layer");
    Layer *layer = findLayerByPath(layerPath);
    if (!layer)
        return "Error: Layer does not exist. Unable to delete it.\n";

    bool result = deleteLayer(layer);
    if (!result)
        return "Error: Unable to delete layer. Maybe it's not empty.\n";

    return "Ok\n"; // TODO: broadcast response
}

QByteArray ItemDB::network_getAll()
{
    QByteArray answer;

    network_getAll_processLayers(getLayerList(), &answer);

    return answer;
}

QByteArray ItemDB::network_getItem(quint64 id)
{
    CADitem *item = getItemById(id);
    if (!item)
        return "Error in network_getItem(" + QByteArray().setNum(id) + ")\n";

    QByteArray answer;

    QList<CADitem*> items;
    items.append(item);

    network_getAll_processItems(items, &answer);

    return answer;
}

QByteArray ItemDB::network_newItem(quint32 type, QMap<QString, QString> data)
{
    QString layerName = data.value("Layer");
    CADitem *newItem = drawItem(layerName, (CADitemTypes::ItemType)type);

    if (!newItem)
        return "Error in network_newItem()\n";

    data.remove("Layer");
    network_modifyItem(newItem->id, data);

    QByteArray answer = "N id " + QByteArray::number(newItem->id) + "\n";

    return answer;
}

QByteArray ItemDB::network_modifyItem(quint64 id, QMap<QString, QString> data)
{
    QByteArray answer;

    QMapIterator<QString, QString> it = QMapIterator<QString, QString>(data);

    while (it.hasNext()) {
        it.next();

        bool result = modifyItem(id, it.key(), it.value());
        if (!result)
            answer += "Error in modifyItem(" + QByteArray().setNum(id) + " " + it.key().toUtf8() + " " + it.value().toUtf8() + ")\n";
    }

    emit signal_repaintNeeded();

    if (answer.isEmpty())
        answer = "Ok\n";

    return answer;
}

QByteArray ItemDB::network_changeLayerOfItem(quint64 id, QMap<QString, QString> data)
{
    QString newLayerName = data.value("newLayer");
    bool result = changeLayerOfItem(id, newLayerName);
    if (result)
        return "Ok\n"; // TODO: broadcast response
    else
        return "Error in changeLayerOfItem(" + QByteArray().setNum(id) + ", " + newLayerName.toUtf8() + ")\n";
}

QByteArray ItemDB::network_deleteItem(quint64 id)
{
    bool result = deleteItem(id);
    if (result) {
        emit signal_repaintNeeded();
        return "Ok\n";
    }

    return "Error while deleting item " + QByteArray().setNum(id) + "\n";
}

bool ItemDB::file_storeDB(const QString filename, QMatrix4x4 projectionMatrix, QMatrix4x4 glSelectMatrix, QMatrix4x4 modelviewMatrix, QMatrix4x4 rotationMatrix)
{
    QDomDocument doc;
    QDomElement root = doc.createElement("SmePowerCadProject");
    doc.appendChild(root);
    root.setAttribute("Version", QString("Build %1 %2").arg(QString(__DATE__)).arg(QString(__TIME__)));

    // Store item [type <-> description] list
    QDomElement elem_itemTypeList = doc.createElement("ItemTypeList");
    root.appendChild(elem_itemTypeList);

    int numberOfItemTypes = getNumberOfItemTypes();
    for (int i = 0; i < numberOfItemTypes; i++) {
        QString desc = getItemDescriptionByItemType((CADitemTypes::ItemType)i);
        QDomElement elem_itemType = doc.createElement("ItemType");
        elem_itemTypeList.appendChild(elem_itemType);
        elem_itemType.setAttribute("Type", QString().sprintf("I%i", i));
        elem_itemType.setAttribute("Description", desc);
    }

    // store matrices
    QDomElement elem_matrixProjection = doc.createElement("MatrixProjection");
    root.appendChild(elem_matrixProjection);
    for (int i = 0; i < 4;i++) {
        QDomElement element_column = doc.createElement("Column" + QString::number(i));
        elem_matrixProjection.appendChild(element_column);
        element_column.setAttribute("x", projectionMatrix.column(i).x());
        element_column.setAttribute("y", projectionMatrix.column(i).y());
        element_column.setAttribute("z", projectionMatrix.column(i).z());
        element_column.setAttribute("w", projectionMatrix.column(i).w());
    }

    QDomElement elem_matrixGlselect = doc.createElement("MatrixGLSelect");
    root.appendChild(elem_matrixGlselect);
    for (int i = 0; i < 4;i++) {
        QDomElement element_column = doc.createElement("Column" + QString::number(i));
        elem_matrixGlselect.appendChild(element_column);
        element_column.setAttribute("x", glSelectMatrix.column(i).x());
        element_column.setAttribute("y", glSelectMatrix.column(i).y());
        element_column.setAttribute("z", glSelectMatrix.column(i).z());
        element_column.setAttribute("w", glSelectMatrix.column(i).w());
    }

    QDomElement elem_matrixModelview = doc.createElement("MatrixModelview");
    root.appendChild(elem_matrixModelview);
    for (int i = 0; i < 4;i++) {
        QDomElement element_column = doc.createElement("Column" + QString::number(i));
        elem_matrixModelview.appendChild(element_column);
        element_column.setAttribute("x", modelviewMatrix.column(i).x());
        element_column.setAttribute("y", modelviewMatrix.column(i).y());
        element_column.setAttribute("z", modelviewMatrix.column(i).z());
        element_column.setAttribute("w", modelviewMatrix.column(i).w());
    }

    QDomElement elem_matrixRotation = doc.createElement("MatrixRotation");
    root.appendChild(elem_matrixRotation);
    for (int i = 0; i < 4;i++) {
        QDomElement element_column = doc.createElement("Column" + QString::number(i));
        elem_matrixRotation.appendChild(element_column);
        element_column.setAttribute("x", rotationMatrix.column(i).x());
        element_column.setAttribute("y", rotationMatrix.column(i).y());
        element_column.setAttribute("z", rotationMatrix.column(i).z());
        element_column.setAttribute("w", rotationMatrix.column(i).w());
    }

    // Store printscripts
    QDomElement elem_printscripts = doc.createElement("Printscripts");
    root.appendChild(elem_printscripts);
    file_storeDB_processPrintscriptItem(doc, elem_printscripts, m_printscriptTreeModel->getRootItem()->getChildItems());

    // Store printscript variables
    QDomElement elem_printscriptVars = doc.createElement("PrintscriptVariables");
    root.appendChild(elem_printscriptVars);
    QMapIterator<QString,QString> it(m_globalPrintscriptVariables);
    while (it.hasNext() && (it.next() != NULL)) {
        QDomElement elemVar = doc.createElement("PrintscriptVariable");
        elem_printscriptVars.appendChild(elemVar);
        elemVar.setAttribute("key", it.key());
        elemVar.setAttribute("value", it.value());
    }


    // Store cad data
    QDomElement elem_cad = doc.createElement("CadData");
    root.appendChild(elem_cad);
    file_storeDB_processLayers(doc, elem_cad, m_rootLayer->getChildLayers());

    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly))
        return false;

    QTextStream stream(&file);
    doc.save(stream, 1);   // Indent = 1
    file.close();
    emit signal_dbStatusSafe();
    return true;
}

bool ItemDB::file_loadDB(const QString filename, QString *error, QMatrix4x4 *projectionMatrix, QMatrix4x4 *glSelectMatrix, QMatrix4x4 *modelviewMatrix, QMatrix4x4 *rotationMatrix)
{
    m_printscriptTreeModel->clear();

    qCDebug(itemdb) << "file_loadDB";
    QFile file(filename);

    if (!file.open(QFile::ReadOnly)) {
        *error = file.errorString();
        return false;
    }

    QString errorString = "";
    int errorLine = -1;
    int errorColumn = -1;

    QDomDocument doc;
    if (!doc.setContent(&file, true, &errorString, &errorLine, &errorColumn)) {
        file.close();
        *error = tr("line %1, column %2:\n%3")
                .arg(errorLine)
                .arg(errorColumn)
                .arg(errorString);
        return false;
    }

    QString currentVersion = QString("Build %1 %2").arg(QString(__DATE__)).arg(QString(__TIME__));

    QDomElement root = doc.documentElement();
    if (root.tagName() != "SmePowerCadProject") {
        file.close();
        *error = tr("Root-Node has wrong tag name: %1").arg(root.tagName());
        return false;
    } else if (!root.hasAttribute("Version") || root.attribute("Version").isEmpty()) {
        file.close();
        *error = tr("Version attribute is missing.");
        return false;
    }

    // Version check
    bool mapByDescription = false;
    if (root.attribute("Version") != currentVersion) {
        *error = tr("Old file version: %1\nCurrent version: %2\nConverting file to current version.")
                .arg(root.attribute("Version")).arg(currentVersion);
        mapByDescription = true;
    }

    // Read matrices
    QDomElement elem_column;
    QDomElement elem_projectionMatrix = root.firstChildElement("MatrixProjection");
    for (int i = 0; i < 4; i++) {
        elem_column = elem_projectionMatrix.firstChildElement("Column" + QString::number(i));
        double x = elem_column.attribute("x").toDouble();
        double y = elem_column.attribute("y").toDouble();
        double z = elem_column.attribute("z").toDouble();
        double w = elem_column.attribute("w").toDouble();
        projectionMatrix->setColumn(i, QVector4D(x, y, z, w));
    }
    QDomElement elem_glSelectMatrix = root.firstChildElement("MatrixGLSelect");
    for (int i = 0; i < 4; i++) {
        elem_column = elem_glSelectMatrix.firstChildElement("Column" + QString::number(i));
        double x = elem_column.attribute("x").toDouble();
        double y = elem_column.attribute("y").toDouble();
        double z = elem_column.attribute("z").toDouble();
        double w = elem_column.attribute("w").toDouble();
        glSelectMatrix->setColumn(i, QVector4D(x, y, z, w));
    }
    QDomElement elem_modelviewMatrix = root.firstChildElement("MatrixModelview");
    for (int i = 0; i < 4; i++) {
        elem_column = elem_modelviewMatrix.firstChildElement("Column" + QString::number(i));
        double x = elem_column.attribute("x").toDouble();
        double y = elem_column.attribute("y").toDouble();
        double z = elem_column.attribute("z").toDouble();
        double w = elem_column.attribute("w").toDouble();
        modelviewMatrix->setColumn(i, QVector4D(x, y, z, w));
    }
    QDomElement elem_rotationMatrix = root.firstChildElement("MatrixRotation");
    for (int i = 0; i < 4; i++) {
        elem_column = elem_rotationMatrix.firstChildElement("Column" + QString::number(i));
        double x = elem_column.attribute("x").toDouble();
        double y = elem_column.attribute("y").toDouble();
        double z = elem_column.attribute("z").toDouble();
        double w = elem_column.attribute("w").toDouble();
        rotationMatrix->setColumn(i, QVector4D(x, y, z, w));
    }
    qCDebug(itemdb) << "successfully read matrices";

    // Read Printscripts
    QDomElement elem_printscripts = root.firstChildElement("Printscripts");
    QDomNodeList printscriptGroupsList = elem_printscripts.childNodes();
    PrintscriptTreeItem *tempRootPrintscriptItem = m_printscriptTreeModel->getRootItem();
    for (int i = 0; i < printscriptGroupsList.count(); i++) {
        QDomElement elem_group = printscriptGroupsList.at(i).toElement();
        file_loadDB_parsePrintscript(elem_group, tempRootPrintscriptItem);
    }

    // Read global printscript variables
    QDomNodeList elemVars = root.firstChildElement("PrintscriptVariables").childNodes();
    m_globalPrintscriptVariables.clear();
    for (int i = 0; i < elemVars.count(); i++) {
        QDomElement elemVar = elemVars.at(i).toElement();
        m_globalPrintscriptVariables.insert(elemVar.attribute("key"), elemVar.attribute("value"));
    }

    // Read itemTypeList from file and build map
    QMap<int, QString> itemDescriptionByItemType;
    QDomElement elem_itemTypeList = root.firstChildElement("ItemTypeList");
    QDomElement elem_itemType = elem_itemTypeList.firstChildElement("ItemType");
    while (!elem_itemType.isNull()) {
        QString type = elem_itemType.attribute("Type");
        QString description = elem_itemType.attribute("Description");
        type.remove(0, 1); // Strip 'I'
        itemDescriptionByItemType.insert(type.toInt(), description);
        elem_itemType = elem_itemType.nextSiblingElement("ItemType");
    }
    qCDebug(itemdb) << "successfully read item type list";

    // Read cad data from file
    QDomElement elem_cadData = root.firstChildElement("CadData");
    Layer *tempParentLayer = m_rootLayer;
    while (!elem_cadData.isNull()) {
        file_loadDB_parseDomElement(elem_cadData, tempParentLayer, mapByDescription, &itemDescriptionByItemType, error);
        elem_cadData = elem_cadData.nextSiblingElement();
    }

    file.close();
    qCDebug(itemdb) << "finished reading file";
    emit signal_layerManagerUpdateNeeded();
    emit signal_dbStatusSafe();
    emit finishedReadingFile();
    return true;
}

void ItemDB::deriveDomainsAndItemTypes()
{
    CADitem *item;

    int type = (int)CADitemTypes::None +1;

    forever {
        if (type == CADitemTypes::LastItem)
            break;

        item = createItem((CADitemTypes::ItemType)type);
        if (item == NULL) {
            QString enumName = CADitemTypes().getEnumNameOfItemType((CADitemTypes::ItemType)type);
            qCDebug(itemdb) << "createItem returned NULL; type:" << type << enumName << "not implemented";
            type++;
            continue;
        } else {
            QString enumName = CADitemTypes().getEnumNameOfItemType((CADitemTypes::ItemType)type);
            qCDebug(itemdb) << "deriveDomainsAndItemTypes()" << enumName;
        }

        m_itemTypesByDomain.insertMulti(item->domain(), type);
        m_iconPathByItemType.insert(type, item->iconPath());
        m_itemDescriptionByItemType.insert(type, item->description());
        m_itemTypeByItemDescription.insert(item->description(), type);
        delete item;
        type++;
    }

    m_domains = m_itemTypesByDomain.uniqueKeys();

    qCDebug(itemdb) << "Item type count:" << m_itemTypesByDomain.count();
}

Layer *ItemDB::getRootLayer()
{
    return m_rootLayer;
}

PrintscriptTreeModel *ItemDB::getPrintscriptTreeModel() const
{
    return m_printscriptTreeModel;
}

QMap<QString, QString> ItemDB::getGlobalPrintscriptVariables() const
{
    return m_globalPrintscriptVariables;
}

void ItemDB::removePrintscriptVariable(const QString &key)
{
    m_globalPrintscriptVariables.remove(key);
}

void ItemDB::insertPrintscriptVariable(const QString &key, const QString &value)
{
    m_globalPrintscriptVariables.insert(key, value);
}

CADitemTypes::ItemType ItemDB::getItemTypeByItemDescription(QString description)
{
    return (CADitemTypes::ItemType)m_itemDescriptionByItemType.key(description);
}

CADitem *ItemDB::getItemById(quint64 id)
{
    return m_itemMap.value(id, NULL);
}

int ItemDB::getNumberOfItemTypes()
{
    return m_itemTypesByDomain.count();
}

void ItemDB::network_getAll_processLayers(LayerList layers, QByteArray *answer)
{
    foreach (Layer *layer, layers) {
        layer->serialOut(answer);
        network_getAll_processItems(layer->getItems(), answer);
        network_getAll_processLayers(layer->getChildLayers(), answer);
    }
}

void ItemDB::network_getAll_processItems(QList<CADitem *> items, QByteArray *answer)
{
    foreach (CADitem *item, items) {
        item->serialIn(answer);
        network_getAll_processItems(item->subItems, answer);
    }
}

void ItemDB::file_loadDB_parseDomElement(QDomElement elem, Layer *layer, bool mapByDescription, QMap<int, QString> *itemDescriptionByItemType, QString *error)
{
    QString tagName = elem.tagName();
    if (tagName == "L") {
        Layer *newLayer = this->addLayer(elem.attribute("Name"), layer);
        newLayer->brush.setColor(QColor(elem.attribute("FillColor")));
        newLayer->pen.setColor(QColor(elem.attribute("OutlineColor")));
        newLayer->lineWidth = elem.attribute("LineWidth").toDouble();
        bool lineType_ok;
        int lineType = elem.attribute("LineType").toInt(&lineType_ok);
        if (!lineType_ok)
            lineType = layer->metaEnum_lineType.keyToValue(qUtf8Printable(elem.attribute("LineType")), &lineType_ok);
        if (!lineType_ok)
            lineType = Layer::Undefined;
        newLayer->lineType = (Layer::LineType)lineType;
        layer = newLayer;
    } else if (tagName.startsWith('I')) {
        tagName.remove(0, 1); // Strip 'I'
        int itemType = tagName.toInt();
        if (mapByDescription) {
            QString itemDescription = itemDescriptionByItemType->value(itemType); // get old description by in-file table
            itemType = getItemTypeByItemDescription(itemDescription); // get current item type
            if ((CADitemTypes::ItemType)itemType == CADitemTypes::None)
                error->append("\n" + tr("Unable to resolve name: %1").arg(itemDescription));
        }
        CADitem *item = drawItem(layer, (CADitemTypes::ItemType)itemType);
        if (!item) {
            error->append("\n" + tr("ItemDB::file_loadDB_parseDomElement(): Got a NULL item."));
            return;
        }

        foreach (QString key, item->wizardParams.keys()) {
            QString elementKey = key;
            elementKey.replace(' ', '_');

            switch (item->wizardParams.value(key).type()) {
            case QVariant::String:
                item->wizardParams.insert(key, elem.attribute(elementKey));
                break;
            case QVariant::Int:
                item->wizardParams.insert(key, elem.attribute(elementKey).toInt());
                break;
            case QVariant::Double:
                item->wizardParams.insert(key, elem.attribute(elementKey).toDouble());
                break;
            case QVariant::StringList: {
                QString value = elem.attribute(elementKey);
                QStringList stringList = value.split('#');
                item->wizardParams.insert(key, stringList);
                break;
            }
            default:
                qCDebug(itemdb) << "ItemDB::file_loadDB_parseDomElement(): Unhandled value type:" << item->wizardParams.value(key).type();
            }
        }

        item->processWizardInput();
        item->calculate();
    }

    QDomElement child = elem.firstChildElement();
    while (!child.isNull()) {
        file_loadDB_parseDomElement(child, layer, mapByDescription, itemDescriptionByItemType, error);
        child = child.nextSiblingElement();
    }
}

void ItemDB::file_loadDB_parsePrintscript(QDomElement elem, PrintscriptTreeItem *parentItem)
{
    QString name = elem.attribute("name");

    QDomElement elemPrintscriptContent = elem.firstChildElement("PrintscriptContent");
    if (!elemPrintscriptContent.isNull()) {   // node is Printscript
        QString printscript = elemPrintscriptContent.firstChild().toText().data();
        Printscript *ps = new Printscript(name, printscript, parentItem, this);
        QDomNodeList elemVars = elem.firstChildElement("PrintscriptVariables").childNodes();
        QMap<QString,QString> vars = QMap<QString,QString>();
        for (int i = 0; i < elemVars.count(); i++) {
            QDomElement elemVar = elemVars.at(i).toElement();
            vars.insert(elemVar.attribute("key"), elemVar.attribute("value"));
        }
        ps->insertVariables(vars);
    } else {                            // node is a PrintscriptGroup
        PrintscriptTreeItem *item = new PrintscriptTreeItem(name, parentItem, this);

        QDomNodeList children = elem.childNodes();
        for (int i = 0; i < children.count(); i++) {
            file_loadDB_parsePrintscript(children.at(i).toElement(), item);
        }
    }
}

void ItemDB::file_storeDB_processLayers(QDomDocument doc, QDomElement parentElement, LayerList layers)
{
    foreach (Layer *layer, layers) {
        QDomElement elem = doc.createElement("L");
        parentElement.appendChild(elem);

        elem.setAttribute("Name", layer->name);
        elem.setAttribute("FillColor", layer->brush.color().name());
        elem.setAttribute("OutlineColor", layer->pen.color().name());
        elem.setAttribute("LineWidth", layer->lineWidth);
        elem.setAttribute("LineType", layer->lineType);

        file_storeDB_processLayers(doc, elem, layer->getChildLayers());
        file_storeDB_processItems(doc, elem, layer->getItems());
    }
}

void ItemDB::file_storeDB_processItems(QDomDocument doc, QDomElement parentElement, QList<CADitem *> items)
{
    foreach (CADitem *item, items) {
        QDomElement elem = doc.createElement(QString().sprintf("I%d", (unsigned int)item->getType()));
        parentElement.appendChild(elem);

        foreach (QString key, item->wizardParams.keys()) {
            QVariant value = item->wizardParams.value(key);
            QMetaType::Type type = (QMetaType::Type)value.type();
            if (type == QMetaType::Float)
                type = QMetaType::Double;
            switch (type)
            {
            case QVariant::String:
            case QVariant::Int:
            case QVariant::Double:
                elem.setAttribute(key.replace(' ', '_'), value.toString());
                break;
            case QVariant::StringList:
                elem.setAttribute(key.replace(' ', '_'), value.toStringList().join('#'));
                break;
            default:
                break;
            }
        }
    }
}

void ItemDB::file_storeDB_processPrintscriptItem(QDomDocument &doc, QDomElement &parentElement, QList<PrintscriptTreeItem*> items)
{
    foreach (PrintscriptTreeItem *item, items) {
        QDomElement elem = doc.createElement("PrintscriptItem");
        parentElement.appendChild(elem);
        elem.setAttribute("name", item->name);

        Printscript *ps = dynamic_cast<Printscript*>(item);
        if (ps != NULL) { // object is Printscript
            QDomElement elemPrintscriptCont = doc.createElement("PrintscriptContent");
            QDomText psValue = doc.createTextNode(ps->script);
            elemPrintscriptCont.appendChild(psValue);
            elem.appendChild(elemPrintscriptCont);

            QDomElement elemPrintscriptVars = doc.createElement("PrintscriptVariables");
            elem.appendChild(elemPrintscriptVars);
            QMapIterator<QString,QString> it(ps->getVariables());
            while (it.hasNext() && (it.next() != NULL)) {
                QDomElement elemVar = doc.createElement("PrintscriptVariable");
                elemPrintscriptVars.appendChild(elemVar);
                elemVar.setAttribute("key", it.key());
                elemVar.setAttribute("value", it.value());
            }
        } else {
            file_storeDB_processPrintscriptItem(doc, elem, item->getChildItems());
        }
    }
}

Layer *ItemDB::getCurrentLayer() const
{
    return m_currentLayer;
}

void ItemDB::setCurrentLayer(Layer *currentLayer)
{
    m_currentLayer = currentLayer;
}
