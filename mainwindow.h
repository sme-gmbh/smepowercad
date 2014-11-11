#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QFont>
#include <QList>
#include <QAction>
#include <QPointF>
#include <QEvent>
#include <QKeyEvent>
#include <QSettings>

#include "3Dmouse/qmagellan.h"

#include "cadcommand.h"
#include "layermanager.h"
#include "layer.h"

#include "caditem.h"

#include "itemdb.h"
#include "geometrydisplay.h"

#include "modaldialog.h"
#include "settingsdialog.h"
#include "itemwizard.h"

#include "network/server.h"

#define MAX_RECENT_FILES 5

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMagellanThread *magellanThread;
    ItemDB* itemDB;
    CADcommand* cadcommand;
//    QGraphicsScene* scene;
    LayerManager* layerManager;

    Server* server;

    // Current drawing objects
    CAD_basic_line* current_cadline;

    GeometryDisplay* mainGeometryDisplay;
    QList<GeometryDisplay*> geometryDisplays;

    // recent files
    QList<QAction *> recentFileActs;
    void updateRecentFileActions();
    QString strippedName(QString fullName);

    SettingsDialog *settingsDialog;

    ItemWizard *itemWizard;

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void slot_file_open_action();
    void slot_file_save_action();
    void slot_file_save_as_action();

    void slot_file_print_action();
    void slot_file_pdf_export_action();

    void slot_file_close_action();

    void slot_newGeometryDisplay();
    void slot_geometryDisplayAboutToClose(QAction *action);

    void slot_highlightItem(CADitem* item);
    void slot_snapTo(QVector3D snapPos_scene, int snapMode);
    void slot_selectionChanged(QList<CADitem*> selectedItems);

private slots:
    void on_prompt_input_returnPressed();
    void slot_openRecentFile();
    void slot_clearRecentFiles();

    void on_actionAbout_OpenGL_triggered();

    void on_actionAbout_triggered();

    void on_actionSettings_triggered();

    void on_action_basic_face_triggered();

    void on_action_basic_arc_triggered();

    void on_action_basic_box_triggered();

    void on_action_basic_circle_triggered();

    void on_action_basic_cylinder_triggered();

    void on_action_basic_linie_triggered();

    void on_action_basic_plane_triggered();

    void on_action_basic_point_triggered();

    void on_action_basic_polylinie_triggered();

    void on_action_basic_sphere_triggered();

    void on_action_arch_beam_triggered();

    void on_action_arch_blockout_triggered();

    void on_action_arch_door_triggered();

    void on_action_arch_levelSlab_triggered();

    void on_action_arch_support_triggered();

    void on_action_arch_wall_loadbearing_triggered();

    void on_action_arch_wall_nonloadbearing_triggered();

    void on_action_arch_window_triggered();

    void on_action_air_canvas_flange_triggered();

    void on_action_air_duct_baffle_silencer_triggered();

    void on_action_air_duct_triggered();

    void on_action_air_duct_end_plate_triggered();

    void on_action_air_duct_fire_damper_triggered();

    void on_action_air_duct_fire_resistant_triggered();

    void on_action_air_duct_T_Connector_triggered();

    void on_action_air_duct_transition_triggered();

    void on_action_air_duct_transition_rect_round_triggered();

    void on_action_air_duct_turn_triggered();

    void on_action_air_duct_volumetric_flow_controller_triggered();

    void on_action_air_duct_Y_piece_triggered();

    void on_action_air_empty_cabinet_triggered();

    void on_action_air_equipment_frame_triggered();

    void on_action_air_fan_triggered();

    void on_action_air_filter_triggered();

    void on_action_air_heat_exchanger_air_air_triggered();

    void on_action_air_heat_exchanger_water_air_triggered();

    void on_action_air_Humidifier_triggered();

    void on_action_air_multi_leaf_damper_triggered();

    void on_action_air_pipe_triggered();

    void on_action_air_pipe_end_cap_triggered();

    void on_action_air_pipe_fire_damper_triggered();

    void on_action_air_pipe_reducer_triggered();

    void on_action_air_pipe_silencer_triggered();

    void on_action_air_pipe_T_Connector_triggered();

    void on_action_air_pipe_turn_triggered();

    void on_action_air_pipe_volumetric_flow_controller_triggered();

    void on_action_air_pressure_relief_damper_triggered();

    void on_action_air_throttle_valve_triggered();

    void on_action_heatcool_Adjustvalve_triggered();

    void on_action_heatcool_ballvalve_triggered();

    void on_action_heatcool_boiler_triggered();

    void on_action_heatcool_butterflyvalve_triggered();

    void on_action_heatcool_chiller_triggered();

    void on_action_heatcool_controlvalve_triggered();

    void on_action_heatcool_coolingtower_triggered();

    void on_action_heatcool_expansionchamber_triggered();

    void on_action_heatcool_filter_triggered();

    void on_action_heatcool_flange_triggered();

    void on_action_heatcool_flowmeter_triggered();

    void on_action_heatcool_heatexchanger_triggered();

    void on_action_heatcool_pipe_triggered();

    void on_action_heatcool_pipeendcap_triggered();

    void on_action_heatcool_pipereducer_triggered();

    void on_action_heatcool_pipeteeconnector_triggered();

    void on_action_heatcool_pipeturn_triggered();

    void on_action_heatcool_pump_triggered();

    void on_action_heatcool_radiator_triggered();

    void on_action_heatcool_safetyvalve_triggered();

    void on_action_heatcool_sensor_triggered();

    void on_action_heatcool_storageboiler_triggered();

    void on_action_heatcool_waterheater_triggered();

    void on_action_sprinkler_compressedairwatercontainer_triggered();

    void on_action_sprinkler_distribution_triggered();

    void on_action_sprinkler_head_triggered();

    void on_action_sprinkler_pipe_triggered();

    void on_action_sprinkler_pipeturn_triggered();

    void on_action_sprinkler_Pump_triggered();

    void on_action_sprinkler_teeconnector_triggered();

    void on_action_sprinkler_valve_triggered();

    void on_action_sprinkler_wetalarmvalve_triggered();

    void on_action_sprinkler_zonecheck_triggered();

    void on_action_basic_pipe_triggered();

    void on_action_basic_Turn_triggered();

    void on_actionServer_triggered(bool checked);

    void on_actionArchitecture_Bored_Pile_triggered();

    void on_actionArchitecture_Foundation_triggered();

    void on_actionArchitecture_Grating_triggered();

    void on_actionBasic_Duct_triggered();

signals:
    void signal_command_prompt_input(QString str);
    void signal_repaintNeeded();
};

#endif // MAINWINDOW_H
