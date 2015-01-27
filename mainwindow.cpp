#include <QDebug>
#include <QFileDialog>
#include <QToolBar>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dxflib/src/dl_dxf.h"
#include "dxflib/src/dl_creationadapter.h"
#include "creationinterface.h"
#include "toolwidget.h"

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


    // ** Layer Manager **
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


    // **** Toolbar toggles ****
    this->createItemToolBar();
    ui->menuWerkzeugleisten->addAction(ui->toolBarItems->toggleViewAction());



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
    magellanThread = new QMagellanThread();
    connect(magellanThread, SIGNAL(signal_mouseCoords(int,int,int,int,int,int)), mainGeometryDisplay, SIGNAL(signal_mouse3Dcoords(int,int,int,int,int,int)));
    //connect(magellanThread, SIGNAL(signal_buttonPressed(int)), this, SLOT());
    //connect(magellanThread, SIGNAL(signal_buttonReleased(int)), this, SLOT());
    magellanThread->start();




    // ***** Spielwiese *****

//    Layer* layer = itemDB->addLayer("Testlayer");
//    layer->pen.setColor(Qt::gray);
//    layer->brush.setColor(Qt::white);
//    layer->lineType = "Continuos";
//    layer->width = 1;

    Layer* layerX = itemDB->addLayer("X");
    layerX->pen.setColor(Qt::gray);
    layerX->brush.setColor(Qt::red);
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

    this->layerManager->slot_updateAllLayers();


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
        slot_file_open_dxf(filename);
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

void MainWindow::slot_file_open_xml(QString filename)
{
    bool ok = itemDB->file_loadDB(filename);
    if (!ok)
        QMessageBox::critical(this, tr("Error while loading"), tr("Unable to open or parse file."));
    else
        this->project_filepath = filename;
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

    bool ok = itemDB->file_storeDB(filename);

    if (ok)
        this->project_filepath = filename;
    else
        QMessageBox::critical(this, tr("Error while saving"), tr("Unable to write file."));
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

//void MainWindow::on_prompt_input_returnPressed()
//{
//    QString str = ui->prompt_input->text();
//    ui->prompt_input->clear();

//    ui->prompt_output->appendPlainText(str);
//    emit signal_command_prompt_input(str);
//}

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
        slot_file_open_dxf(filename);
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

void MainWindow::slot_createNewItem(CADitemTypes::ItemType type)
{
    Layer* currentLayer = this->layerManager->getCurrentLayer();
    if (currentLayer == this->itemDB->getTopLevelLayer())
    {
        QMessageBox::critical(this, tr("Item creation"), tr("No layer is selected."));
        return;
    }
    CADitem* item = itemDB->drawItem(currentLayer, type);
    this->itemWizard->showWizard(item);
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

void MainWindow::on_actionServer_triggered(bool checked)
{
    // Switch Server function on or off

}
