#include "itemdb.h"

ItemDB::ItemDB(QObject *parent) :
    QObject(parent)
{
    currentItemId = 0;
    topLevelLayer = new Layer(this);
    topLevelLayer->name = "$$ToplevelLayer";
    layers.append(topLevelLayer);

    this->activeDrawCommand = CADitem::None;
}

Layer* ItemDB::addLayer(QString layerName, QString parentLayerName)
{
    // First: check if layer already exists
    if (layerMap.contains(layerName))
        return layerMap.value(layerName);

    // Second: Find parent layer
    Layer* parentLayer = getLayerByName(parentLayerName);
    if (parentLayer == NULL)
        parentLayer = topLevelLayer;

    return addLayer(layerName, parentLayer);
}

Layer *ItemDB::addLayer(QString layerName, Layer *parentLayer)
{
    if (parentLayer == NULL)
        return NULL;

    // First: check if layer already exists
    if (layerMap.contains(layerName))
        return layerMap.value(layerName);

    // Insert Layer in quickfind-map
    Layer* newLayer = new Layer(this);
    newLayer->name = layerName;
    newLayer->parentLayer = parentLayer;
    parentLayer->subLayers.append(newLayer);
    layerMap.insert(layerName, newLayer);
    emit signal_layerAdded(newLayer, parentLayer);
    return newLayer;
}

bool ItemDB::moveLayer(QString layerName, QString newParentLayerName, quint32 position)
{
    Layer* layer = getLayerByName(layerName);
    if (layer == NULL)
        return false;
    if (layer == topLevelLayer)
        return false;

    Layer* oldParentLayer = layer->parentLayer;
    if (oldParentLayer == NULL)
        return false;

    Layer* newParentLayer = getLayerByName(newParentLayerName);
    if (newParentLayer == NULL)
        return false;




    oldParentLayer->subLayers.removeOne(layer);
    newParentLayer->subLayers.insert(position, layer);



    emit signal_layerMoved(layer);
    return true;
}

bool ItemDB::renameLayer(QString layerName, QString newLayerName)
{
    Layer* layer = getLayerByName(layerName);
    if (layer == NULL)
        return false;
    if (layer == topLevelLayer)
        return false;

    return renameLayer(layer, newLayerName);
}

bool ItemDB::renameLayer(Layer *layer, QString newLayerName)
{
    if (layer == NULL)
        return false;
    if (newLayerName.isEmpty())
        return false;

    layerMap.remove(layer->name);
    layer->name = newLayerName;
    layerMap.insert(layer->name, layer);
    emit signal_layerChanged(layer);
    return true;
}

bool ItemDB::deleteLayer(Layer *layer)
{
    Layer* parentLayer = layer->parentLayer;
    if (parentLayer == NULL)
        return false;

    if (!layer->isEmpty())
        return false;

    if (parentLayer->subLayers.removeOne(layer))
    {
        layerMap.remove(layer->name);
        delete layer;
        emit signal_layerDeleted(layer);
        return true;
    }
    else
        return false;
}

ItemDB::~ItemDB()
{
    // TODO delete all layers incl. sublayers
}

Layer* ItemDB::getLayerByName(QString layerName)
{
    if (layerName.isEmpty())
        return topLevelLayer;
    else
        return layerMap.value(layerName, NULL);
}

Layer* ItemDB::getTopLevelLayer()
{
    return topLevelLayer;
}

bool ItemDB::isLayerValid(Layer *layer)
{
    if (layerMap.values().contains(layer))
        return true;
    else
        return false;
}

void ItemDB::addItem(CADitem* item, QString LayerName)
{
    Layer* layer = getLayerByName(LayerName);
    if (layer == NULL)
        layer = topLevelLayer;

    this->addItem(item, layer);
}

void ItemDB::addItem(CADitem *item, Layer *layer)
{
    if (layer == NULL)
    {
        qDebug() << "ItemDB::addItem(): layer is NULL.";
        return;
    }

    item->layerName = layer->name;
    item->id = currentItemId;
    itemMap.insert(item->id, item);
    currentItemId++;
    layer->items.append(item);
    emit signal_itemAdded(item, layer);
}

void ItemDB::deleteItem(CADitem *item)
{
    Layer* layer = getLayerByName(item->layerName);

    layer->items.removeOne(item);
    itemMap.remove(item->id);

    foreach (CADitem* subItem, item->subItems)
    {
        deleteItem(subItem);
    }

    delete item;
}

