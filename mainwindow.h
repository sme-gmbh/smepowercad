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
#include <QMouseEvent>
#include <QSettings>
#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QSurfaceFormat>

#ifdef USE_3D_MOUSE
#include "3Dmouse/qmagellan.h"
#endif

#include "layermanager.h"
#include "layer.h"

#include "caditem.h"

#include "itemdb.h"
#include "geometrydisplay.h"
#include "collisiondetection.h"

#include "modaldialog.h"
#include "settingsdialog.h"
#include "itemwizard.h"
#include "itemgripmodifier.h"
#include "printwidget.h"
#include "keyframeanimation.h"
#include "itemcatalog.h"

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

#ifdef USE_3D_MOUSE
    QMagellanThread *magellanThread;
#endif
    ItemDB* itemDB;
    ItemCatalog* itemCatalog;
    LayerManager* layerManager;
    CollisionDetection* collisionDetection;

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

    ItemWizard* itemWizard;
    ItemGripModifier* itemGripModifier;

    PrintWidget* printwidget;
    KeyframeAnimation* keyframeAnimation;

    QString project_filepath;

    void createItemToolBar();
    void setProjectFilepath(QString filepath);
    void addToRecentFiles(QString filepath);

protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void slot_file_open_action();
    void slot_file_open_dxf(QString filename);
    void slot_file_open_dxf_with_libdxfrw(QString filename);
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
    void slot_fileNeedsSaving();
    void slot_fileSaved();

private slots:
    void slot_openRecentFile();
    void slot_clearRecentFiles();
    void on_actionAbout_OpenGL_triggered();
    void on_actionAbout_triggered();
    void on_actionSettings_triggered();
    void on_actionServer_triggered(bool checked);
    void slot_collision_detected(CADitem* item_1, CADitem *item_2);
    void slot_no_collision_detected(CADitem* item_1);


    void on_actionCollision_Detection_triggered(bool checked);

signals:
    void signal_command_prompt_input(QString str);
    void signal_repaintNeeded();
};

#endif // MAINWINDOW_H
