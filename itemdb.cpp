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

ItemDB::ItemDB(QObject *parent) :
    QObject(parent)
{
    currentItemId = 0;
    topLevelLayer = new Layer(this);
    topLevelLayer->name = "$$ToplevelLayer";
    topLevelLayer->solo = true;
    layers.append(topLevelLayer);
    layerSoloActive = false;

    this->activeDrawCommand = CADitemTypes::None;

    this->deriveDomainsAndItemTypes();
}

ItemDB::~ItemDB()
{
    // TODO delete all layers incl. sublayers
}

void ItemDB::deriveDomainsAndItemTypes()
{
    CADitem* item;

    int type = (int)CADitemTypes::None + 1;

    for (;;)
    {
        if (type == CADitemTypes::LastItem)
            break;

        item = createItem((CADitemTypes::ItemType)type);
        if (item == NULL)
        {
            QString enumName = CADitemTypes().getEnumNameOfItemType((CADitemTypes::ItemType)type);
            qDebug() << "itemDB: createItem returned NULL; itemtype:" << type << enumName << "not implemented";
            type++;
            continue;
        }
        else
        {
            //            QString enumName = CADitemTypes().getEnumNameOfItemType((CADitemTypes::ItemType)type);
            //            qDebug() << "ItemDB::deriveDomainsAndItemTypes()" << enumName;
        }

        itemTypesByDomain.insertMulti(item->domain(), (int)type);
        iconPathByItemType.insert(type, item->iconPath());
        itemDescriptionByItemType.insert(type, item->description());
        itemTypeByItemDescription.insert(item->description(), type);
        delete item;
        type++;
    }

    this->domains = itemTypesByDomain.uniqueKeys();

    qDebug() << "Item Type Count:" << itemTypesByDomain.count();
}

int ItemDB::getNumberOfItemTypes()
{
    return itemTypesByDomain.count();
}

QList<QString> ItemDB::getDomains()
{
    return domains;
}

QList<int> ItemDB::getItemTypesByDomain(QString domain)
{
    return itemTypesByDomain.values(domain);
}

QString ItemDB::getItemDescriptionByItemType(CADitemTypes::ItemType type)
{
    return itemDescriptionByItemType.value((int)type);
}

CADitemTypes::ItemType ItemDB::getItemTypeByItemDescription(QString description)
{
    return (CADitemTypes::ItemType) itemTypeByItemDescription.value(description, CADitemTypes::None);
}

QString ItemDB::getIconPathByItemType(CADitemTypes::ItemType type)
{
    return iconPathByItemType.value((int)type);
}

