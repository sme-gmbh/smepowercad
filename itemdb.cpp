#include "itemdb.h"

ItemDB::ItemDB(QObject *parent) :
    QObject(parent)
{
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

    // Insert Layer in quickfind-map
    Layer* newLayer = new Layer(this);
    newLayer->name = layerName;
    parentLayer->subLayers.append(newLayer);
    layerMap.insert(layerName, newLayer);
    emit signal_layerAdded(newLayer, parentLayer);
    return newLayer;
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
        return layerMap.value(layerName, topLevelLayer);
}

Layer* ItemDB::getTopLevelLayer()
{
    return topLevelLayer;
}

void ItemDB::addItem(CADitem* item, QString LayerName)
{
    Layer* layer = getLayerByName(LayerName);

    this->addItem(item, layer);
}

void ItemDB::addItem(CADitem *item, Layer *layer)
{
    if (layer == NULL)
    {
        qDebug() << "ItemDB::addItem(): layer is NULL.";
        return;
    }

    layer->items.append(item);
    emit signal_itemAdded(item, layer);
}

CADitem* ItemDB::drawItem(Layer* layer, CADitem::ItemType type)
{
    if (layer == NULL)
    {
        qDebug() << "ItemDB::drawItem(): layer is NULL.";
        return NULL;
    }

    this->activeDrawCommand = type;
    CADitem* newItem;

    switch (type)
    {
    case CADitem::Basic_Point:
        newItem = new CAD_basic_point();
        break;
    case CADitem::Basic_Line:
        newItem = new CAD_basic_line();
        break;
    case CADitem::Basic_Polyline:
        newItem = CAD_basic_polyline();
        break;
    case CADitem::Basic_Circle:
        newItem = CAD_basic_circle();
        break;
    case CADitem::Basic_Arc:
        newItem = CAD_basic_arc();
        break;
    case CADitem::Basic_Face:
        newItem = CAD_basic_3Dface();
        break;
    case CADitem::Basic_Plane:
        newItem = CAD_basic_plane();
        break;
    case CADitem::Basic_Box:
        newItem = CAD_basic_box();
        break;
    case CADitem::Basic_Cylinder:
        newItem = CAD_basic_cylinder();
        break;
    case CADitem::Basic_Sphere:
        newItem = CAD_basic_sphere();
        break;

    case CADitem::Arch_LevelSlab:
        newItem = CAD_arch_levelSlab();
        break;
    case CADitem::Arch_Wall_loadBearing:
        newItem = CAD_arch_wall_loadBearing();
        break;
    case CADitem::Arch_Wall_nonLoadBearing:
        newItem = CAD_arch_wall_nonLoadBearing();
        break;
    case CADitem::Arch_BlockOut:
        newItem = CAD_arch_blockOut();
        break;
    case CADitem::Arch_Support:
        newItem = CAD_arch_support();
        break;
    case CADitem::Arch_Beam:
        newItem = CAD_arch_beam();
        break;
    case CADitem::Arch_Door:
        newItem = CAD_arch_door();
        break;
    case CADitem::Arch_Window:
        newItem = CAD_arch_window();
        break;

    case CADitem::Air_Duct:
        newItem = CAD_air_duct();
        break;
    case CADitem::Air_Pipe:
        newItem = CAD_air_pipe();
        break;
    case CADitem::Air_DuctFireResistant:
        newItem = CAD_air_ductFireResistant();
        break;
    case CADitem::Air_DuctTurn:
        newItem = CAD_air_ductTurn();
        break;
    case CADitem::Air_PipeTurn:
        newItem = CAD_air_pipeTurn();
        break;
    case CADitem::Air_PipeReducer:
        newItem = CAD_air_pipeReducer();
        break;
    case CADitem::Air_PipeTeeConnector:
        newItem = CAD_air_pipeTeeConnector();
        break;
    case CADitem::Air_DuctTeeConnector:
        newItem = CAD_air_ductTeeConnector();
        break;
    case CADitem::Air_DuctTransition:
        newItem = CAD_air_ductTransition();
        break;
    case CADitem::Air_DuctTransitionRectRound:
        newItem = CAD_air_ductTransitionRectRound();
        break;
    case CADitem::Air_DuctYpiece:
        newItem = CAD_air_ductYpiece();
        break;
    case CADitem::Air_DuctEndPlate:
        newItem = CAD_air_ductEndPlate();
        break;
    case CADitem::Air_PipeEndCap:
        newItem = CAD_air_pipeEndCap();
        break;
    case CADitem::Air_ThrottleValve:
        newItem = CAD_air_throttleValve();
        break;
    case CADitem::Air_MultiLeafDamper:
        newItem = CAD_air_multiLeafDamper();
        break;
    case CADitem::Air_PressureReliefDamper:
        newItem = CAD_air_pressureReliefDamper();
        break;
    case CADitem::Air_PipeFireDamper:
        newItem = CAD_air_pipeFireDamper();
        break;
    case CADitem::Air_DuctFireDamper:
        newItem = CAD_air_ductFireDamper();
        break;
    case CADitem::Air_DuctVolumetricFlowController:
        newItem = CAD_air_ductVolumetricFlowController();
        break;
    case CADitem::Air_PipeVolumetricFlowController:
        newItem = CAD_air_pipeVolumetricFlowController();
        break;
    case CADitem::Air_HeatExchangerWaterAir:
        newItem = CAD_air_heatExchangerWaterAir();
        break;
    case CADitem::Air_HeatExchangerAirAir:
        newItem = CAD_air_heatExchangerAirAir();
        break;
    case CADitem::Air_CanvasFlange:
        newItem = CAD_air_canvasFlange();
        break;
    case CADitem::Air_Filter:
        newItem = CAD_air_filter();
        break;
    case CADitem::Air_PipeSilencer:
        newItem = CAD_air_pipeSilencer();
        break;
    case CADitem::Air_DuctBaffleSilencer:
        newItem = CAD_air_ductBaffleSilencer();
        break;
    case CADitem::Air_Fan:
        newItem = CAD_air_fan();
        break;
    case CADitem::Air_Humidifier:
        newItem = CAD_air_humidifier();
        break;
    case CADitem::Air_EmptyCabinet:
        newItem = CAD_air_emptyCabinet();
        break;
    case CADitem::Air_EquipmentFrame:
        newItem = CAD_air_equipmentFrame();
        break;

    case CADitem::HeatCool_Adjustvalve:
        newItem = CAD_heatcool_adjustvalve();
        break;
    case CADitem::HeatCool_Chiller:
        newItem = CAD_heatcool_chiller();
        break;
    case CADitem::HeatCool_Controlvalve:
        newItem = CAD_heatcool_controlvalve();
        break;
    case CADitem::HeatCool_CoolingTower:
        newItem = CAD_heatcool_coolingTower();
        break;
    case CADitem::HeatCool_HeatExchanger:
        newItem = CAD_heatcool_heatExchanger();
        break;
    case CADitem::HeatCool_Pipe:
        newItem = CAD_heatcool_pipe();
        break;
    case CADitem::HeatCool_Pump:
        newItem = CAD_heatcool_pump();
        break;
    case CADitem::HeatCool_Sensor:
        newItem = CAD_heatcool_sensor();
        break;
    case CADitem::HeatCool_PipeTurn:
        newItem = CAD_heatcool_pipeTurn();
        break;
    case CADitem::HeatCool_PipeReducer:
        newItem = CAD_heatcool_pipeReducer();
        break;
    case CADitem::HeatCool_PipeTeeConnector:
        newItem = CAD_heatcool_pipeTeeConnector();
        break;
    case CADitem::HeatCool_PipeEndCap:
        newItem = CAD_heatcool_pipeEndCap();
        break;
    case CADitem::HeatCool_Flange:
        newItem = CAD_heatcool_flange();
        break;
    case CADitem::HeatCool_ExpansionChamber:
        newItem = CAD_heatcool_expansionChamber();
        break;
    case CADitem::HeatCool_Boiler:
        newItem = CAD_heatcool_boiler();
        break;
    case CADitem::HeatCool_WaterHeater:
        newItem = CAD_heatcool_waterHeater();
        break;
    case CADitem::HeatCool_StorageBoiler:
        newItem = CAD_heatcool_storageBoiler();
        break;
    case CADitem::HeatCool_Radiator:
        newItem = CAD_heatcool_radiator();
        break;
    case CADitem::HeatCool_Filter:
        newItem = CAD_heatcool_filter();
        break;
    case CADitem::HeatCool_BallValve:
        newItem = CAD_heatcool_ballValve();
        break;
    case CADitem::HeatCool_ButterflyValve:
        newItem = CAD_heatcool_butterflyValve();
        break;
    case CADitem::HeatCool_SafetyValve:
        newItem = CAD_heatcool_safetyValve();
        break;
    case CADitem::HeatCool_Flowmeter:
        newItem = CAD_heatcool_flowmeter();
        break;

    case CADitem::Sprinkler_CompressedAirWaterContainer:
        newItem = CAD_sprinkler_compressedAirWaterContainer();
        break;
    case CADitem::Sprinkler_Distribution:
        newItem = CAD_sprinkler_distribution();
        break;
    case CADitem::Sprinkler_Head:
        newItem = CAD_sprinkler_head();
        break;
    case CADitem::Sprinkler_Pipe:
        newItem = CAD_sprinkler_pipe();
        break;
    case CADitem::Sprinkler_Pump:
        newItem = CAD_sprinkler_pump();
        break;
    case CADitem::Sprinkler_TeeConnector:
        newItem = CAD_sprinkler_teeConnector();
        break;
    case CADitem::Sprinkler_Valve:
        newItem = CAD_sprinkler_valve();
        break;
    case CADitem::Sprinkler_WetAlarmValve:
        newItem = CAD_sprinkler_wetAlarmValve();
        break;
    case CADitem::Sprinkler_ZoneCheck:
        newItem = CAD_sprinkler_zoneCheck();
        break;

    case CADitem::Electrical_Cabinet:
        newItem = CAD_electrical_cabinet();
        break;
    case CADitem::Electrical_CableTray:
        newItem = CAD_electrical_cableTray();
        break;
    default:
    {
        qDebug() << "ItemDB::drawItem(): unknown item type.";
        return NULL;
    }
        break;
    }

    return newItem;
}
