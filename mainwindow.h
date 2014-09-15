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

signals:
    void signal_command_prompt_input(QString str);
    void signal_repaintNeeded();
};

#endif // MAINWINDOW_H