QPixmap ItemDB::getIconByItemType(CADitemTypes::ItemType type, QSize size)
{
    QString filename = getIconPathByItemType(type);
    if (QFile(filename).exists())
    {
        QSvgRenderer svgRenderer(filename);
        QPixmap pixmap(size);
        QPainter painter(&pixmap);
        svgRenderer.render(&painter);
        return pixmap;
    }
    else
    {
        QPixmap pixmap(size);
        pixmap.fill(Qt::yellow);
        return pixmap;
    }
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

CADitem *ItemDB::createItem(CADitemTypes::ItemType type)
{
    CADitem* newItem = NULL;

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
        newItem = new CAD_Basic_Gauge;
        break;
    case CADitemTypes::Basic_Gauge90Degree:
        newItem = new CAD_Basic_Gauge90Degree;
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
    case CADitemTypes::Cleanroom_CeilingFrameFeedThrough:
        newItem = new CAD_Cleanroom_CeilingFrameFeedThrough();
        break;
    case CADitemTypes::Cleanroom_CeilingFrame:
        newItem = new CAD_Cleanroom_CeilingFrame();
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
    case CADitemTypes::Cleanroom_CeilingMountingRails:
        newItem = new CAD_Cleanroom_CeilingMountingRails();
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
    case CADitemTypes::Cleanroom_FloorPanelWithBushing:
        newItem = new CAD_Cleanroom_FloorPanelWithBushing();
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
    case CADitemTypes::Cleanroom_WallMountingProfile:
        newItem = new CAD_Cleanroom_WallMountingProfile();
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
        newItem = new CAD_Electrical_BusbarTapOffUnit;
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
        newItem = new CAD_electrical_cabletray;
        break;
    case CADitemTypes::Electrical_CabletrayCross:
        newItem = new CAD_Electrical_CabletrayCross;
        break;
    case CADitemTypes::Electrical_CabletrayReducer:
        newItem = new CAD_electrical_cabletrayreducer;
        break;
    case CADitemTypes::Electrical_CabletrayTeeConnector:
        newItem = new CAD_electrical_CabletrayTeeconnector;
        break;
    case CADitemTypes::Electrical_CabletrayTransition:
        newItem = new CAD_Electrical_CabletrayTransition;
        break;
    case CADitemTypes::Electrical_CabletrayTurn:
        newItem = new CAD_Electrical_CabletrayTurn;
        break;
    case CADitemTypes::Electrical_CabletrayVerticalLadder:
        newItem = new CAD_Electrical_CabletrayVerticalLadder;
        break;
    case CADitemTypes::Electrical_EquipmentSwitchOrSocket:
        newItem = new CAD_Electrical_EquipmentSwitchOrSocket;
        break;
    case CADitemTypes::Electrical_LuminaireEscapeLighting:
        newItem = new CAD_Electrical_LuminaireEscapeLighting;
        break;
    case CADitemTypes::Electrical_LuminaireRailMounted:
        newItem = new CAD_Electrical_LuminaireRailMounted;
        break;
    case CADitemTypes::Electrical_LuminaireRecessedMounted:
        newItem = new CAD_Electrical_LuminaireRecessedMounted;
        break;
    case CADitemTypes::Electrical_LuminaireSemicircular:
        newItem = new CAD_Electrical_LuminaireSemicircular;
        break;
    case CADitemTypes::Electrical_LuminaireSurfaceMounted:
        newItem = new CAD_Electrical_LuminaireSurfaceMounted;
        break;
    case CADitemTypes::Electrical_MotorAsynchronous:
        newItem = new CAD_Electrical_MotorAsynchronous;
        break;


//        //Gas
    case CADitemTypes::Gas_ActivatedCarbonAdsorber:
        newItem = new CAD_Gas_ActivatedCarbonAdsorber;
        break;
    case CADitemTypes::Gas_Compressor:
        newItem = new CAD_Gas_Compressor;
        break;
    case CADitemTypes::Gas_DryerDesiccant:
        newItem = new CAD_Gas_DryerDesiccant;
        break;
    case CADitemTypes::Gas_DryerRefrigerant:
        newItem = new CAD_Gas_DryerRefrigerant;
        break;
    case CADitemTypes::Gas_Filter:
        newItem = new CAD_Gas_Filter;
        break;
    case CADitemTypes::Gas_GaugePressure:
        newItem = new CAD_Gas_GaugePressure;
        break;
    case CADitemTypes::Gas_Hose:
        newItem = new CAD_Gas_Hose;
        break;
    case CADitemTypes::Gas_LiquidSeparator:
        newItem = new CAD_Gas_LiquidSeparator;
        break;
    case CADitemTypes::Gas_OilWaterSeparationSystem:
        newItem = new CAD_Gas_OilWaterSeparationSystem;
        break;
    case CADitemTypes::Gas_Pipe:
        newItem = new CAD_Gas_Pipe;
        break;
    case CADitemTypes::Gas_PipeTeeConnector:
        newItem = new CAD_Gas_PipeTeeConnector;
        break;
    case CADitemTypes::Gas_PipeTurn:
        newItem = new CAD_Gas_PipeTurn;
        break;
    case CADitemTypes::Gas_PressureRegulator:
        newItem = new CAD_Gas_PressureRegulator;
        break;
    case CADitemTypes::Gas_Pump:
        newItem = new CAD_Gas_Pump;
        break;
    case CADitemTypes::Gas_QuicklockCoupling:
        newItem = new CAD_Gas_QuicklockCoupling;
        break;
    case CADitemTypes::Gas_SensorFlowrate:
        newItem = new CAD_Gas_SensorFlowrate;
        break;
    case CADitemTypes::Gas_SensorMoisture:
        newItem = new CAD_Gas_SensorMoisture;
        break;
    case CADitemTypes::Gas_Tank:
        newItem = new CAD_Gas_Tank;
        break;
    case CADitemTypes::Gas_ValveBall:
        newItem = new CAD_Gas_ValveBall;
        break;
    case CADitemTypes::Gas_ValveDiaphragm:
        newItem = new CAD_Gas_ValveDiaphragm;
        break;
    case CADitemTypes::Gas_ValveNonReturn:
        newItem = new CAD_Gas_ValveNonReturn;
        break;

//    case CADitemTypes::Gas_CDABallValve:
//        newItem = new CAD_Gas_CDABallValve;
//        break;
//    case CADitemTypes::Gas_CDACompressor:
//        newItem = new CAD_Gas_CDACompressor;
//        break;
//    case CADitemTypes::Gas_CDADesiccantDryer:
//        newItem = new CAD_Gas_CDADesiccantDryer;
//        break;
//    case CADitemTypes::Gas_CDADiaphragmValve:
//        newItem = new CAD_Gas_CDADiaphragmValve;
//        break;
//    case CADitemTypes::Gas_CDAFilter:
//        newItem = new CAD_Gas_CDAFilter;
//        break;
//    case CADitemTypes::Gas_CDAFlowmeter:
//        newItem = new CAD_Gas_CDAFlowmeter;
//        break;
//    case CADitemTypes::Gas_CDAHose:
//        newItem = new CAD_Gas_CDAHose;
//        break;
//    case CADitemTypes::Gas_CDAManometer:
//        newItem = new CAD_Gas_CDAManometer;
//        break;
//    case CADitemTypes::Gas_CDANonReturnValve:
//        newItem = new CAD_Gas_CDANonReturnValve;
//        break;
//    case CADitemTypes::Gas_CDAMoistureSensor:
//        newItem = new CAD_Gas_CDAMoistureSensor;
//        break;
//    case CADitemTypes::Gas_CDAPipe:
//        newItem = new CAD_Gas_CDAPipe;
//        break;
//    case CADitemTypes::Gas_CDAPipeTeeConnector:
//        newItem = new CAD_Gas_CDAPipeTeeConnector;
//        break;
//    case CADitemTypes::Gas_CDAPipeTurn:
//        newItem = new CAD_Gas_CDAPipeTurn;
//        break;
//    case CADitemTypes::Gas_CDAPressureRegulator:
//        newItem = new CAD_Gas_CDAPressureRegulator;
//        break;
//    case CADitemTypes::Gas_CDAQuickLockCoupling:
//        newItem = new CAD_Gas_CDAQuickLockCoupling;
//        break;
//    case CADitemTypes::Gas_CDARefrigerantDryer:
//        newItem = new CAD_Gas_CDARefrigerantDryer;
//        break;
//    case CADitemTypes::Gas_CDATank:
//        newItem = new CAD_Gas_CDATank;
//        break;
//    case CADitemTypes::Gas_VACBallValve:
//        newItem = new CAD_Gas_VACBallValve;
//        break;
//    case CADitemTypes::Gas_VACDiaphragmValve:
//        newItem = new CAD_Gas_VACDiaphragmValve;
//        break;
//    case CADitemTypes::Gas_VACFilter:
//        newItem = new CAD_Gas_VACFilter;
//        break;
//    case CADitemTypes::Gas_VACFlowmeter:
//        newItem = new CAD_Gas_VACFlowmeter;
//        break;
//    case CADitemTypes::Gas_VACHose:
//        newItem = new CAD_Gas_VACHose;
//        break;
//    case CADitemTypes::Gas_VACLiquidSeparator:
//        newItem = new CAD_Gas_VACLiquidSeparator;
//        break;
//    case CADitemTypes::Gas_VACManometer:
//        newItem = new CAD_Gas_VACManometer;
//        break;
//    case CADitemTypes::Gas_VACNonReturnValve:
//        newItem = new CAD_Gas_VACNonReturnValve;
//        break;
//    case CADitemTypes::Gas_VACPipe:
//        newItem = new CAD_Gas_VACPipe;
//        break;
//    case CADitemTypes::Gas_VACPipeTeeConnector:
//        newItem = new CAD_Gas_VACPipeTeeConnector;
//        break;
//    case CADitemTypes::Gas_VACPipeTurn:
//        newItem = new CAD_Gas_VACPipeTurn;
//        break;
//    case CADitemTypes::Gas_VACPump:
//        newItem = new CAD_Gas_VACPump;
//        break;
//    case CADitemTypes::Gas_VACQuickLockCoupling:
//        newItem = new CAD_Gas_VACQuickLockCoupling;
//        break;
//    case CADitemTypes::Gas_VACTank:
//        newItem = new CAD_Gas_VACTank;
//        break;

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
        newItem = new CAD_HeatCool_ButterflyValveBolted;
        break;
    case CADitemTypes::HeatCool_ButterflyValveClamped:
        newItem = new CAD_HeatCool_ButterflyValveClamped;
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
        newItem = new CAD_HeatCool_Gauge;
        break;
    case CADitemTypes::HeatCool_Gauge90Degree:
        newItem = new CAD_HeatCool_Gauge90Degree;
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
        newItem = new CAD_HeatCool_PumpNorm;
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
        newItem = new CAD_Sanitary_CleaningPiece;
        break;
    case CADitemTypes::Sanitary_ElectricWaterHeater:
        newItem = new CAD_Sanitary_ElectricWaterHeater;
        break;
    case CADitemTypes::Sanitary_EmergencyEyeShower:
        newItem = new CAD_Sanitary_EmergencyEyeShower;
        break;
    case CADitemTypes::Sanitary_EmergencyShower:
        newItem = new CAD_Sanitary_EmergencyShower;
        break;
    case CADitemTypes::Sanitary_Flange:
        newItem = new CAD_Sanitary_Flange;
        break;
    case CADitemTypes::Sanitary_LiftingUnit:
        newItem = new CAD_Sanitary_LiftingUnit;
        break;
    case CADitemTypes::Sanitary_Pipe:
        newItem = new CAD_sanitary_pipe;
        break;
    case CADitemTypes::Sanitary_PipeEndCap:
        newItem = new CAD_Sanitary_PipeEndCap;
        break;
    case CADitemTypes::Sanitary_PipeReducer:
        newItem = new CAD_sanitary_pipeReducer;
        break;
    case CADitemTypes::Sanitary_PipeTeeConnector:
        newItem = new CAD_sanitary_pipeTeeConnector;
        break;
    case CADitemTypes::Sanitary_PipeTurn:
        newItem = new CAD_sanitary_pipeTurn;
        break;
    case CADitemTypes::Sanitary_PipeYPiece:
        newItem = new CAD_Sanitary_PipeYPiece;
        break;
    case CADitemTypes::Sanitary_Shower:
        newItem = new CAD_Sanitary_Shower;
        break;
    case CADitemTypes::Sanitary_Sink:
        newItem = new CAD_Sanitary_Sink;
        break;
    case CADitemTypes::Sanitary_WashBasin:
        newItem = new CAD_Sanitary_WashBasin;
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

    default:
    {
        qDebug() << "ItemDB::drawItem(): unknown item type.";
        return NULL;
    }
        break;
    }

    return newItem;
}

