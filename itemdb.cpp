#include "itemdb.h"

ItemDB::ItemDB(QObject *parent) :
    QObject(parent)
{
    currentItemId = 0;
    topLevelLayer = new Layer(this);
    topLevelLayer->name = "$$ToplevelLayer";
    topLevelLayer->solo = true;
    layers.append(topLevelLayer);
    layerSoloActive = false;

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

QString ItemDB::getIconPathByItemType(CADitem::ItemType type)
{
    QString path;

    switch (type)
    {
    case CADitem::Air_CanvasFlange:
    {
        path = ":/icons/cad_air/cad_air_canvasflange.svg";
        break;
    }
    case CADitem::Air_Duct:
    {
        path = ":/icons/cad_air/cad_air_duct.svg";
        break;
    }
    case CADitem::Air_DuctBaffleSilencer:
    {
        path = ":/icons/cad_air/cad_air_ductbafflesilencer.svg";
        break;
    }
    case CADitem::Air_DuctEndPlate:
    {
        path = ":/icons/cad_air/cad_air_ductendplate.svg";
        break;
    }
    case CADitem::Air_DuctFireDamper:
    {
        path = ":/icons/cad_air/cad_air_ductfiredamper.svg";
        break;
    }
    case CADitem::Air_DuctFireResistant:
    {
        path = ":/icons/cad_air/cad_air_ductfireresistant.svg";
        break;
    }
    case CADitem::Air_DuctTeeConnector:
    {
        path = ":/icons/cad_air/cad_air_ductteeconnector.svg";
        break;
    }
    case CADitem::Air_DuctTransition:
    {
        path = ":/icons/cad_air/cad_air_ducttransition.svg";
        break;
    }
    case CADitem::Air_DuctTransitionRectRound:
    {
        path = ":/icons/cad_air/cad_air_ducttransitionrectround.svg";
        break;
    }
    case CADitem::Air_DuctTurn:
    {
        path = ":/icons/cad_air/cad_air_ductturn.svg";
        break;
    }
    case CADitem::Air_DuctVolumetricFlowController:
    {
        path = ":/icons/cad_air/cad_air_ductvolumetricflowcontroller.svg";
        break;
    }
    case CADitem::Air_DuctYpiece:
    {
        path = ":/icons/cad_air/cad_air_ductypiece.svg";
        break;
    }
    case CADitem::Air_EmptyCabinet:
    {
        path = ":/icons/cad_air/cad_air_emptycabinet.svg";
        break;
    }
    case CADitem::Air_EquipmentFrame:
    {
        path = ":/icons/cad_air/cad_air_equipmentframe.svg";
        break;
    }
    case CADitem::Air_Fan:
    {
        path = ":/icons/cad_air/cad_air_fan.svg";
        break;
    }
    case CADitem::Air_Filter:
    {
        path = ":/icons/cad_air/cad_air_filter.svg";
        break;
    }
    case CADitem::Air_HeatExchangerAirAir:
    {
        path = ":/icons/cad_air/cad_air_heatexchangerairair.svg";
        break;
    }
    case CADitem::Air_HeatExchangerWaterAir:
    {
        path = ":/icons/cad_air/cad_air_heatexchangerwaterair.svg";
        break;
    }
    case CADitem::Air_Humidifier:
    {
        path = ":/icons/cad_air/cad_air_humidifier.svg";
        break;
    }
    case CADitem::Air_MultiLeafDamper:
    {
        path = ":/icons/cad_air/cad_air_multileafdamper.svg";
        break;
    }
    case CADitem::Air_Pipe:
    {
        path = ":/icons/cad_air/cad_air_pipe.svg";
        break;
    }
    case CADitem::Air_PipeEndCap:
    {
        path = ":/icons/cad_air/cad_air_pipeendcap.svg";
        break;
    }
    case CADitem::Air_PipeFireDamper:
    {
        path = ":/icons/cad_air/cad_air_pipefiredamper.svg";
        break;
    }
    case CADitem::Air_PipeReducer:
    {
        path = ":/icons/cad_air/cad_air_pipereducer.svg";
        break;
    }
    case CADitem::Air_PipeSilencer:
    {
        path = ":/icons/cad_air/cad_air_pipesilencer.svg";
        break;
    }
    case CADitem::Air_PipeTeeConnector:
    {
        path = ":/icons/cad_air/cad_air_pipeteeconnector.svg";
        break;
    }
    case CADitem::Air_PipeTurn:
    {
        path = ":/icons/cad_air/cad_air_pipeturn.svg";
        break;
    }
    case CADitem::Air_PipeVolumetricFlowController:
    {
        path = ":/icons/cad_air/cad_air_pipevolumetricflowcontroller.svg";
        break;
    }
    case CADitem::Air_PressureReliefDamper:
    {
        path = ":/icons/cad_air/cad_air_pressurereliefdamper.svg";
        break;
    }
    case CADitem::Air_ThrottleValve:
    {
        path = ":/icons/cad_air/cad_air_throttlevalve.svg";
        break;
    }

    case CADitem::Arch_Beam:
    {
        path = ":/icons/cad_arch/cad_arch_beam.svg";
        break;
    }
    case CADitem::Arch_BlockOut:
    {
        path = ":/icons/cad_arch/cad_arch_blockout.svg";
        break;
    }
    case CADitem::Arch_BoredPile:
    {
        path = ":/icons/cad_arch/cad_arch_bored_pile.svg";
        break;
    }
    case CADitem::Arch_Door:
    {
        path = ":/icons/cad_arch/cad_arch_door.svg";
        break;
    }
    case CADitem::Arch_Foundation:
    {
        path = ":/icons/cad_arch/cad_arch_";
        break;
    }
    case CADitem::Arch_Grating:
    {
        path = ":/icons/cad_arch/cad_arch_";
        break;
    }
    case CADitem::Arch_LevelSlab:
    {
        path = ":/icons/cad_arch/cad_arch_levelslab.svg";
        break;
    }
    case CADitem::Arch_Support:
    {
        path = ":/icons/cad_arch/cad_arch_support.svg";
        break;
    }
    case CADitem::Arch_Wall_loadBearing:
    {
        path = ":/icons/cad_arch/cad_arch_wall_loadbearing.svg";
        break;
    }
    case CADitem::Arch_Wall_nonLoadBearing:
    {
        path = ":/icons/cad_arch/cad_arch_wall_nonloadbearing.svg";
        break;
    }
    case CADitem::Arch_Window:
    {
        path = ":/icons/cad_arch/cad_arch_window.svg";
        break;
    }

    case CADitem::Basic_Arc:
    {
        path = ":/icons/cad_basic/cad_basic_arc.svg";
        break;
    }
    case CADitem::Basic_Box:
    {
        path = ":/icons/cad_basic/cad_basic_box.svg";
        break;
    }
    case CADitem::Basic_Circle:
    {
        path = ":/icons/cad_basic/cad_basic_circle.svg";
        break;
    }
    case CADitem::Basic_Cylinder:
    {
        path = ":/icons/cad_basic/cad_basic_cylinder.svg";
        break;
    }
    case CADitem::Basic_Duct:
    {
        path = ":/icons/cad_basic/cad_basic_";
        break;
    }
    case CADitem::Basic_Face:
    {
        path = ":/icons/cad_basic/cad_basic_3Dface.svg";
        break;
    }
    case CADitem::Basic_Line:
    {
        path = ":/icons/cad_basic/cad_basic_line.svg";
        break;
    }
    case CADitem::Basic_Pipe:
    {
        path = ":/icons/cad_basic/cad_basic_pipe.svg";
        break;
    }
    case CADitem::Basic_Plane:
    {
        path = ":/icons/cad_basic/cad_basic_plane.svg";
        break;
    }
    case CADitem::Basic_Point:
    {
        path = ":/icons/cad_basic/cad_basic_point.svg";
        break;
    }
    case CADitem::Basic_Polyline:
    {
        path = ":/icons/cad_basic/cad_basic_polyline.svg";
        break;
    }
    case CADitem::Basic_Sphere:
    {
        path = ":/icons/cad_basic/cad_basic_sphere.svg";
        break;
    }
    case CADitem::Basic_Turn:
    {
        path = ":/icons/cad_basic/cad_basic_turn.svg";
        break;
    }

    case CADitem::Electrical_Cabinet:
    {
        path = ":/icons/cad_electrical/cad_electrical_";
        break;
    }
    case CADitem::Electrical_CableTray:
    {
        path = ":/icons/cad_electrical/cad_electrical_";
        break;
    }

    case CADitem::HeatCool_Adjustvalve:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_adjustvalve.svg";
        break;
    }
    case CADitem::HeatCool_BallValve:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_ballvalve.svg";
        break;
    }
    case CADitem::HeatCool_Boiler:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_boiler.svg";
        break;
    }
    case CADitem::HeatCool_ButterflyValve:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_butterflyvalve.svg";
        break;
    }
    case CADitem::HeatCool_Chiller:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_chiller.svg";
        break;
    }
    case CADitem::HeatCool_Controlvalve:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_controlvalve.svg";
        break;
    }
    case CADitem::HeatCool_CoolingTower:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_coolingtower.svg";
        break;
    }
    case CADitem::HeatCool_ExpansionChamber:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_expansionchamber.svg";
        break;
    }
    case CADitem::HeatCool_Filter:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_filter.svg";
        break;
    }
    case CADitem::HeatCool_Flange:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_flange.svg";
        break;
    }
    case CADitem::HeatCool_Flowmeter:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_flowmeter.svg";
        break;
    }
    case CADitem::HeatCool_HeatExchanger:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_heatexchanger.svg";
        break;
    }
    case CADitem::HeatCool_Pipe:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_pipe.svg";
        break;
    }
    case CADitem::HeatCool_PipeEndCap:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_pipeendcap.svg";
        break;
    }
    case CADitem::HeatCool_PipeReducer:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_pipereducer.svg";
        break;
    }
    case CADitem::HeatCool_PipeTeeConnector:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_pipeteeconnector.svg";
        break;
    }
    case CADitem::HeatCool_PipeTurn:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_pipeturn.svg";
        break;
    }
    case CADitem::HeatCool_Pump:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_pump.svg";
        break;
    }
    case CADitem::HeatCool_Radiator:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_radiator.svg";
        break;
    }
    case CADitem::HeatCool_SafetyValve:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_safetyvalve.svg";
        break;
    }
    case CADitem::HeatCool_Sensor:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_sensor.svg";
        break;
    }
    case CADitem::HeatCool_StorageBoiler:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_storageboiler.svg";
        break;
    }
    case CADitem::HeatCool_WaterHeater:
    {
        path = ":/icons/cad_heatcool/cad_heatcool_waterheater.svg";
        break;
    }

    case CADitem::Sanitary_ElectricWaterHeater:
    {
        path = ":/icons/cad_sanitary/cad_sanitary_";
        break;
    }
    case CADitem::Sanitary_EmergencyEyeShower:
    {
        path = ":/icons/cad_sanitary/cad_sanitary_";
        break;
    }
    case CADitem::Sanitary_EmergencyShower:
    {
        path = ":/icons/cad_sanitary/cad_sanitary_";
        break;
    }
    case CADitem::Sanitary_Flange:
    {
        path = ":/icons/cad_sanitary/cad_sanitary_";
        break;
    }
    case CADitem::Sanitary_LiftingUnit:
    {
        path = ":/icons/cad_sanitary/cad_sanitary_";
        break;
    }
    case CADitem::Sanitary_Pipe:
    {
        path = ":/icons/cad_sanitary/cad_sanitary_";
        break;
    }
    case CADitem::Sanitary_PipeEndCap:
    {
        path = ":/icons/cad_sanitary/cad_sanitary_";
        break;
    }
    case CADitem::Sanitary_PipeReducer:
    {
        path = ":/icons/cad_sanitary/cad_sanitary_";
        break;
    }
    case CADitem::Sanitary_PipeTeeConnector:
    {
        path = ":/icons/cad_sanitary/cad_sanitary_";
        break;
    }
    case CADitem::Sanitary_PipeTurn:
    {
        path = ":/icons/cad_sanitary/cad_sanitary_";
        break;
    }
    case CADitem::Sanitary_Shower:
    {
        path = ":/icons/cad_sanitary/cad_sanitary_";
        break;
    }
    case CADitem::Sanitary_Sink:
    {
        path = ":/icons/cad_sanitary/cad_sanitary_";
        break;
    }
    case CADitem::Sanitary_WashBasin:
    {
        path = ":/icons/cad_sanitary/cad_sanitary_";
        break;
    }

    case CADitem::Sprinkler_CompressedAirWaterContainer:
    {
        path = ":/icons/cad_sprinkler/cad_sprinkler_compressedairwatercontainer.svg";
        break;
    }
    case CADitem::Sprinkler_Distribution:
    {
        path = ":/icons/cad_sprinkler/cad_sprinkler_distribution.svg";
        break;
    }
    case CADitem::Sprinkler_Head:
    {
        path = ":/icons/cad_sprinkler/cad_sprinkler_head.svg";
        break;
    }
    case CADitem::Sprinkler_Pipe:
    {
        path = ":/icons/cad_sprinkler/cad_sprinkler_pipe.svg";
        break;
    }
    case CADitem::Sprinkler_PipeEndCap:
    {
        path = ":/icons/cad_sprinkler/";
        break;
    }
    case CADitem::Sprinkler_PipeReducer:
    {
        path = ":/icons/cad_sprinkler/";
        break;
    }
    case CADitem::Sprinkler_PipeTurn:
    {
        path = ":/icons/cad_sprinkler/cad_sprinkler_pipeturn.svg";
        break;
    }
    case CADitem::Sprinkler_Pump:
    {
        path = ":/icons/cad_sprinkler/cad_sprinkler_pump.svg";
        break;
    }
    case CADitem::Sprinkler_TeeConnector:
    {
        path = ":/icons/cad_sprinkler/cad_sprinkler_teeconnector.svg";
        break;
    }
    case CADitem::Sprinkler_Valve:
    {
        path = ":/icons/cad_sprinkler/cad_sprinkler_valve.svg";
        break;
    }
    case CADitem::Sprinkler_WetAlarmValve:
    {
        path = ":/icons/cad_sprinkler/cad_sprinkler_wetalarmvalve.svg";
        break;
    }
    case CADitem::Sprinkler_ZoneCheck:
    {
        path = ":/icons/cad_sprinkler/cad_sprinkler_zonecheck.svg";
        break;
    }

    default:
        path = "ERROR: unable to resolve icon path in ItemDB::getIconPathByItemType(" + QString ().setNum(type) + ")";
        break;
    }

    return path;
}