bool ItemDB::deleteItem(quint64 id)
{
    CADitem* item = getItemById(id);

    if (item == NULL)
        return false;

    deleteItem(item);
    return true;
}

void ItemDB::deleteItems(QList<CADitem *> items)
{
    foreach (CADitem* item, items)
        deleteItem(item);
}

bool ItemDB::changeLayerOfItem(CADitem *item, Layer *newLayer)
{
    if (item == NULL)
        return false;
    if (newLayer == NULL)
        return false;

    Layer* oldLayer = getLayerByName(item->layerName);
    if (oldLayer == NULL)
        return false;

    oldLayer->items.removeOne(item);
    item->layerName = newLayer->name;
    newLayer->items.append(item);
    emit signal_repaintNeeded();
    return true;
}

bool ItemDB::changeLayerOfItem(quint64 id, QString newLayerName)
{
    CADitem* item = getItemById(id);
    Layer* newLayer = getLayerByName(newLayerName);
    return changeLayerOfItem(item, newLayer);
}

CADitem* ItemDB::drawItem(Layer* layer, CADitem::ItemType type)
{
    if (layer == NULL)
    {
        qDebug() << "ItemDB::drawItem(): layer is NULL.";
        return NULL;
    }

    this->activeDrawCommand = type;
    CADitem* newItem = NULL;

    switch (type)
    {



    case CADitem::Basic_Arc:
        newItem = new CAD_basic_arc();
        break;
    case CADitem::Basic_Box:
        newItem = new CAD_basic_box();
        break;
    case CADitem::Basic_Cylinder:
        newItem = new CAD_basic_cylinder();
        break;
    case CADitem::Basic_Circle:
        newItem = new CAD_basic_circle();
        break;
    case CADitem::Basic_Duct:
        newItem = new CAD_basic_duct();
        break;
    case CADitem::Basic_Face:
        newItem = new CAD_basic_3Dface();
        break;
    case CADitem::Basic_Line:
        newItem = new CAD_basic_line();
        break;
    case CADitem::Basic_Pipe:
        newItem = new CAD_basic_pipe();
        break;
    case CADitem::Basic_Plane:
        newItem = new CAD_basic_plane();
        break;
    case CADitem::Basic_Point:
        newItem = new CAD_basic_point();
        break;
    case CADitem::Basic_Polyline:
        newItem = new CAD_basic_polyline();
        break;
    case CADitem::Basic_Sphere:
        newItem = new CAD_basic_sphere();
        break;
    case CADitem::Basic_Turn:
        newItem = new CAD_basic_turn();
        break;




    case CADitem::Arch_Beam:
        newItem = new CAD_arch_beam();
        break;
    case CADitem::Arch_BlockOut:
        newItem = new CAD_arch_blockOut();
        break;
    case CADitem::Arch_BoredPile:
        newItem = new CAD_arch_boredPile();
        break;
    case CADitem::Arch_Door:
        newItem = new CAD_arch_door();
        break;
    case CADitem::Arch_Foundation:
        newItem = new CAD_arch_foundation();
        break;
    case CADitem::Arch_Grating:
        newItem = new CAD_arch_grating();
        break;
    case CADitem::Arch_LevelSlab:
        newItem = new CAD_arch_levelSlab();
        break;
    case CADitem::Arch_Support:
        newItem = new CAD_arch_support();
        break;
    case CADitem::Arch_Wall_loadBearing:
        newItem = new CAD_arch_wall_loadBearing();
        break;
    case CADitem::Arch_Wall_nonLoadBearing:
        newItem = new CAD_arch_wall_nonLoadBearing();
        break;
    case CADitem::Arch_Window:
        newItem = new CAD_arch_window();
        break;

    case CADitem::Air_Duct:
        newItem = new CAD_air_duct();
        break;
    case CADitem::Air_Pipe:
        newItem = new CAD_air_pipe();
        break;
    case CADitem::Air_DuctFireResistant:
        newItem = new CAD_air_ductFireResistant();
        break;
    case CADitem::Air_DuctTurn:
        newItem = new CAD_air_ductTurn();
        break;
    case CADitem::Air_PipeTurn:
        newItem = new CAD_air_pipeTurn();
        break;
    case CADitem::Air_PipeReducer:
        newItem = new CAD_air_pipeReducer();
        break;
    case CADitem::Air_PipeTeeConnector:
        newItem = new CAD_air_pipeTeeConnector();
        break;
    case CADitem::Air_DuctTeeConnector:
        newItem = new CAD_air_ductTeeConnector();
        break;
    case CADitem::Air_DuctTransition:
        newItem = new CAD_air_ductTransition();
        break;
    case CADitem::Air_DuctTransitionRectRound:
        newItem = new CAD_air_ductTransitionRectRound();
        break;
    case CADitem::Air_DuctYpiece:
        newItem = new CAD_air_ductYpiece();
        break;
    case CADitem::Air_DuctEndPlate:
        newItem = new CAD_air_ductEndPlate();
        break;
    case CADitem::Air_PipeEndCap:
        newItem = new CAD_air_pipeEndCap();
        break;
    case CADitem::Air_ThrottleValve:
        newItem = new CAD_air_throttleValve();
        break;
    case CADitem::Air_MultiLeafDamper:
        newItem = new CAD_air_multiLeafDamper();
        break;
    case CADitem::Air_PressureReliefDamper:
        newItem = new CAD_air_pressureReliefDamper();
        break;
    case CADitem::Air_PipeFireDamper:
        newItem = new CAD_air_pipeFireDamper();
        break;
    case CADitem::Air_DuctFireDamper:
        newItem = new CAD_air_ductFireDamper();
        break;
    case CADitem::Air_DuctVolumetricFlowController:
        newItem = new CAD_air_ductVolumetricFlowController();
        break;
    case CADitem::Air_PipeVolumetricFlowController:
        newItem = new CAD_air_pipeVolumetricFlowController();
        break;
    case CADitem::Air_HeatExchangerWaterAir:
        newItem = new CAD_air_heatExchangerWaterAir();
        break;
    case CADitem::Air_HeatExchangerAirAir:
        newItem = new CAD_air_heatExchangerAirAir();
        break;
    case CADitem::Air_CanvasFlange:
        newItem = new CAD_air_canvasFlange();
        break;
    case CADitem::Air_Filter:
        newItem = new CAD_air_filter();
        break;
    case CADitem::Air_PipeSilencer:
        newItem = new CAD_air_pipeSilencer();
        break;
    case CADitem::Air_DuctBaffleSilencer:
        newItem = new CAD_air_ductBaffleSilencer();
        break;
    case CADitem::Air_Fan:
        newItem = new CAD_air_fan();
        break;
    case CADitem::Air_Humidifier:
        newItem = new CAD_air_humidifier();
        break;
    case CADitem::Air_EmptyCabinet:
        newItem = new CAD_air_emptyCabinet();
        break;
    case CADitem::Air_EquipmentFrame:
        newItem = new CAD_air_equipmentFrame();
        break;

    case CADitem::HeatCool_Adjustvalve:
        newItem = new CAD_heatcool_adjustvalve();
        break;
    case CADitem::HeatCool_Chiller:
        newItem = new CAD_heatcool_chiller();
        break;
    case CADitem::HeatCool_Controlvalve:
        newItem = new CAD_heatcool_controlvalve();
        break;
    case CADitem::HeatCool_CoolingTower:
        newItem = new CAD_heatcool_coolingTower();
        break;
    case CADitem::HeatCool_HeatExchanger:
        newItem = new CAD_heatcool_heatExchanger();
        break;
    case CADitem::HeatCool_Pipe:
        newItem = new CAD_heatcool_pipe();
        break;
    case CADitem::HeatCool_Pump:
        newItem = new CAD_heatcool_pump();
        break;
    case CADitem::HeatCool_Sensor:
        newItem = new CAD_heatcool_sensor();
        break;
    case CADitem::HeatCool_PipeTurn:
        newItem = new CAD_heatcool_pipeTurn();
        break;
    case CADitem::HeatCool_PipeReducer:
        newItem = new CAD_heatcool_pipeReducer();
        break;
    case CADitem::HeatCool_PipeTeeConnector:
        newItem = new CAD_heatcool_pipeTeeConnector();
        break;
    case CADitem::HeatCool_PipeEndCap:
        newItem = new CAD_heatcool_pipeEndCap();
        break;
    case CADitem::HeatCool_Flange:
        newItem = new CAD_heatcool_flange();
        break;
    case CADitem::HeatCool_ExpansionChamber:
        newItem = new CAD_heatcool_expansionChamber();
        break;
    case CADitem::HeatCool_Boiler:
        newItem = new CAD_heatcool_boiler();
        break;
    case CADitem::HeatCool_WaterHeater:
        newItem = new CAD_heatcool_waterHeater();
        break;
    case CADitem::HeatCool_StorageBoiler:
        newItem = new CAD_heatcool_storageBoiler();
        break;
    case CADitem::HeatCool_Radiator:
        newItem = new CAD_heatcool_radiator();
        break;
    case CADitem::HeatCool_Filter:
        newItem = new CAD_heatcool_filter();
        break;
    case CADitem::HeatCool_BallValve:
        newItem = new CAD_heatcool_ballValve();
        break;
    case CADitem::HeatCool_ButterflyValve:
        newItem = new CAD_heatcool_butterflyValve();
        break;
    case CADitem::HeatCool_SafetyValve:
        newItem = new CAD_heatcool_safetyValve();
        break;
    case CADitem::HeatCool_Flowmeter:
        newItem = new CAD_heatcool_flowmeter();
        break;

    case CADitem::Sprinkler_CompressedAirWaterContainer:
        newItem = new CAD_sprinkler_compressedAirWaterContainer();
        break;
    case CADitem::Sprinkler_Distribution:
        newItem = new CAD_sprinkler_distribution();
        break;
    case CADitem::Sprinkler_Head:
        newItem = new CAD_sprinkler_head();
        break;
    case CADitem::Sprinkler_Pipe:
        newItem = new CAD_sprinkler_pipe();
        break;
    case CADitem::Sprinkler_Pump:
        newItem = new CAD_sprinkler_pump();
        break;
    case CADitem::Sprinkler_TeeConnector:
        newItem = new CAD_sprinkler_teeConnector();
        break;
    case CADitem::Sprinkler_Valve:
        newItem = new CAD_sprinkler_valve();
        break;
    case CADitem::Sprinkler_WetAlarmValve:
        newItem = new CAD_sprinkler_wetAlarmValve();
        break;
    case CADitem::Sprinkler_ZoneCheck:
        newItem = new CAD_sprinkler_zoneCheck();
        break;
    case CADitem::Sprinkler_PipeTurn:
        newItem = new CAD_sprinkler_pipeTurn();
        break;
    case CADitem::Sprinkler_PipeEndCap:
        newItem = new CAD_sprinkler_pipeEndCap();
        break;
    case CADitem::Sprinkler_PipeReducer:
        newItem = new CAD_sprinkler_pipeReducer();
        break;

    case CADitem::Electrical_Cabinet:
        newItem = new CAD_electrical_cabinet();
        break;
    case CADitem::Electrical_CableTray:
        newItem = new CAD_electrical_cableTray();
        break;
    default:
    {
        qDebug() << "ItemDB::drawItem(): unknown item type.";
        return NULL;
    }
        break;
    }

    this->addItem(newItem, layer);

    return newItem;
}

