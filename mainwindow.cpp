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

#include <QDebug>
#include <QFileDialog>
#include <QToolBar>
#include <QPrinter>
#include <libdxfrw0/drw_interface.h>
#include <libdxfrw0/libdxfrw.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dxflib/src/dl_dxf.h"
#include "dxflib/src/dl_creationadapter.h"
#include "creationinterface.h"
#include "toolwidget.h"
#include "dxfreaderinterface.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // **** Global Variables ****
    current_cadline = NULL;

    // **** CAD Item Database *****
    itemDB = new ItemDB(this);
    Layer* topLevelLayer = itemDB->getTopLevelLayer();

    // **** Settings Dialog ****
    settingsDialog = new SettingsDialog(this);

    // **** Item Wizard ****
    itemWizard = new ItemWizard(this);
    connect(itemWizard, SIGNAL(signal_sceneRepaintNeeded()), this, SIGNAL(signal_repaintNeeded()));

    // **** Item Grip Modifier ****
    itemGripModifier = new ItemGripModifier(itemDB, itemWizard, this);
    connect(itemGripModifier, SIGNAL(signal_sceneRepaintNeeded()), this, SIGNAL(signal_repaintNeeded()));

    // **** Command prompt ****
//    QWidget *promptTitle = new QWidget(ui->dockWidgetPrompt);
//    promptTitle->setMaximumWidth(0);
//    promptTitle->setMaximumHeight(0);
//    ui->dockWidgetPrompt->setTitleBarWidget(promptTitle);


    // **** Menubar actions ****
    connect(ui->actionLoad,      SIGNAL(triggered()), this, SLOT(slot_file_open_action()));
    connect(ui->actionSave,      SIGNAL(triggered()), this, SLOT(slot_file_save_action()));
    connect(ui->actionSaveAs,    SIGNAL(triggered()), this, SLOT(slot_file_save_as_action()));
    connect(ui->actionPlot,      SIGNAL(triggered()), this, SLOT(slot_file_print_action()));
    connect(ui->actionPDF,       SIGNAL(triggered()), this, SLOT(slot_file_pdf_export_action()));
    connect(ui->actionClose,     SIGNAL(triggered()), this, SLOT(slot_file_close_action()));
    connect(ui->actionQuit,      SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->actionNewWindow, SIGNAL(triggered()), this, SLOT(slot_newGeometryDisplay()));


    // **** Layer Manager ****
    this->layerManager = new LayerManager(this, topLevelLayer, itemDB);
    QAction* action_layerManager = this->layerManager->toggleViewAction();
    action_layerManager->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    connect(itemDB, SIGNAL(signal_layerAdded(Layer*,Layer*)), layerManager, SLOT(slot_layerAdded(Layer*,Layer*)));
    connect(itemDB, SIGNAL(signal_layerChanged(Layer*)), layerManager, SLOT(slot_layerChanged(Layer*)));
    connect(itemDB, SIGNAL(signal_layerDeleted(Layer*)), layerManager, SLOT(slot_layerDeleted(Layer*)));
    ui->menuFormat->addAction(action_layerManager);

    // **** CAD window (2nd version) *****
    mainGeometryDisplay = new GeometryDisplay(itemDB, itemWizard, itemGripModifier, this);
    connect(this, SIGNAL(signal_repaintNeeded()), mainGeometryDisplay, SIGNAL(signal_repaintNeeded()));
    connect(layerManager, SIGNAL(signal_repaintNeeded()), mainGeometryDisplay, SIGNAL(signal_repaintNeeded()));
    connect(itemDB, SIGNAL(signal_repaintNeeded()), mainGeometryDisplay, SIGNAL(signal_repaintNeeded()));
    connect(itemDB, SIGNAL(signal_itemDeleted(CADitem*)), mainGeometryDisplay, SIGNAL(signal_itemDeleted(CADitem*)));
    connect(itemDB, SIGNAL(signal_layerManagerUpdateNeeded()), layerManager, SLOT(slot_updateAllLayers()));
    connect(settingsDialog, SIGNAL(signal_settingsChanged()), mainGeometryDisplay, SIGNAL(signal_settingsChanged()));
    connect(mainGeometryDisplay, SIGNAL(signal_highlightItem(CADitem*)), this, SLOT(slot_highlightItem(CADitem*)));
    connect(mainGeometryDisplay, SIGNAL(signal_snapFired(QVector3D,int)), this, SLOT(slot_snapTo(QVector3D,int)));
    connect(mainGeometryDisplay, SIGNAL(signal_selectionChanged(QList<CADitem*>)), this, SLOT(slot_selectionChanged(QList<CADitem*>)));
    mainGeometryDisplay->setFeatures(QDockWidget::NoDockWidgetFeatures);
    mainGeometryDisplay->setAllowedAreas(Qt::NoDockWidgetArea);
    mainGeometryDisplay->hideButtons();
    geometryDisplays.append(mainGeometryDisplay);       // Test if this works!
    this->setCentralWidget(this->mainGeometryDisplay);

    // **** Collision detection ****
    QOffscreenSurface* offscreensurface = new QOffscreenSurface();
    offscreensurface->create();
    offscreensurface->setFormat(QSurfaceFormat::defaultFormat());
    this->collisionDetection = new CollisionDetection(this->itemDB, mainGeometryDisplay, offscreensurface, this);
    offscreensurface->moveToThread(collisionDetection);
    connect(itemDB, SIGNAL(signal_itemDeleted(CADitem*)), collisionDetection, SLOT(slot_itemDeleted(CADitem*)));
    connect(itemDB, SIGNAL(signal_itemModified(CADitem*)), collisionDetection, SLOT(slot_testModifiedItem(CADitem*)));
    connect(collisionDetection, SIGNAL(signal_itemsDoCollide(CADitem*,CADitem*, QVector3D, QVector3D)), this, SLOT(slot_collision_detected(CADitem*,CADitem*, QVector3D, QVector3D)));



    // **** Toolbar toggles ****
    this->createItemToolBar();
    ui->menuWerkzeugleisten->addAction(ui->toolBarItems->toggleViewAction());

    // **** Print Widget ****
    printwidget = new PrintWidget(this, itemDB);

    // **** Recent files ****
    QMenu *recentFilesMenu = new QMenu();
    for (int i = 0; i < MAX_RECENT_FILES; i++)
    {
        QAction *action = new QAction(this);
        recentFileActs.append(action);
        action->setVisible(false);
        connect(action, SIGNAL(triggered()), this, SLOT(slot_openRecentFile()));
        recentFilesMenu->addAction(action);
    }
    recentFilesMenu->addSeparator();
    QAction *clear = recentFilesMenu->addAction(tr("Clear menu"));
    connect(clear, SIGNAL(triggered()), this, SLOT(slot_clearRecentFiles()));
    ui->menuDatei->actions().at(1)->setMenu(recentFilesMenu);
    updateRecentFileActions();

    // **** Network server ****
    this->server = new Server(itemDB, this);

    // **** 3D mouse ****