QPixmap ItemDB::getIconByItemType(CADitem::ItemType type, QSize size)
{
    QSvgRenderer svgRenderer(getIconPathByItemType(type));
//    qDebug() << getIconPathByItemType(type);
    QPixmap pixmap(size);
    QPainter painter(&pixmap);
    svgRenderer.render(&painter);
    return pixmap;
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

//    item->layerName = layer->name;
    item->setLayer(layer);
    item->setID(currentItemId);
    itemMap.insert(item->id, item);
    currentItemId++;
    layer->items.append(item);
    emit signal_itemAdded(item, layer);
}

void ItemDB::deleteItem(CADitem *item)
{
//    Layer* layer = getLayerByName(item->layerName);
    Layer* layer = item->layer;

    layer->items.removeOne(item);
    itemMap.remove(item->id);

    foreach (CADitem* subItem, item->subItems)
    {
        deleteItem(subItem);
    }

    signal_itemDeleted(item);
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

//    Layer* oldLayer = getLayerByName(item->layerName);
    Layer* oldLayer = item->layer;
    if (oldLayer == NULL)
        return false;

    oldLayer->items.removeOne(item);
//    item->layerName = newLayer->name;
    item->setLayer(newLayer);
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
    case CADitem::Air_PipeBranch:
        newItem = new CAD_air_pipeBranch();
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

bool ItemDB::file_storeDB(QString filename)
{
    QDomDocument document;
    QDomElement root = document.createElement("SmePowerCadProject");
    document.appendChild(root);
    root.setAttribute("Version", QString());    //tbd.

    file_storeDB_processLayers(document, root, this->topLevelLayer->subLayers);

    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly))
        return false;

    QTextStream stream(&file);
    document.save(stream, 1);   // Indent = 1
    file.close();
    return true;
}

