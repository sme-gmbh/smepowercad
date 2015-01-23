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

#include "layermanager.h"
#include "layer.h"

#include "caditem.h"

#include "itemdb.h"
#include "geometrydisplay.h"

#include "modaldialog.h"
#include "settingsdialog.h"
#include "itemwizard.h"
#include "itemgripmodifier.h"

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
    ItemGripModifier* itemGripModifier;

    QString project_filepath;

    void createItemToolBar();

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void slot_file_open_action();
    void slot_file_open_dxf(QString filename);
    void slot_file_open_xml(QString filename);
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

    void slot_createNewItem(CADitemTypes::ItemType type);

private slots:
//    void on_prompt_input_returnPressed();
    void slot_openRecentFile();
    void slot_clearRecentFiles();
    void on_actionAbout_OpenGL_triggered();
    void on_actionAbout_triggered();
    void on_actionSettings_triggered();
    void on_actionServer_triggered(bool checked);


signals:
    void signal_command_prompt_input(QString str);
    void signal_repaintNeeded();
};

#endif // MAINWINDOW_H