CADitem* ItemDB::drawItem(Layer* layer, CADitemTypes::ItemType type)
{
    if (layer == NULL)
    {
        qDebug() << "ItemDB::drawItem(): layer is NULL.";
        return NULL;
    }

    if (type == CADitemTypes::None)
    {
        qDebug() << "ItemDB::drawItem(): Tried to create a CADitemTypes::None.";
        return NULL;
    }

    this->activeDrawCommand = type;

    CADitem* newItem = this->createItem(type);

    this->addItem(newItem, layer);

    return newItem;
}

CADitem *ItemDB::drawItem(QString layerName, CADitemTypes::ItemType type)
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

CADitem *ItemDB::drawItem_withRestorePoint(Layer *layer, CADitemTypes::ItemType type, WizardParams wizardParams)
{
    CADitem* item = this->drawItem(layer, type);

    if (item == NULL)   // tbd. handle error...
        return NULL;

    item->wizardParams.insert(wizardParams);
    restorePoints_undo.append(new RestorePoint(RestorePoint::Restore_ItemCreation, item->layer, item->id, item->getType(), wizardParams, wizardParams));

    item->processWizardInput();
    item->calculate();

    emit signal_itemModified(item);
    return item;
}

void ItemDB::deleteItems_withRestorePoint(QList<CADitem *> items)
{
    foreach (CADitem* item, items)
    {
        restorePoints_undo.append(new RestorePoint(RestorePoint::Restore_ItemDeletion, item->layer, item->id, item->getType(), item->wizardParams, item->wizardParams));
        deleteItem(item);
    }
}