#ifdef USE_3D_MOUSE
    magellanThread = new QMagellanThread();
    connect(magellanThread, SIGNAL(signal_mouseCoords(int,int,int,int,int,int)), mainGeometryDisplay, SIGNAL(signal_mouse3Dcoords(int,int,int,int,int,int)));
    //connect(magellanThread, SIGNAL(signal_buttonPressed(int)), this, SLOT());
    //connect(magellanThread, SIGNAL(signal_buttonReleased(int)), this, SLOT());
    magellanThread->start();
#endif




    // ***** Spielwiese *****

//    Layer* layer = itemDB->addLayer("Testlayer");
//    layer->pen.setColor(Qt::gray);
//    layer->brush.setColor(Qt::white);
//    layer->lineType = "Continuos";
//    layer->width = 1;

    Layer* layerX = itemDB->addLayer("X");
    layerX->pen.setColor(Qt::gray);
    layerX->brush.setColor(QColor(0x1c,0x1c,0x1c));
    layerX->lineType = "Continuos";
    layerX->width = 1;

//    Layer* layerY = itemDB->addLayer("Y");
//    layerY->pen.setColor(Qt::gray);
//    layerY->brush.setColor(Qt::green);
//    layerY->lineType = "Continuos";
//    layerY->width = 1;