void ItemDB::file_storeDB_processLayers(QDomDocument document, QDomElement parentElement, QList<Layer *> layers)
{
    foreach (Layer* layer, layers)
    {
        QDomElement element = document.createElement("L");
        parentElement.appendChild(element);

        element.setAttribute("Name", layer->name);
        element.setAttribute("FillColor", layer->brush.color().name());
        element.setAttribute("OutlineColor", layer->pen.color().name());
        element.setAttribute("LineWidth", layer->width);
        element.setAttribute("LineType", layer->lineType);

        file_storeDB_processLayers(document, element, layer->subLayers);
        file_storeDB_processItems(document, element, layer->items);
    }
}

void ItemDB::file_storeDB_processItems(QDomDocument document, QDomElement parentElement, QList<CADitem *> items)
{
    foreach (CADitem* item, items)
    {
        QDomElement element = document.createElement(QString().sprintf("I%d", (unsigned int)item->getType()));
        parentElement.appendChild(element);

        foreach (QString key, item->wizardParams.keys())
        {
            element.setAttribute(key.replace(' ', '_'), item->wizardParams.value(key).toString());
        }

        // Do not store subitems as they are recovered automatically when loading the parent item
//        file_storeDB_processItems(document, element, item->subItems);
    }
}

bool ItemDB::file_loadDB(QString filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly))
        return false;

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument document;
    if (!document.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
    {
        file.close();
//        QMessageBox::information(this, tr("Error while reading xml-file"),
//                                 tr("line %1, column %2:\n%3")
//                                 .arg(errorLine)
//                                 .arg(errorColumn)
//                                 .arg(errorStr));
        return false;
    }

    QDomElement root = document.documentElement();
    if (root.tagName() != "SmePowerCadProject")
    {
        file.close();
//        QMessageBox::information(this, tr("Error"),
//                                 tr("Root-Node has wrong tagName."));
        return false;
    }
    else if (root.hasAttribute("Version") && root.attribute("Version") != "")
    {
        file.close();
//        QMessageBox::information(this, tr("Error"),
//                                 tr("Invalid file version"));
        return false;
    }

    QDomElement child = root.firstChildElement("");
    while (!child.isNull())
    {
        this->file_loadDB_parseDomElement(child, this->topLevelLayer);  // tbd. toplevellayer may be wrong here...
        child = child.nextSiblingElement();
    }

    file.close();
    return true;
}