void ItemDB::modifyItem_withRestorePoint(CADitem *item, WizardParams newParams)
{
    if (item == NULL)
        return;

    WizardParams paramsBefore = item->wizardParams;
    item->wizardParams.insert(newParams);
    WizardParams paramsAfter = item->wizardParams;

    restorePoints_undo.append(new RestorePoint(RestorePoint::Restore_WizardParams, item->layer, item->id, item->getType(), paramsBefore, paramsAfter));

    item->processWizardInput();
    item->calculate();
    emit signal_itemModified(item);
}

void ItemDB::setRestorePoint()
{
    restorePoints_undo.append(new RestorePoint(RestorePoint::Restore_Stoppoint));
    this->restore_clearRedo();
}

void ItemDB::restore_clearRedo()
{
    while(!this->restorePoints_redo.isEmpty())
    {
        RestorePoint* restorePoint = this->restorePoints_redo.takeLast();
        delete restorePoint;
    }
}

void ItemDB::restore_undo()
{
    RestorePoint* restorePoint;

    while (1)
    {
        if (this->restorePoints_undo.isEmpty())
        {
            emit signal_repaintNeeded();
            return;
        }

        restorePoint = this->restorePoints_undo.takeLast();
        this->restorePoints_redo.prepend(restorePoint);

        switch (restorePoint->getType())
        {
        case RestorePoint::Restore_ItemCreation:
        {
            this->deleteItem(restorePoint->itemID);
        }
            break;
        case RestorePoint::Restore_ItemDeletion:
        {
            quint64 currentItemId_shadow = this->currentItemId;
            this->currentItemId = restorePoint->itemID;
            CADitem* newItem = this->drawItem(restorePoint->layer, restorePoint->itemType);
            this->currentItemId = currentItemId_shadow;

            newItem->wizardParams = restorePoint->wizardParamsBefore;
            newItem->processWizardInput();
            newItem->calculate();
        }
            break;
        case RestorePoint::Restore_ItemLayerChange:
            break;
        case RestorePoint::Restore_WizardParams:
        {
            CADitem* item = this->getItemById(restorePoint->itemID);
            if (item == NULL)
                continue;
            item->wizardParams.insert(restorePoint->wizardParamsBefore);
            item->processWizardInput();
            item->calculate();
        }
            break;
        case RestorePoint::Restore_Stoppoint:
        {
            emit signal_repaintNeeded();
            return;
        }
            break;
        }
    }
}

