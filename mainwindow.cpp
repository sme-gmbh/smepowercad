#include <QDebug>
#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dxflib/src/dl_dxf.h"
#include "dxflib/src/dl_creationadapter.h"
#include "creationinterface.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // **** Global Variables ****
    current_cadline = NULL;

    // **** Settings Dialog ****
    settingsDialog = new SettingsDialog(this);

    // **** Item Wizard ****
    itemWizard = new ItemWizard(this);
    connect(itemWizard, SIGNAL(signal_sceneRepaintNeeded()), this, SIGNAL(signal_repaintNeeded()));

    // **** Command prompt ****
    QWidget *promptTitle = new QWidget(ui->dockWidgetPrompt);
    promptTitle->setMaximumWidth(0);
    promptTitle->setMaximumHeight(0);
    ui->dockWidgetPrompt->setTitleBarWidget(promptTitle);


    // **** CAD Item Database *****
    itemDB = new ItemDB(this);
    Layer* topLevelLayer = itemDB->getTopLevelLayer();


    // **** CAD command interpreter ****
    this->cadcommand = new CADcommand(this);
    connect(this, SIGNAL(signal_command_prompt_input(QString)), cadcommand, SLOT(slot_prompt_input(QString)));
    connect(cadcommand, SIGNAL(signal_prompt_output(QString)), ui->prompt_output, SLOT(appendPlainText(QString)));

