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

#include "cadcommand.h"
#include "layermanager.h"
#include "layer.h"

#include "caditem.h"

#include "itemdb.h"
#include "geometrydisplay.h"

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
    ItemDB* itemDB;
    CADcommand* cadcommand;
//    QGraphicsScene* scene;
    LayerManager* layerManager;

    // Current drawing objects
    CADline* current_cadline;

    GeometryDisplay* mainGeometryDisplay;
    QList<GeometryDisplay*> geometryDisplays;

    // recent files
    QList<QAction *> recentFileActs;
    void updateRecentFileActions();
    QString strippedName(QString fullName);

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void slot_file_open_action();
    void slot_file_save_action();
    void slot_file_save_as_action();

    void slot_file_print_action();
    void slot_file_pdf_export_action();

    void slot_file_close_action();

    void slot_draw_line_action();
    void slot_draw_arc_action();
    void slot_draw_circle_action();

    void slot_newGeometryDisplay();
    void slot_geometryDisplayAboutToClose(QAction *action);
private slots:
    void on_prompt_input_returnPressed();
    void slot_openRecentFile();
    void slot_clearRecentFiles();

signals:
    void signal_command_prompt_input(QString str);
    void signal_repaintNeeded();
};

#endif // MAINWINDOW_H