void ItemDB::restore_redo()
{
    RestorePoint* restorePoint;

    while (1)
    {
        if (this->restorePoints_redo.isEmpty())
        {
            emit signal_repaintNeeded();
            return;
        }

        restorePoint = this->restorePoints_redo.takeFirst();
        this->restorePoints_undo.append(restorePoint);

        switch (restorePoint->getType())
        {
        case RestorePoint::Restore_ItemCreation:
        {
            quint64 currentItemId_shadow = this->currentItemId;
            this->currentItemId = restorePoint->itemID;
            CADitem* newItem = this->drawItem(restorePoint->layer, restorePoint->itemType);
            this->currentItemId = currentItemId_shadow;

            newItem->wizardParams = restorePoint->wizardParamsBefore;
            newItem->processWizardInput();
            newItem->calculate();
        }
            break;
        case RestorePoint::Restore_ItemDeletion:
        {
            this->deleteItem(restorePoint->itemID);
        }
            break;
        case RestorePoint::Restore_ItemLayerChange:
            break;
        case RestorePoint::Restore_WizardParams:
        {
            CADitem* item = this->getItemById(restorePoint->itemID);
            if (item == NULL)
                continue;
            item->wizardParams.insert(restorePoint->wizardParamsAfter);
            item->processWizardInput();
            item->calculate();
        }
            break;
        case RestorePoint::Restore_Stoppoint:
        {
        }
            break;
        }

        if ((this->restorePoints_redo.length() >= 1) && (this->restorePoints_redo.at(0)->getType() == RestorePoint::Restore_Stoppoint))
        {
            emit signal_repaintNeeded();
            return;
        }
    }
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
    CADitem* newItem = drawItem(layerName, (CADitemTypes::ItemType) type);

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

bool ItemDB::file_storeDB(QString filename, QMatrix4x4 matrix_projection, QMatrix4x4 matrix_glSelect, QMatrix4x4 matrix_modelview, QMatrix4x4 matrix_rotation)
{
    QDomDocument document;
    QDomElement root = document.createElement("SmePowerCadProject");
    document.appendChild(root);
    root.setAttribute("Version", QString("Build ") + QString(__DATE__) + " " + QString(__TIME__));

    // Store item [type <-> description] list
    QDomElement element_itemTypeList = document.createElement("ItemTypeList");
    root.appendChild(element_itemTypeList);

    int numberOfItemTypes = this->getNumberOfItemTypes();
    for (int i = 1; i < numberOfItemTypes; i++)
    {
        QString description = this->getItemDescriptionByItemType((CADitemTypes::ItemType) i);
        QDomElement element_itemType = document.createElement("ItemType");
        element_itemTypeList.appendChild(element_itemType);
        element_itemType.setAttribute("Type", QString().sprintf("I%i", i));
        element_itemType.setAttribute("Description", description);
    }

    //store matrices
    QDomElement element_matrix_projection = document.createElement("MatrixProjection");
    root.appendChild(element_matrix_projection);
    for(int i = 0; i < 4;i++)
    {
        QDomElement element_column = document.createElement("Column" + QString::number(i));
        element_matrix_projection.appendChild(element_column);
        element_column.setAttribute("x", matrix_projection.column(i).x());
        element_column.setAttribute("y", matrix_projection.column(i).y());
        element_column.setAttribute("z", matrix_projection.column(i).z());
        element_column.setAttribute("w", matrix_projection.column(i).w());
    }

    QDomElement element_matrix_glselect = document.createElement("MatrixGLSelect");
    root.appendChild(element_matrix_glselect);
    for(int i = 0; i < 4;i++)
    {
        QDomElement element_column = document.createElement("Column" + QString::number(i));
        element_matrix_glselect.appendChild(element_column);
        element_column.setAttribute("x", matrix_glSelect.column(i).x());
        element_column.setAttribute("y", matrix_glSelect.column(i).y());
        element_column.setAttribute("z", matrix_glSelect.column(i).z());
        element_column.setAttribute("w", matrix_glSelect.column(i).w());
    }

    QDomElement element_matrix_modelview = document.createElement("MatrixModelview");
    root.appendChild(element_matrix_modelview);
    for(int i = 0; i < 4;i++)
    {
        QDomElement element_column = document.createElement("Column" + QString::number(i));
        element_matrix_modelview.appendChild(element_column);
        element_column.setAttribute("x", matrix_modelview.column(i).x());
        element_column.setAttribute("y", matrix_modelview.column(i).y());
        element_column.setAttribute("z", matrix_modelview.column(i).z());
        element_column.setAttribute("w", matrix_modelview.column(i).w());
    }

    QDomElement element_matrix_rotation = document.createElement("MatrixRotation");
    root.appendChild(element_matrix_rotation);
    for(int i = 0; i < 4;i++)
    {
        QDomElement element_column = document.createElement("Column" + QString::number(i));
        element_matrix_rotation.appendChild(element_column);
        element_column.setAttribute("x", matrix_rotation.column(i).x());
        element_column.setAttribute("y", matrix_rotation.column(i).y());
        element_column.setAttribute("z", matrix_rotation.column(i).z());
        element_column.setAttribute("w", matrix_rotation.column(i).w());
    }

    // Store cad data
    QDomElement element_cad = document.createElement("CadData");
    root.appendChild(element_cad);
    file_storeDB_processLayers(document, element_cad, this->topLevelLayer->subLayers);

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

bool ItemDB::file_loadDB(QString filename, QString* error, QMatrix4x4 *matrix_projection, QMatrix4x4 *matrix_glSelect, QMatrix4x4 *matrix_modelview, QMatrix4x4 *matrix_rotation)
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
        *error = tr("line %1, column %2:\n%3")
                .arg(errorLine)
                .arg(errorColumn)
                .arg(errorStr);
        return false;
    }

    QString currentVersion = QString("Build ") + QString(__DATE__) + " " + QString(__TIME__);

    QDomElement root = document.documentElement();
    if (root.tagName() != "SmePowerCadProject")
    {
        file.close();
        *error = tr("Root-Node has wrong tagName: ") + root.tagName();
        return false;
    }
    else if (!root.hasAttribute("Version") || root.attribute("Version").isEmpty())
    {
        file.close();
        *error =  tr("Version attribute is missing.");
        return false;
    }

    // Version check
    bool mapByDescription = false;
    if (root.attribute("Version") != currentVersion)
    {
        *error = tr("Old file version: ") + root.attribute("Version") + "\n" +
                tr("Current version: ") + currentVersion + "\n" +
                tr("Converting file to current version.");
        mapByDescription = true;
    }

    //Read matrices
    QDomElement element_matrix_projection = root.firstChildElement("MatrixProjection");
    for(int i = 0; i < 4; i++)
    {
        QDomElement element_column = element_matrix_projection.firstChildElement("Column" + QString::number(i));
        double x = element_column.attribute("x").toDouble();
        double y = element_column.attribute("y").toDouble();
        double z = element_column.attribute("z").toDouble();
        double w = element_column.attribute("w").toDouble();
        matrix_projection->setColumn(i, QVector4D(x, y, z, w));
    }

    QDomElement element_matrix_glselect = root.firstChildElement("MatrixGLSelect");
    for(int i = 0; i < 4; i++)
    {
        QDomElement element_column = element_matrix_glselect.firstChildElement("Column" + QString::number(i));
        double x = element_column.attribute("x").toDouble();
        double y = element_column.attribute("y").toDouble();
        double z = element_column.attribute("z").toDouble();
        double w = element_column.attribute("w").toDouble();
        matrix_glSelect->setColumn(i, QVector4D(x, y, z, w));
    }

    QDomElement element_matrix_modelview = root.firstChildElement("MatrixModelview");
    for(int i = 0; i < 4; i++)
    {
        QDomElement element_column = element_matrix_modelview.firstChildElement("Column" + QString::number(i));
        double x = element_column.attribute("x").toDouble();
        double y = element_column.attribute("y").toDouble();
        double z = element_column.attribute("z").toDouble();
        double w = element_column.attribute("w").toDouble();
        matrix_modelview->setColumn(i, QVector4D(x, y, z, w));
    }

    QDomElement element_matrix_rotation = root.firstChildElement("MatrixRotation");
    for(int i = 0; i < 4; i++)
    {
        QDomElement element_column = element_matrix_rotation.firstChildElement("Column" + QString::number(i));
        double x = element_column.attribute("x").toDouble();
        double y = element_column.attribute("y").toDouble();
        double z = element_column.attribute("z").toDouble();
        double w = element_column.attribute("w").toDouble();
        matrix_rotation->setColumn(i, QVector4D(x, y, z, w));
    }

    // Read itemTypeList from file and build map
    QMap <int, QString> file_itemDescriptionByItemType;
    QDomElement element_itemTypeList = root.firstChildElement("ItemTypeList");
    QDomElement element_itemType = element_itemTypeList.firstChildElement("ItemType");
    while (!element_itemType.isNull())
    {
        QString type = element_itemType.attribute("Type");
        QString description = element_itemType.attribute("Description");
        type.remove(0, 1);   // Strip "I"
        file_itemDescriptionByItemType.insert(type.toInt(), description);
        element_itemType = element_itemType.nextSiblingElement("ItemType");
    }

    // Read cad data from file
    QDomElement element_cadData = root.firstChildElement("CadData");
    while (!element_cadData.isNull())
    {
        this->file_loadDB_parseDomElement(element_cadData, this->topLevelLayer, mapByDescription, &file_itemDescriptionByItemType, error);  // tbd. toplevellayer may be wrong here...
        element_cadData = element_cadData.nextSiblingElement();
    }

    file.close();
    emit signal_layerManagerUpdateNeeded();
    return true;
}

void ItemDB::file_loadDB_parseDomElement(QDomElement element, Layer *currentLayer, bool mapByDescription, QMap<int, QString>* file_itemDescriptionByItemType, QString* error)
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
        if (mapByDescription)   // Get the right item type if file version is out of date (fetch item by descrition string instead of item type number which may have changed)
        {
            QString itemDescription = file_itemDescriptionByItemType->value(itemType);
            itemType = this->getItemTypeByItemDescription(itemDescription);
            if ((CADitemTypes::ItemType)itemType == CADitemTypes::None)
                error->append("\nUnable to resolve name: " + itemDescription);
        }
        CADitem* item = this->drawItem(currentLayer, (CADitemTypes::ItemType)itemType);
        if (item == NULL)
        {
            error->append(tr("\nItemDB::file_loadDB_parseDomElement(): Got a NULL item."));
            return;
        }
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
        this->file_loadDB_parseDomElement(child, currentLayer, mapByDescription, file_itemDescriptionByItemType, error);
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