CADitem *ItemDB::drawItem(QString layerName, CADitem::ItemType type)
{
    Layer* layer = getLayerByName(layerName);
    if (layer == NULL)
        layer = topLevelLayer;
    return this->drawItem(layer, type);
}

CADitem *ItemDB::getItemById(quint64 id)
{
    return itemMap.value(id, NULL);
}

bool ItemDB::modifyItem(quint64 &id, QString &key, QString &value)
{
    CADitem* item = getItemById(id);
    if (item == NULL)
        return false;

    QVariant oldValue = item->wizardParams.value(key);

    if (!oldValue.isValid())
        return false;

    switch (oldValue.type())
    {
    case QVariant::Double:
        item->wizardParams.insert(key, QVariant(value.toDouble()));
        break;
    case QVariant::String:
        item->wizardParams.insert(key, value);
        break;
    default:
        return false;
        break;
    }

    item->processWizardInput();
    item->calculate();
    return true;
}

void ItemDB::itemAdded(CADitem *item)
{

}

void ItemDB::itemModified(CADitem *item)
{

}

void ItemDB::itemDeleted(CADitem *item)
{

}

QByteArray ItemDB::network_newLayer(QMap<QString, QString> data)
{
    QString newLayerName = data.value("newLayer");
    if (getLayerByName(newLayerName) != NULL)
        return "Error: Layer already exists.\n";

    QString parentLayerName = data.value("parentLayer");

    Layer* newLayer = addLayer(newLayerName, parentLayerName);
    // tbd: set layer properties
    return "Ok\n";  // tbd: Broadcast response
}