//    connect(cadcommand, SIGNAL(signal_startLine(QPointF)), cadview, SLOT(slot_startLine(QPointF)));
//    connect(cadcommand, SIGNAL(signal_update_line(QPointF)), cadview, SLOT(slot_update_line(QPointF)));
//    connect(cadcommand, SIGNAL(signal_finishLine(QPointF)), cadview, SLOT(slot_finishLine(QPointF)));
//    connect(cadcommand, SIGNAL(signal_abort()), cadview, SLOT(slot_abort()));


    // **** Menubar actions ****
    connect(ui->actionLaden, SIGNAL(triggered()),           this, SLOT(slot_file_open_action()));
    connect(ui->actionSpeichern, SIGNAL(triggered()),       this, SLOT(slot_file_save_action()));
    connect(ui->actionSpeichern_unter, SIGNAL(triggered()), this, SLOT(slot_file_save_as_action()));
    connect(ui->actionPlotten, SIGNAL(triggered()),         this, SLOT(slot_file_print_action()));
    connect(ui->actionPDF, SIGNAL(triggered()),             this, SLOT(slot_file_pdf_export_action()));
    connect(ui->actionZeichnungClose, SIGNAL(triggered()),  this, SLOT(slot_file_close_action()));
    connect(ui->actionBeenden, SIGNAL(triggered()),         qApp, SLOT(quit()));
    connect(ui->actionNeues_Schnittfenster, SIGNAL(triggered()), this, SLOT(slot_newGeometryDisplay()));


    // ** Layer Manager **
    this->layerManager = new LayerManager(this, topLevelLayer, itemDB);
    QAction* action_layerManager = this->layerManager->toggleViewAction();
    action_layerManager->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    connect(itemDB, SIGNAL(signal_layerAdded(Layer*,Layer*)), layerManager, SLOT(slot_layerAdded(Layer*,Layer*)));
    ui->menuFormat->addAction(action_layerManager);


    // **** CAD window (2nd version) *****
    mainGeometryDisplay = new GeometryDisplay(itemDB, itemWizard, this);
    connect(this, SIGNAL(signal_repaintNeeded()), mainGeometryDisplay, SIGNAL(signal_repaintNeeded()));
    connect(layerManager, SIGNAL(signal_repaintNeeded()), mainGeometryDisplay, SIGNAL(signal_repaintNeeded()));
    connect(settingsDialog, SIGNAL(signal_settingsChanged()), mainGeometryDisplay, SIGNAL(signal_settingsChanged()));
    connect(mainGeometryDisplay, SIGNAL(signal_highlightItem(CADitem*)), this, SLOT(slot_highlightItem(CADitem*)));
    connect(mainGeometryDisplay, SIGNAL(signal_snapFired(QVector3D,int)), this, SLOT(slot_snapTo(QVector3D,int)));
    connect(mainGeometryDisplay, SIGNAL(signal_selectionChanged(QList<CADitem*>)), this, SLOT(slot_selectionChanged(QList<CADitem*>)));
    mainGeometryDisplay->setFeatures(QDockWidget::NoDockWidgetFeatures);
    mainGeometryDisplay->setAllowedAreas(Qt::NoDockWidgetArea);
    mainGeometryDisplay->hideButtons();
    geometryDisplays.append(mainGeometryDisplay);       // Test if this works!
    this->setCentralWidget(this->mainGeometryDisplay);


    // **** Toolbar toggles ****
    ui->menuWerkzeugleisten->addAction(ui->toolBarBasic->toggleViewAction());
    ui->menuWerkzeugleisten->addAction(ui->toolBarArchitecture->toggleViewAction());
    ui->menuWerkzeugleisten->addAction(ui->toolBarAir->toggleViewAction());



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


    // **** 3D mouse ****
    magellanThread = new QMagellanThread();
    connect(magellanThread, SIGNAL(signal_mouseCoords(int,int,int,int,int,int)), mainGeometryDisplay, SIGNAL(signal_mouse3Dcoords(int,int,int,int,int,int)));
    //connect(magellanThread, SIGNAL(signal_buttonPressed(int)), this, SLOT());
    //connect(magellanThread, SIGNAL(signal_buttonReleased(int)), this, SLOT());
    magellanThread->start();




    // ***** Spielwiese *****

    Layer* layer = itemDB->addLayer("Testlayer");
    layer->pen.setColor(Qt::gray);
    layer->brush.setColor(Qt::white);
    layer->lineType = "Continuos";
    layer->width = 1;

    Layer* layerX = itemDB->addLayer("X");
    layerX->pen.setColor(Qt::gray);
    layerX->brush.setColor(Qt::red);
    layerX->lineType = "Continuos";
    layerX->width = 1;

    Layer* layerY = itemDB->addLayer("Y");
    layerY->pen.setColor(Qt::gray);
    layerY->brush.setColor(Qt::green);
    layerY->lineType = "Continuos";
    layerY->width = 1;

    Layer* layerZ = itemDB->addLayer("Z");
    layerZ->pen.setColor(Qt::gray);
    layerZ->brush.setColor(Qt::blue);
    layerZ->lineType = "Continuos";
    layerZ->width = 1;


    CAD_basic_box* item;

    item = (CAD_basic_box*)itemDB->drawItem("Testlayer", CADitem::Basic_Box);
    item->position = QVector3D(0.0, 0.0, 0.0);

    item->wizardParams.insert(QObject::tr("Center x"), QVariant::fromValue(0.0));
    item->wizardParams.insert(QObject::tr("Center y"), QVariant::fromValue(0.0));
    item->wizardParams.insert(QObject::tr("Center z"), QVariant::fromValue(0.0));
    item->wizardParams.insert(QObject::tr("Size x"), QVariant::fromValue(10.0));
    item->wizardParams.insert(QObject::tr("Size y"), QVariant::fromValue(10.0));
    item->wizardParams.insert(QObject::tr("Size z"), QVariant::fromValue(10.0));

    item->processWizardInput();
    item->calculate();

    item = (CAD_basic_box*)itemDB->drawItem("X", CADitem::Basic_Box);
    item->position = QVector3D(0.0, 0.0, 0.0);

    item->wizardParams.insert(QObject::tr("Center x"), QVariant::fromValue(15.0));
    item->wizardParams.insert(QObject::tr("Center y"), QVariant::fromValue(0.0));
    item->wizardParams.insert(QObject::tr("Center z"), QVariant::fromValue(0.0));
    item->wizardParams.insert(QObject::tr("Size x"), QVariant::fromValue(5.0));
    item->wizardParams.insert(QObject::tr("Size y"), QVariant::fromValue(5.0));
    item->wizardParams.insert(QObject::tr("Size z"), QVariant::fromValue(5.0));

    item->processWizardInput();
    item->calculate();

    item = (CAD_basic_box*)itemDB->drawItem("Y", CADitem::Basic_Box);
    item->position = QVector3D(0.0, 0.0, 0.0);

    item->wizardParams.insert(QObject::tr("Center x"), QVariant::fromValue(0.0));
    item->wizardParams.insert(QObject::tr("Center y"), QVariant::fromValue(15.0));
    item->wizardParams.insert(QObject::tr("Center z"), QVariant::fromValue(0.0));
    item->wizardParams.insert(QObject::tr("Size x"), QVariant::fromValue(4.0));
    item->wizardParams.insert(QObject::tr("Size y"), QVariant::fromValue(4.0));
    item->wizardParams.insert(QObject::tr("Size z"), QVariant::fromValue(4.0));

    item->processWizardInput();
    item->calculate();

    item = (CAD_basic_box*)itemDB->drawItem("Z", CADitem::Basic_Box);
    item->position = QVector3D(0.0, 0.0, 0.0);

    item->wizardParams.insert(QObject::tr("Center x"), QVariant::fromValue(0.0));
    item->wizardParams.insert(QObject::tr("Center y"), QVariant::fromValue(0.0));
    item->wizardParams.insert(QObject::tr("Center z"), QVariant::fromValue(15.0));
    item->wizardParams.insert(QObject::tr("Size x"), QVariant::fromValue(3.0));
    item->wizardParams.insert(QObject::tr("Size y"), QVariant::fromValue(3.0));
    item->wizardParams.insert(QObject::tr("Size z"), QVariant::fromValue(3.0));

    item->processWizardInput();
    item->calculate();


    this->layerManager->updateAllLayers();


