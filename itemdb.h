#ifndef ITEMDB_H
#define ITEMDB_H

#include <QObject>
#include <QList>

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
#include "items/cad_basic_sphere.h"

#include "items/cad_arch_levelslab.h"
#include "items/cad_arch_door.h"
#include "items/cad_arch_window.h"
#include "items/cad_arch_wall_loadbearing.h"
#include "items/cad_arch_wall_nonloadbearing.h"
#include "items/cad_arch_blockout.h"

#include "items/cad_air_duct.h"
#include "items/cad_air_pipe.h"
#include "items/cad_air_ductturn.h"
#include "items/cad_air_pipeturn.h"
#include "items/cad_air_pipereducer.h"
#include "items/cad_air_pipeteeconnector.h"

#include "items/cad_heatcool_adjustvalve.h"
#include "items/cad_heatcool_chiller.h"
#include "items/cad_heatcool_controlvalve.h"
#include "items/cad_heatcool_coolingtower.h"
#include "items/cad_heatcool_heatexchanger.h"
#include "items/cad_heatcool_pipe.h"
#include "items/cad_heatcool_pump.h"
#include "items/cad_heatcool_sensor.h"

#include "items/cad_sprinkler_compressedairwatercontainer.h"
#include "items/cad_sprinkler_distribution.h"
#include "items/cad_sprinkler_head.h"
#include "items/cad_sprinkler_pipe.h"
#include "items/cad_sprinkler_pump.h"
#include "items/cad_sprinkler_teeconnector.h"
#include "items/cad_sprinkler_valve.h"
#include "items/cad_sprinkler_wetalarmvalve.h"
#include "items/cad_sprinkler_zonecheck.h"

#include "items/cad_electrical_cabinet.h"
#include "items/cad_electrical_cabletray.h"

class ItemDB : public QObject
{
    Q_OBJECT
public:
    explicit ItemDB(QObject *parent = 0);
    ~ItemDB();
    QList<Layer*> layers;

    Layer *addLayer(QString layerName, QString parentLayerName = QString());
    Layer* getLayerByName(QString layerName);
    Layer* getTopLevelLayer();

    void addItem(CADitem* item, QString LayerName);

private:
    Layer* topLevelLayer;
    QMap<QString, Layer*> layerMap;

signals:
    void signal_layerAdded(Layer* newLayer, Layer* parentLayer);

public slots:

};

#endif // ITEMDB_H