QByteArray ItemDB::network_modifyLayer(QMap<QString, QString> data)
{
    QString layerName = data.value("Layer");
    Layer* layer = getLayerByName(layerName);
    if ((layer == NULL) || (layer == topLevelLayer))
        return "Error: Layer does not exist. Unable to modify it.\n";

    QByteArray answer;
    bool repaintNeeded = false;

    if (data.contains("pen"))
    {
        QColor color_pen;
        color_pen = QColor(data.value("pen"));
        layer->pen.setColor(color_pen);
        repaintNeeded = true;
    }
    if (data.contains("brush"))
    {
        QColor color_brush;
        color_brush = QColor(data.value("brush"));
        layer->brush.setColor(color_brush);
        repaintNeeded = true;
    }
    if (data.contains("lineWidth"))
    {
        layer->width = data.value("lineWidth").toInt();
        repaintNeeded = true;
    }
    if (data.contains("lineType"))
    {
        layer->lineType = data.value("lineType");
        repaintNeeded = true;
    }
    if (data.contains("name"))
    {
        bool result = renameLayer(layer, data.value("name"));
        if (result == false)
            answer += "Error: Unable to rename Layer.\n";
    }

    emit signal_layerChanged(layer);
    if (repaintNeeded)
        emit signal_repaintNeeded();

    if (answer.isEmpty())
        answer = "Ok\n";

    return answer;
}