//    emit signal_repaintNeeded();
}

MainWindow::~MainWindow()
{
    magellanThread->terminate();
    delete magellanThread;
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

    qDebug() << numRecentFiles;

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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
//    foreach (GeometryDisplay* gd, geometryDisplays)
//    {
//        gd->slot_changeCuttingplane();
//    }
//    QMainWindow::keyPressEvent(event);

    event->accept();
}

// **** File functions ****

void MainWindow::slot_file_open_action()
{
    qDebug() << "slot_file_open_action()";

    QString filename;
    filename = QFileDialog::getOpenFileName(this, tr("Load drawing"), QString(), tr("dxf-Datei (*.dxf)"));

    if (filename.isEmpty())
        return;

    qDebug() << "loading file: " + filename;

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

    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(filename);
    files.prepend(filename);
    while (files.size() > MAX_RECENT_FILES)
        files.removeLast();

    settings.setValue("recentFileList", files);
    updateRecentFileActions();

    this->layerManager->updateAllLayers();
    emit signal_repaintNeeded();
}

void MainWindow::slot_file_save_action()
{
    qDebug() << "slot_file_save_action()";
}

void MainWindow::slot_file_save_as_action()
{
    qDebug() << "slot_file_save_as_action()";
}

void MainWindow::slot_file_print_action()
{
    qDebug() << "slot_file_print_action()";
}

void MainWindow::slot_file_pdf_export_action()
{
    qDebug() << "slot_file_pdf_export_action()";
}

void MainWindow::slot_file_close_action()
{
    qDebug() << "slot_file_close_action()";
}

void MainWindow::on_prompt_input_returnPressed()
{
    QString str = ui->prompt_input->text();
    ui->prompt_input->clear();

    ui->prompt_output->appendPlainText(str);
    emit signal_command_prompt_input(str);
}

void MainWindow::slot_openRecentFile()
{
    qDebug() << "slot_openRecentFile()";

    QString filename;
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        filename = action->data().toString();

    if (filename.isEmpty())
        return;

    qDebug() << "loading file: " + filename;

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

    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(filename);
    files.prepend(filename);
    while (files.size() > MAX_RECENT_FILES)
        files.removeLast();

    settings.setValue("recentFileList", files);
    updateRecentFileActions();

    this->layerManager->updateAllLayers();
    emit signal_repaintNeeded();
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
    GeometryDisplay* newGeometryDisplay = new GeometryDisplay(itemDB, itemWizard, this);
    connect(newGeometryDisplay, SIGNAL(signal_aboutToClose(QAction*)), this, SLOT(slot_geometryDisplayAboutToClose(QAction*)));
    connect(newGeometryDisplay, SIGNAL(signal_highlightItem(CADitem*)), this, SLOT(slot_highlightItem(CADitem*)));
    connect(newGeometryDisplay, SIGNAL(signal_snapFired(QVector3D,int)), this, SLOT(slot_snapTo(QVector3D,int)));
    connect(newGeometryDisplay, SIGNAL(signal_selectionChanged(QList<CADitem*>)), this, SLOT(slot_selectionChanged(QList<CADitem*>)));
    connect(this, SIGNAL(signal_repaintNeeded()), newGeometryDisplay, SIGNAL(signal_repaintNeeded()));
    connect(layerManager, SIGNAL(signal_repaintNeeded()), newGeometryDisplay, SIGNAL(signal_repaintNeeded()));
    connect(magellanThread, SIGNAL(signal_mouseCoords(int,int,int,int,int,int)), newGeometryDisplay, SIGNAL(signal_mouse3Dcoords(int,int,int,int,int,int)));
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
    about << "Email" << "diener@sme-gmbh.com";
    about << "Email" << "moritz.sternemann@web.de";
    about << "Email" << "sebwolf@web.de";

    ModalDialog *dialog = new ModalDialog(tr("About %1").arg(QCoreApplication::applicationName()), about, this);
    dialog->exec();
    delete dialog;
}

void MainWindow::on_actionSettings_triggered()
{
    //SettingsDialog *dialog = new SettingsDialog(this);
    //dialog->exec();
    //delete dialog;
    settingsDialog->show();
}

void MainWindow::on_action_basic_face_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Basic_Face);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_basic_arc_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Basic_Arc);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_basic_box_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

