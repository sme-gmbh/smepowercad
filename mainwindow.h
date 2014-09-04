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

#include "cadcommand.h"
#include "layermanager.h"
#include "layer.h"

#include "caditem.h"

#include "itemdb.h"
#include "geometrydisplay.h"

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

    //void on_doubleSpinBox_heightOfIntersection_valueChanged(double arg1);

signals:
    void signal_command_prompt_input(QString str);
    void signal_repaintNeeded();
};

#endif // MAINWINDOW_H