//    Layer* layerZ = itemDB->addLayer("Z");
//    layerZ->pen.setColor(Qt::gray);
//    layerZ->brush.setColor(Qt::blue);
//    layerZ->lineType = "Continuos";
//    layerZ->width = 1;


    //    CAD_basic_box* item;

    //    item = (CAD_basic_box*)itemDB->drawItem("Testlayer", CADitemTypes::Basic_Box);
    //    item->position = QVector3D(0.0, 0.0, 0.0);

    //    item->wizardParams.insert(QObject::tr("Center x"), (0.0));
    //    item->wizardParams.insert(QObject::tr("Center y"), (0.0));
    //    item->wizardParams.insert(QObject::tr("Center z"), (0.0));
    //    item->wizardParams.insert(QObject::tr("Size x"), (10.0));
    //    item->wizardParams.insert(QObject::tr("Size y"), (10.0));
    //    item->wizardParams.insert(QObject::tr("Size z"), (10.0));

    //    item->processWizardInput();
    //    item->calculate();

    //    item = (CAD_basic_box*)itemDB->drawItem("X", CADitemTypes::Basic_Box);
    //    item->position = QVector3D(0.0, 0.0, 0.0);

    //    item->wizardParams.insert(QObject::tr("Center x"), (15.0));
    //    item->wizardParams.insert(QObject::tr("Center y"), (0.0));
    //    item->wizardParams.insert(QObject::tr("Center z"), (0.0));
    //    item->wizardParams.insert(QObject::tr("Size x"), (5.0));
    //    item->wizardParams.insert(QObject::tr("Size y"), (5.0));
    //    item->wizardParams.insert(QObject::tr("Size z"), (5.0));

    //    item->processWizardInput();
    //    item->calculate();

    //    item = (CAD_basic_box*)itemDB->drawItem("Y", CADitemTypes::Basic_Box);
    //    item->position = QVector3D(0.0, 0.0, 0.0);

    //    item->wizardParams.insert(QObject::tr("Center x"), (0.0));
    //    item->wizardParams.insert(QObject::tr("Center y"), (15.0));
    //    item->wizardParams.insert(QObject::tr("Center z"), (0.0));
    //    item->wizardParams.insert(QObject::tr("Size x"), (4.0));
    //    item->wizardParams.insert(QObject::tr("Size y"), (4.0));
    //    item->wizardParams.insert(QObject::tr("Size z"), (4.0));

    //    item->processWizardInput();
    //    item->calculate();

    //    item = (CAD_basic_box*)itemDB->drawItem("Z", CADitemTypes::Basic_Box);
    //    item->position = QVector3D(0.0, 0.0, 0.0);

    //    item->wizardParams.insert(QObject::tr("Center x"), (0.0));
    //    item->wizardParams.insert(QObject::tr("Center y"), (0.0));
    //    item->wizardParams.insert(QObject::tr("Center z"), (15.0));
    //    item->wizardParams.insert(QObject::tr("Size x"), (3.0));
    //    item->wizardParams.insert(QObject::tr("Size y"), (3.0));
    //    item->wizardParams.insert(QObject::tr("Size z"), (3.0));

    //    item->processWizardInput();
    //    item->calculate();

//    CAD_basic_turn* item;
//    QVector3D pos;


//    for (int i=0; i < 2; i++)
//    {
//        if(i != 0)
//        {
//            pos = QVector3D(20.0 * i, 20.0 * i, 20.0 * i);
//            item = (CAD_basic_turn*)itemDB->drawItem("X", CADitemTypes::Basic_Turn);
//            item->position = pos + QVector3D(0.0, (-20 * i) % 40, 0.0);
//            item->angle_z = (90 * i) % 180;

//            item->calculate();
//        }
//    }

//    // Memory usage test
//    CAD_sprinkler_valve* item;  // An item that is empty at the moment

//    for (int i=0; i < 1000000; i++)
//    {
//        item = (CAD_sprinkler_valve*)itemDB->drawItem("X", CADitemTypes::Sprinkler_Valve);
//        item->processWizardInput();
//        item->calculate();
//    }