void ItemDB::file_loadDB_parseDomElement(QDomElement element, Layer *currentLayer)
{
    QString tagName = element.tagName();
    if (tagName == "L")
    {
        Layer* newLayer = this->addLayer(element.attribute("Name"), currentLayer);
        newLayer->brush.setColor(QColor(element.attribute("FillColor")));
        newLayer->pen.setColor(QColor(element.attribute("OutlineColor")));
        newLayer->width = element.attribute("LineWidth").toDouble();
        newLayer->lineType = element.attribute("LineType");
        currentLayer = newLayer;
    }
    else if (tagName.startsWith('I'))
    {
        tagName.remove(0, 1);   // Strip "I"
        int itemType = tagName.toInt();
        CADitem* item = this->drawItem(currentLayer, (CADitem::ItemType)itemType);
        foreach (QString key, item->wizardParams.keys())
        {
            QString elementKey = key;
            elementKey.replace(' ', '_');

            switch (item->wizardParams.value(key).type())
            {
            case QVariant::String:
                item->wizardParams.insert(key, QString(element.attribute(elementKey)));
                break;
            case QVariant::Int:
                item->wizardParams.insert(key, QString(element.attribute(elementKey)).toInt());
                break;
            case QVariant::Double:
                item->wizardParams.insert(key, QString(element.attribute(elementKey)).toDouble());
                break;
            default:
                qDebug() << "ItemDB::file_loadDB_parseDomElement() Unhandled value type:" << item->wizardParams.value(key).type();
                break;
            }
        }

        item->processWizardInput();
        item->calculate();
    }


    QDomElement child = element.firstChildElement();
    while (!child.isNull())
    {
        this->file_loadDB_parseDomElement(child, currentLayer);
        child = child.nextSiblingElement();
    }
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
