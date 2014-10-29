#ifndef ITEMDB_H
#define ITEMDB_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QByteArray>
#include <QDataStream>
#include <QDebug>

#include "layer.h"

#include "items/cad_basic_point.h"
#include "items/cad_basic_line.h"
#include "items/cad_basic_polyline.h"
#include "items/cad_basic_circle.h"
#include "items/cad_basic_arc.h"
#include "items/cad_basic_3Dface.h"
#include "items/cad_basic_plane.h"
#include "items/cad_basic_box.h"
#include "items/cad_basic_cylinder.h"
#include "items/cad_basic_pipe.h"
#include "items/cad_basic_turn.h"
#include "items/cad_basic_sphere.h"

#include "items/cad_arch_beam.h"
#include "items/cad_arch_blockout.h"
#include "items/cad_arch_door.h"
#include "items/cad_arch_levelslab.h"
#include "items/cad_arch_support.h"
#include "items/cad_arch_wall_loadbearing.h"
#include "items/cad_arch_wall_nonloadbearing.h"
#include "items/cad_arch_window.h"

#include "items/cad_air_canvasflange.h"
#include "items/cad_air_duct.h"
#include "items/cad_air_ductbafflesilencer.h"
#include "items/cad_air_ductendplate.h"
#include "items/cad_air_ductfiredamper.h"
#include "items/cad_air_ductfireresistant.h"
#include "items/cad_air_ductteeconnector.h"
#include "items/cad_air_ducttransition.h"
#include "items/cad_air_ducttransitionrectround.h"
#include "items/cad_air_ductturn.h"
#include "items/cad_air_ductvolumetricflowcontroller.h"
#include "items/cad_air_ductypiece.h"
#include "items/cad_air_emptycabinet.h"
#include "items/cad_air_equipmentframe.h"
#include "items/cad_air_fan.h"
#include "items/cad_air_filter.h"
#include "items/cad_air_heatexchangerairair.h"
#include "items/cad_air_heatexchangerwaterair.h"
#include "items/cad_air_humidifier.h"
#include "items/cad_air_multileafdamper.h"
#include "items/cad_air_pipe.h"
#include "items/cad_air_pipeendcap.h"
#include "items/cad_air_pipefiredamper.h"
#include "items/cad_air_pipereducer.h"
#include "items/cad_air_pipesilencer.h"
#include "items/cad_air_pipeteeconnector.h"
#include "items/cad_air_pipeturn.h"
#include "items/cad_air_pipevolumetricflowcontroller.h"
#include "items/cad_air_pressurereliefdamper.h"
#include "items/cad_air_throttlevalve.h"

#include "items/cad_heatcool_adjustvalve.h"
#include "items/cad_heatcool_ballvalve.h"
#include "items/cad_heatcool_boiler.h"
#include "items/cad_heatcool_butterflyvalve.h"
#include "items/cad_heatcool_chiller.h"
#include "items/cad_heatcool_controlvalve.h"
#include "items/cad_heatcool_coolingtower.h"
#include "items/cad_heatcool_expansionchamber.h"
#include "items/cad_heatcool_filter.h"
#include "items/cad_heatcool_flange.h"
#include "items/cad_heatcool_flowmeter.h"
#include "items/cad_heatcool_heatexchanger.h"
#include "items/cad_heatcool_pipe.h"
#include "items/cad_heatcool_pipeendcap.h"
#include "items/cad_heatcool_pipereducer.h"
#include "items/cad_heatcool_pipeteeconnector.h"
#include "items/cad_heatcool_pipeturn.h"
#include "items/cad_heatcool_pump.h"
#include "items/cad_heatcool_radiator.h"
#include "items/cad_heatcool_safetyvalve.h"
#include "items/cad_heatcool_sensor.h"
#include "items/cad_heatcool_storageboiler.h"
#include "items/cad_heatcool_waterheater.h"

#include "items/cad_sprinkler_compressedairwatercontainer.h"
#include "items/cad_sprinkler_distribution.h"
#include "items/cad_sprinkler_head.h"
#include "items/cad_sprinkler_pipe.h"
#include "items/cad_sprinkler_pump.h"
#include "items/cad_sprinkler_teeconnector.h"
#include "items/cad_sprinkler_valve.h"
#include "items/cad_sprinkler_wetalarmvalve.h"
#include "items/cad_sprinkler_zonecheck.h"
#include "items/cad_sprinkler_pipeturn.h"
#include "items/cad_sprinkler_pipeendcap.h"
#include "items/cad_sprinkler_pipereducer.h"

#include "items/cad_electrical_cabinet.h"
#include "items/cad_electrical_cabletray.h"

#include "items/cad_sanitary_electricwaterheater.h"
#include "items/cad_sanitary_emergencyeyeshower.h"
#include "items/cad_sanitary_emergencyshower.h"
#include "items/cad_sanitary_flange.h"
#include "items/cad_sanitary_liftingunit.h"
#include "items/cad_sanitary_pipe.h"
#include "items/cad_sanitary_pipeendcap.h"
#include "items/cad_sanitary_pipereducer.h"
#include "items/cad_sanitary_pipeteeconnector.h"
#include "items/cad_sanitary_pipeturn.h"
#include "items/cad_sanitary_shower.h"
#include "items/cad_sanitary_sink.h"
#include "items/cad_sanitary_washbasin.h"

class ItemDB : public QObject
{
    Q_OBJECT
public:
    explicit ItemDB(QObject *parent = 0);
    ~ItemDB();
    QList<Layer*> layers;

    Layer *addLayer(QString layerName, QString parentLayerName = QString());
    Layer *addLayer(QString layerName, Layer* parentLayer);
    bool deleteLayer(Layer* layer);
    Layer* getLayerByName(QString layerName);
    Layer* getTopLevelLayer();

    void addItem(CADitem* item, QString LayerName);
    void addItem(CADitem* item, Layer* layer);
    void deleteItem(CADitem* item);
    bool deleteItem(quint64 id);
    void deleteItems(QList<CADitem*> items);
    CADitem *drawItem(Layer *layer, CADitem::ItemType type);
    CADitem *drawItem(QString layerName, CADitem::ItemType type);
    CADitem *getItemById(quint64 id);
    bool modifyItem(quint64 &id, QString &key, QString &value);

    QByteArray network_getAll();
    QByteArray network_modifyItem(quint64 id, QMap<QString, QString> data);
    QByteArray network_deleteItem(quint64 id);

private:
    Layer* topLevelLayer;
    QMap<QString, Layer*> layerMap;
    QMap<quint64, CADitem*> itemMap;
    CADitem::ItemType activeDrawCommand;
    quint64 currentItemId;
    void network_getAll_processLayers(QList<Layer *> layers, QByteArray *answer);
    void network_getAll_processItems(QList<CADitem *> items, QByteArray* answer);

signals:
    void signal_layerAdded(Layer* newLayer, Layer* parentLayer);
    void signal_itemAdded(CADitem* item, Layer* layer);
//    void signal_itemModified(CADitem* item, Layer* layer);
    void signal_repaintNeeded();

public slots:

};

#endif // ITEMDB_H