QByteArray ItemDB::network_moveLayer(QMap<QString, QString> data)
{
    QString layerName = data.value("Layer");
    Layer* layer = getLayerByName(layerName);
    if (layer == NULL)
        return "Error: Layer does not exist. Unable to delete it.\n";

    QString newParentLayerName = data.value("newParent");
    quint32 pos = data.value("Pos").toUInt();

    bool result = moveLayer(layerName, newParentLayerName, pos);
    if (result == false)
        return "Error: Unable to move layer.\n";
    else
        return "Ok\n";  // tbd: Broadcast response
}

QByteArray ItemDB::network_deleteLayer(QMap<QString, QString> data)
{
    QString layerName = data.value("Layer");
    Layer* layer = getLayerByName(layerName);
    if (layer == NULL)
        return "Error: Layer does not exist. Unable to delete it.\n";

    bool result = deleteLayer(layer);
    if (result == false)
        return "Error: Unable to delete layer. May be it is not empty.\n";
    else
        return "Ok\n";  // tbd: Broadcast response
}

QByteArray ItemDB::network_getAll()
{
    QByteArray answer;

    network_getAll_processLayers(this->layers, &answer);

    return answer;
}

QByteArray ItemDB::network_getItem(quint64 id)
{
    CADitem* item = getItemById(id);
    if (item == NULL)
        return "Error in network_getItem(" + QByteArray().setNum(id) + ")\n";

    QByteArray answer;

    QList<CADitem*> items;
    items.append(item);

    network_getAll_processItems(items, &answer);

    return answer;
}

QByteArray ItemDB::network_newItem(quint32 type, QMap<QString, QString> data)
{
    QString layerName;
    layerName = data.value("Layer");
    CADitem* newItem = drawItem(layerName, (CADitem::ItemType) type);

    if (newItem == NULL)
        return "Error in network_newItem()\n";

    data.remove("Layer");
    network_modifyItem(newItem->id, data);
    QByteArray answer;
    answer = "N id " + QByteArray().setNum(newItem->id) + "\n";
    return answer;
}

QByteArray ItemDB::network_modifyItem(quint64 id, QMap<QString, QString> data)
{
    QByteArray answer;

    QList<QString> keys = data.keys();

    foreach (QString key, keys)
    {
        QString value = data.value(key);
        bool result = modifyItem(id, key, value);
        if (result == false)
            answer += "Error in modifyItem(" + QByteArray().setNum(id) + " " + key.toUtf8() + " " + value.toUtf8() + ")\n";
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
    if (result == true)
        return "Ok\n";  // tbd: Broadcast response
    else
        return QByteArray("Error in changeLayerOfItem(" + QByteArray().setNum(id) + ", " + newLayerName.toUtf8() + ")\n");
}

QByteArray ItemDB::network_deleteItem(quint64 id)
{
    bool result = deleteItem(id);
    if (result == true)
    {
        emit signal_repaintNeeded();
        return "Ok\n";
    }
    else
        return QByteArray("Error while deleting item ") + QByteArray().setNum(id) + "\n";
}

void ItemDB::network_getAll_processLayers(QList<Layer *> layers, QByteArray* answer)
{
    foreach (Layer* layer, layers)
    {
        layer->serialOut(answer);
        network_getAll_processItems(layer->items, answer);
        network_getAll_processLayers(layer->subLayers, answer);
    }
}

void ItemDB::network_getAll_processItems(QList<CADitem *> items, QByteArray* answer)
{
    foreach (CADitem* item, items)
    {
        item->serialOut(answer);
        network_getAll_processItems(item->subItems, answer);
    }
}