//    qDebug() << "Start test anglesFromMatrix()";
//    for(int x = 0; x < 360; x++)
//    {
//        qDebug() << x << " / 360";
//        for(int y = 0; y < 360; y++)
//        {
//            for(int z = 0; z < 360; z++)
//            {
//                qreal psi = 0.0;
//                qreal theta = 0.0;
//                qreal phi = 0.0;

//                psi = x;
//                theta = y;
//                phi = z;
//                QMatrix4x4 m;
//                m.setToIdentity();
//                m.rotate(psi, 1.0, 0.0, 0.0);
//                m.rotate(theta, 0.0, 1.0, 0.0);
//                m.rotate(phi, 0.0, 0.0, 1.0);
//                QVector3D angles = MAngleCalculations().anglesFromMatrix(m);
//                QMatrix4x4 w;
//                w.setToIdentity();
//                w.rotate(angles.x(), 1.0, 0.0, 0.0);
//                w.rotate(angles.y(), 0.0, 1.0, 0.0);
//                w.rotate(angles.z(), 0.0, 0.0, 1.0);
//                if(MAngleCalculations().matrixNorm(m-w) > 10E-6)
//                {
//                    qDebug() << m;
//                    qDebug() << w;
//                }
//            }
//        }
//    }
//    qDebug() << "Finished test anglesFromMatrix()";

    itemDB->addLayer("Collision");
    itemDB->getLayerByName("Collision")->pen.setColor(QColor(255,0,0));
    itemDB->getLayerByName("Collision")->brush.setColor(QColor(255,0,0));
    this->layerManager->slot_updateAllLayers();


    //    emit signal_repaintNeeded();
}

MainWindow::~MainWindow()
{
#ifdef USE_3D_MOUSE
    magellanThread->terminate();
    delete magellanThread;
#endif
    //    delete layerManager;
    //    delete itemDB;
    //    delete settingsDialog;
    delete ui;
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MAX_RECENT_FILES);

    if (numRecentFiles < 1)
    {
        ui->menuDatei->actions().at(1)->setEnabled(false);
        return;
    }
    else
        ui->menuDatei->actions().at(1)->setEnabled(true);

    for (int i = 0; i < numRecentFiles; i++)
    {
        QString text = strippedName(files[i]);
        recentFileActs.at(i)->setText(text);
        recentFileActs.at(i)->setData(files[i]);
        recentFileActs.at(i)->setVisible(true);
    }
    for (int i = numRecentFiles; i < MAX_RECENT_FILES; i++)
        recentFileActs.at(i)->setVisible(false);
}

QString MainWindow::strippedName(QString fullName)
{
    return QFileInfo(fullName).fileName();
}

void MainWindow::createItemToolBar()
{
    QList<QString> domains = itemDB->getDomains();

    foreach(QString domain, domains)
    {
        ToolWidget* toolWidget = new ToolWidget(this->itemDB, ui->toolBarItems);
        toolWidget->setDomain(domain);
        ui->toolBarItems->addWidget(toolWidget);
        connect(toolWidget, SIGNAL(signal_newItemRequested(CADitemTypes::ItemType)), this, SLOT(slot_createNewItem(CADitemTypes::ItemType)));
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // keyPressEvent does not work because it is forwarded to children
//    qDebug() << "MainWindow::keyPressEvent";
//    switch (event->key())
//    {
//    case Qt::Key_F11:
//        ui->menuBar->setHidden(!ui->menuBar->isHidden());
//        break;
//    }

    event->accept();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();
}

// **** File functions ****

void MainWindow::slot_file_open_action()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this, tr("Load project or drawing"), QString(), tr("XML File (*.xml);;DXF File (*.dxf)"));

    if (filename.isEmpty())
        return;

    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(filename);
    files.prepend(filename);
    while (files.size() > MAX_RECENT_FILES)
        files.removeLast();

    settings.setValue("recentFileList", files);
    updateRecentFileActions();

    if (filename.endsWith(".dxf", Qt::CaseInsensitive))
        slot_file_open_dxf_with_libdxfrw(filename);
    else if (filename.endsWith(".xml", Qt::CaseInsensitive))
        slot_file_open_xml(filename);
}

