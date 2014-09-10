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
    connect(ui->actionLinie, SIGNAL(triggered()), this, SLOT(slot_draw_line_action()));
    connect(ui->actionBogen, SIGNAL(triggered()), this, SLOT(slot_draw_arc_action()));
    connect(ui->actionKreis, SIGNAL(triggered()), this, SLOT(slot_draw_circle_action()));
    connect(ui->actionNeues_Schnittfenster, SIGNAL(triggered()), this, SLOT(slot_newGeometryDisplay()));


    // ** Layer Manager **
    this->layerManager = new LayerManager(this, topLevelLayer);
    QAction* action_layerManager = this->layerManager->toggleViewAction();
    action_layerManager->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    connect(itemDB, SIGNAL(signal_layerAdded(Layer*,Layer*)), layerManager, SLOT(slot_layerAdded(Layer*,Layer*)));
    ui->menuFormat->addAction(action_layerManager);


    // **** CAD window (2nd version) *****
    mainGeometryDisplay = new GeometryDisplay(itemDB, this);
    connect(this, SIGNAL(signal_repaintNeeded()), mainGeometryDisplay, SIGNAL(signal_repaintNeeded()));
    connect(layerManager, SIGNAL(signal_repaintNeeded()), mainGeometryDisplay, SIGNAL(signal_repaintNeeded()));
    mainGeometryDisplay->setFeatures(QDockWidget::NoDockWidgetFeatures);
    mainGeometryDisplay->setAllowedAreas(Qt::NoDockWidgetArea);
    mainGeometryDisplay->hideButtons();
    this->setCentralWidget(this->mainGeometryDisplay);


    // **** Toolbar toggles ****
    ui->menuWerkzeugleisten->addAction(ui->toolBarPrimitiv->toggleViewAction());
    ui->menuWerkzeugleisten->addAction(ui->toolBarArchitektur->toggleViewAction());
    ui->menuWerkzeugleisten->addAction(ui->toolBarLuft->toggleViewAction());



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




    // ***** Spielwiese *****

    Layer* layer = itemDB->addLayer("Testlayer");
    layer->pen.setColor(Qt::gray);
    layer->brush.setColor(Qt::yellow);
    layer->lineType = "Continuos";
    layer->width = 1;

    CAD_basic_box* box = new CAD_basic_box();
    itemDB->addItem(box, "Testlayer");

    CAD_basic_circle* circle = new CAD_basic_circle();
    circle->center = QVector3D(-2.0, 0.0, 0.0);
    itemDB->addItem(circle, "Testlayer");

    CAD_basic_cylinder* cylinder = new CAD_basic_cylinder();
    cylinder->center_base = QVector3D(3.0, 0.0, 0.0);
    itemDB->addItem(cylinder, "Testlayer");

    this->layerManager->updateAllLayers();
    emit signal_repaintNeeded();
}

MainWindow::~MainWindow()
{
    delete mainGeometryDisplay;
    delete layerManager;
    delete itemDB;
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

// **** Drawing functions ****

void MainWindow::slot_draw_line_action()
{
    qDebug() << "slot_draw_line_action()";

    // Linie


    //connect(cadview, SIGNAL(signal_sceneCoordinateChanged(QPointF)), current_cadline, SLOT(slot_setStopPoint(QPointF)));

//    cadview->slot_startLine(QPointF(0,0));
//    cadview->slot_finishLine(QPointF(300,300));
}

void MainWindow::slot_draw_arc_action()
{
//    qDebug() << "slot_draw_arc_action()";

//    // Bogen
//    CADarc* cadarc = new CADarc();
//    this->scene->addItem(cadarc);
//    cadarc->setCenter(QPointF(10.0, 20.0));
//    cadarc->setRadius(20.0);
//    cadarc->setStartAngle(0.0);
//    cadarc->setSpanAngle(360.0);
}

void MainWindow::slot_draw_circle_action()
{
//    qDebug() << "slot_draw_circle_action()";

//    // Kreis
//    CADcircle* cadcircle = new CADcircle();
//    this->scene->addItem(cadcircle);
//    cadcircle->setCenter(QPointF(10.0, 0.0));
//    cadcircle->setRadius(20.0);
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
    GeometryDisplay* newGeometryDisplay = new GeometryDisplay(itemDB, this);
    connect(newGeometryDisplay, SIGNAL(signal_aboutToClose(QAction*)), this, SLOT(slot_geometryDisplayAboutToClose(QAction*)));
    connect(this, SIGNAL(signal_repaintNeeded()), newGeometryDisplay, SIGNAL(signal_repaintNeeded()));
    connect(layerManager, SIGNAL(signal_repaintNeeded()), newGeometryDisplay, SIGNAL(signal_repaintNeeded()));
    this->addDockWidget(Qt::LeftDockWidgetArea, newGeometryDisplay);
    ui->menuFenster->addAction(newGeometryDisplay->toggleViewAction());

    newGeometryDisplay->setFocusPolicy(Qt::StrongFocus);
    geometryDisplays.append(newGeometryDisplay);
}

void MainWindow::slot_geometryDisplayAboutToClose(QAction *action)
{
    ui->menuFenster->removeAction(action);
}