//    CAD_basic_box* box = new CAD_basic_box();
//    box->calculate();
//    itemDB->addItem(box, "Testlayer");

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Basic_Box);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_basic_circle_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

//    CAD_basic_circle* circle = new CAD_basic_circle();
//    circle->center = QVector3D(-2.0, 0.0, 0.0);
//    circle->calculate();
//    itemDB->addItem(circle, "Testlayer");

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Basic_Circle);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_basic_cylinder_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Basic_Cylinder);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_basic_pipe_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Basic_Pipe);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_basic_linie_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Basic_Line);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_basic_plane_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Basic_Plane);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_basic_point_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Basic_Point);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_basic_polylinie_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Basic_Polyline);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_basic_sphere_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Basic_Sphere);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_arch_beam_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Arch_Beam);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_arch_blockout_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Arch_BlockOut);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_arch_door_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Arch_Door);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_arch_levelSlab_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Arch_LevelSlab);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_arch_support_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Arch_Support);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_arch_wall_loadbearing_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Arch_Wall_loadBearing);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_arch_wall_nonloadbearing_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Arch_Wall_nonLoadBearing);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_arch_window_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Arch_Window);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_canvas_flange_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_CanvasFlange);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_duct_baffle_silencer_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_DuctBaffleSilencer);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_duct_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_Duct);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_duct_end_plate_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_DuctEndPlate);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_duct_fire_damper_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_DuctFireDamper);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_duct_fire_resistant_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_DuctFireResistant);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_duct_T_Connector_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_DuctTeeConnector);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_duct_transition_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_DuctTransition);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_duct_transition_rect_round_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_DuctTransitionRectRound);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_duct_turn_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_DuctTurn);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_duct_volumetric_flow_controller_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_DuctVolumetricFlowController);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_duct_Y_piece_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_DuctYpiece);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_empty_cabinet_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_EmptyCabinet);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_equipment_frame_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_EquipmentFrame);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_fan_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_Fan);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_filter_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_Filter);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_heat_exchanger_air_air_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_HeatExchangerAirAir);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_heat_exchanger_water_air_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_HeatExchangerWaterAir);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_Humidifier_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_Humidifier);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_multi_leaf_damper_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_MultiLeafDamper);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_pipe_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_Pipe);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_pipe_end_cap_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_PipeEndCap);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_pipe_fire_damper_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_PipeFireDamper);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_pipe_reducer_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_PipeReducer);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_pipe_silencer_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_PipeSilencer);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_pipe_T_Connector_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_PipeTeeConnector);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_pipe_turn_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_PipeTurn);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_pipe_volumetric_flow_controller_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_PipeVolumetricFlowController);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_pressure_relief_damper_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_PressureReliefDamper);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_air_throttle_valve_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Air_ThrottleValve);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_Adjustvalve_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_Adjustvalve);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_ballvalve_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_BallValve);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_boiler_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_Boiler);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_butterflyvalve_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_ButterflyValve);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_chiller_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_Chiller);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_controlvalve_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_Controlvalve);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_coolingtower_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_CoolingTower);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_expansionchamber_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_ExpansionChamber);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_filter_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_Filter);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_flange_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_Flange);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_flowmeter_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_Flowmeter);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_heatexchanger_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_HeatExchanger);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_pipe_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_Pipe);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_pipeendcap_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_PipeEndCap);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_pipereducer_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_PipeReducer);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_pipeteeconnector_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_PipeTeeConnector);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_pipeturn_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_PipeTurn);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_pump_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_Pump);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_radiator_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_Radiator);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_safetyvalve_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_SafetyValve);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_sensor_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_Sensor);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_storageboiler_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_StorageBoiler);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_heatcool_waterheater_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::HeatCool_WaterHeater);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_sprinkler_compressedairwatercontainer_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Sprinkler_CompressedAirWaterContainer);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_sprinkler_distribution_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Sprinkler_Distribution);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_sprinkler_head_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Sprinkler_Head);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_sprinkler_pipe_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Sprinkler_Pipe);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_sprinkler_pipeturn_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Sprinkler_PipeTurn);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_sprinkler_Pump_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Sprinkler_Pump);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_sprinkler_teeconnector_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Sprinkler_TeeConnector);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_sprinkler_valve_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Sprinkler_Valve);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_sprinkler_wetalarmvalve_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Sprinkler_WetAlarmValve);
    this->itemWizard->showWizard(item);
}

void MainWindow::on_action_sprinkler_zonecheck_triggered()
{
    qDebug() << "Action name" << ((QAction*)sender())->objectName();
    qDebug() << "Action text" << ((QAction*)sender())->text();

    CADitem* item = itemDB->drawItem(this->layerManager->getCurrentLayer(), CADitem::Sprinkler_ZoneCheck);
    this->itemWizard->showWizard(item);
}