void MainWindow::slot_file_open_dxf(QString filename)
{
    // Load DXF file into memory:
    CreationInterface* creationInterface = new CreationInterface(this->itemDB);
    DL_Dxf* dxf = new DL_Dxf();
    if (!dxf->in(filename.toStdString(), creationInterface))
    {
        qDebug() << "unable to open file";
        return;
    }

    delete dxf;
    delete creationInterface;


    this->project_filepath = filename;
    this->layerManager->slot_updateAllLayers();
    emit signal_repaintNeeded();
}

void MainWindow::slot_file_open_dxf_with_libdxfrw(QString filename)
{
    QByteArray filename_array = filename.toUtf8();
    char* filename_data = filename_array.data();
    dxfRW reader(filename_data);
    DxfReaderInterface* dxfReaderInterface = new DxfReaderInterface(this->itemDB);
    reader.read(dxfReaderInterface, true);

    delete dxfReaderInterface;
}

void MainWindow::slot_file_open_xml(QString filename)
{
    QString error;
    QMatrix4x4 matrix_projection, matrix_glSelect, matrix_modelview, matrix_rotation;
    bool ok = itemDB->file_loadDB(filename, &error, &matrix_projection, &matrix_glSelect, &matrix_modelview, &matrix_rotation);

    if (!ok)
        QMessageBox::critical(this, tr("Error while loading"), tr("Unable to open or parse file. Error:\n") + error);
    else if (!error.isEmpty())
    {
        QMessageBox::information(this, tr("Information while loading"), error);
    }
    else
    {
        this->project_filepath = filename;
        this->geometryDisplays.at(0)->getWidget()->setMatrices(matrix_projection, matrix_glSelect, matrix_modelview, matrix_rotation);
    }
}

void MainWindow::slot_file_save_action()
{
    QString path = this->project_filepath;

    QString filename;
    if (path.isEmpty())
        filename = QFileDialog::getSaveFileName(this, tr("Save project as file"), path, "XML File (*.xml)");
    else
        filename = path;

    if (filename.isEmpty())
        return;

    if (!filename.endsWith(".xml"))
        filename.append(".xml");

    bool ok = itemDB->file_storeDB(filename, this->geometryDisplays.first()->getWidget()->getMatrix_projection(),
                                   this->geometryDisplays.first()->getWidget()->getMatrix_glSelect(),
                                   this->geometryDisplays.first()->getWidget()->getMatrix_modelview(),
                                   this->geometryDisplays.first()->getWidget()->getMatrix_rotation());

    if (ok)
        this->project_filepath = filename;
    else
        QMessageBox::critical(this, tr("Error while saving"), tr("Unable to write file."));
}

void MainWindow::slot_file_save_as_action()
{
    qDebug() << "slot_file_save_as_action()";

    QString filename;
    filename = QFileDialog::getSaveFileName(this, tr("Save project as file"), this->project_filepath, "XML File (*.xml)");

    if (filename.isEmpty())
        return;

    if (!filename.endsWith(".xml"))
        filename.append(".xml");

    bool ok = itemDB->file_storeDB(filename, this->geometryDisplays.first()->getWidget()->getMatrix_projection(),
                                   this->geometryDisplays.first()->getWidget()->getMatrix_glSelect(),
                                   this->geometryDisplays.first()->getWidget()->getMatrix_modelview(),
                                   this->geometryDisplays.first()->getWidget()->getMatrix_rotation());

    if (ok)
        this->project_filepath = filename;
    else
        QMessageBox::critical(this, tr("Error while saving"), tr("Unable to write file."));
}

void MainWindow::slot_file_print_action()
{
    qDebug() << "slot_file_print_action()";

    printwidget->show();
    printwidget->raise();
}

