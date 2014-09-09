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

#include "items/cad_arch_door.h"
#include "items/cad_arch_window.h"
#include "items/cad_arch_wall_loadbearing.h"
#include "items/cad_arch_wall_nonloadbearing.h"
#include "items/cad_arch_window.h"

#include "items/cad_air_duct.h"
#include "items/cad_air_pipe.h"

#include "items/cad_heatcool_adjustvalve.h"
#include "items/cad_heatcool_chiller.h"
#include "items/cad_heatcool_controlvalve.h"
#include "items/cad_heatcool_coolingtower.h"
#include "items/cad_heatcool_heatexchanger.h"
#include "items/cad_heatcool_pipe.h"
#include "items/cad_heatcool_pump.h"
#include "items/cad_heatcool_sensor.h"

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