void MainWindow::slot_file_pdf_export_action()
{
    qDebug() << "slot_file_pdf_export_action()";

    printwidget->show();
    printwidget->raise();

//    QString filename = QFileDialog::getSaveFileName(NULL, "PDF oder PS speichern unter...", QString(), "pdf Datei (*.pdf);;PostScript Datei (*.ps)");
//    if (filename.isEmpty()) return;

//    QPrinter* printer = new QPrinter();
//    printer->setOutputFormat(QPrinter::PdfFormat);
//    printer->setPaperSize(QPrinter::A0);
//    printer->setOrientation(QPrinter::Landscape);
//    printer->setFullPage(true);
//    printer->setResolution(600); // DPI
//    printer->setOutputFileName(filename);

//    // Todo: Print here!

//    delete printer;

//    QDesktopServices::openUrl(QUrl::fromUserInput(filename));
}

void MainWindow::slot_file_close_action()
{
    qDebug() << "slot_file_close_action()";
}

void MainWindow::slot_openRecentFile()
{
    QString filename;
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        filename = action->data().toString();

    if (filename.isEmpty())
        return;

    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(filename);
    files.prepend(filename);
    while (files.size() > MAX_RECENT_FILES)
        files.removeLast();

    settings.setValue("recentFileList", files);
    updateRecentFileActions();

    if (filename.endsWith(".dxf", Qt::CaseInsensitive))
        slot_file_open_dxf_with_libdxfrw(filename);
    else if (filename.endsWith(".xml", Qt::CaseInsensitive))
        slot_file_open_xml(filename);
}

void MainWindow::slot_clearRecentFiles()
{
    QSettings settings;
    settings.setValue("recentFileList", QStringList());
    updateRecentFileActions();
}

// **** Window functions ****

void MainWindow::slot_newGeometryDisplay()
{
    GeometryDisplay* newGeometryDisplay = new GeometryDisplay(itemDB, itemWizard, itemGripModifier, this);
    connect(newGeometryDisplay, SIGNAL(signal_aboutToClose(QAction*)), this, SLOT(slot_geometryDisplayAboutToClose(QAction*)));
    connect(newGeometryDisplay, SIGNAL(signal_highlightItem(CADitem*)), this, SLOT(slot_highlightItem(CADitem*)));
    connect(newGeometryDisplay, SIGNAL(signal_snapFired(QVector3D,int)), this, SLOT(slot_snapTo(QVector3D,int)));
    connect(newGeometryDisplay, SIGNAL(signal_selectionChanged(QList<CADitem*>)), this, SLOT(slot_selectionChanged(QList<CADitem*>)));
    connect(this, SIGNAL(signal_repaintNeeded()), newGeometryDisplay, SIGNAL(signal_repaintNeeded()));
    connect(layerManager, SIGNAL(signal_repaintNeeded()), newGeometryDisplay, SIGNAL(signal_repaintNeeded()));
    connect(itemDB, SIGNAL(signal_repaintNeeded()), newGeometryDisplay, SIGNAL(signal_repaintNeeded()));
    connect(itemDB, SIGNAL(signal_itemDeleted(CADitem*)), newGeometryDisplay, SIGNAL(signal_itemDeleted(CADitem*)));
#ifdef USE_3D_MOUSE
    connect(magellanThread, SIGNAL(signal_mouseCoords(int,int,int,int,int,int)), newGeometryDisplay, SIGNAL(signal_mouse3Dcoords(int,int,int,int,int,int)));
#endif
    connect(settingsDialog, SIGNAL(signal_settingsChanged()), newGeometryDisplay, SIGNAL(signal_settingsChanged()));

    this->addDockWidget(Qt::LeftDockWidgetArea, newGeometryDisplay);
    ui->menuFenster->addAction(newGeometryDisplay->toggleViewAction());

    newGeometryDisplay->setFocusPolicy(Qt::StrongFocus);
    geometryDisplays.append(newGeometryDisplay);
}

void MainWindow::slot_geometryDisplayAboutToClose(QAction *action)
{
    ui->menuFenster->removeAction(action);
}

void MainWindow::slot_highlightItem(CADitem *item)
{
    foreach (GeometryDisplay* gd, geometryDisplays)
    {
        gd->slot_highlightItem(item);
    }
}

void MainWindow::slot_snapTo(QVector3D snapPos_scene, int snapMode)
{
    foreach (GeometryDisplay* gd, geometryDisplays)
    {
        gd->slot_snapTo(snapPos_scene, snapMode);
    }
}

void MainWindow::slot_selectionChanged(QList<CADitem*> selectedItems)
{
    foreach (GeometryDisplay* gd, geometryDisplays)
    {
        gd->slot_changeSelection(selectedItems);
    }
}

void MainWindow::slot_createNewItem(CADitemTypes::ItemType type)
{
    Layer* currentLayer = this->layerManager->getCurrentLayer();
    if (currentLayer == this->itemDB->getTopLevelLayer())
    {
        QMessageBox::critical(this, tr("Item creation"), tr("No layer is selected."));
        return;
    }
    itemDB->setRestorePoint();
    CADitem* item = itemDB->drawItem_withRestorePoint(currentLayer, type, WizardParams());
    this->itemWizard->showWizard(item, itemDB);
}

void MainWindow::on_actionAbout_OpenGL_triggered()
{
    ModalDialog *dialog = new ModalDialog(tr("About %1").arg("OpenGL"), mainGeometryDisplay->getOpenGLinfo(), this);
    dialog->exec();
    delete dialog;
}

void MainWindow::on_actionAbout_triggered()
{
    QStringList about;
    about << tr("Authors") << "Peter Diener, Moritz Sternemann, Sebastian Wolf";
    about << "Build" << QString(__DATE__) + " " + QString(__TIME__);
    about << QCoreApplication::organizationName() << "Rudolf-Diesel-Str. 17";
    about << "" << "82205 Gilching";
    about << tr("Tel.") << "+49 8105 2713 -0";
    about << "Email" << "diener.git@sme-gmbh.com";
    about << "Email" << "sternemann.git@sme-gmbh.com";
    about << "Email" << "wolf.git@sme-gmbh.com";

    ModalDialog *dialog = new ModalDialog(tr("About %1").arg(QCoreApplication::applicationName()), about, this);
    dialog->exec();
    delete dialog;
}

void MainWindow::on_actionSettings_triggered()
{
    settingsDialog->show();
}

void MainWindow::on_actionServer_triggered(bool checked)
{
    // Switch Server function on or off

}

void MainWindow::slot_collision_detected(CADitem *item_1, CADitem *item_2, QVector3D line_1, QVector3D line_2)
{
    CAD_basic_line* line = new CAD_basic_line;
    line->wizardParams.insert("Position x1", line_1.x());
    line->wizardParams.insert("Position y1", line_1.y());
    line->wizardParams.insert("Position z1", line_1.z());
    line->wizardParams.insert("Position x2", line_2.x());
    line->wizardParams.insert("Position y2", line_2.y());
    line->wizardParams.insert("Position z2", line_2.z());
    line->wizardParams.insert("r", 50.0);
    line->processWizardInput();
    line->calculate();
    itemDB->addItem(line, "Collision");

    QString pos_1 = QString().sprintf(" @{%.3lf|%.3lf|%.3lf}", item_1->position.x(), item_1->position.y(), item_1->position.z());
    QString pos_2 = QString().sprintf(" @{%.3lf|%.3lf|%.3lf}", item_2->position.x(), item_2->position.y(), item_2->position.z());
//    if(QMessageBox::question(this, tr("Collision Detection"), tr("There has been a collision between\n")
//                             + item_1->description() + " on " + item_1->layer->name + pos_1 + "\nand\n"
//                             + item_2->description() + " on " + item_2->layer->name + pos_2 + ".\n"
//                             + "Accept anyway?")
//            == QMessageBox::No)
//    {
//        this->itemDB->restore_undo();
//    }

    QMessageBox::information(this, tr("Collision Detection"), tr("There has been a collision between\n")
                             + item_1->description() + " on " + item_1->layer->name + pos_1 + "\nand\n"
                             + item_2->description() + " on " + item_2->layer->name + pos_2 + ".");

    itemDB->changeLayerOfItem(item_1->id, "Collision");
    itemDB->changeLayerOfItem(item_2->id, "Collision");

}
