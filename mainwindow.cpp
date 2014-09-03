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
    statusBar_sceneCoordinate = QVector3D();
    statusBar_selectionCount = 0;

    current_cadline = NULL;

    ui->dockWidgetPrompt->setTitleBarWidget(new QWidget());

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

    // **** Main Menu Bar ****
    QMenu* menu_file = new QMenu("Datei");
    QMenu* menu_draw = new QMenu("Zeichnen");
    QMenu* menu_format = new QMenu("Format");
    QMenu* menu_fenster = new QMenu("Fenster");


    // **** File Menu ****
    // test QIcon::fromTheme("document-save");
    menu_file->addAction(QIcon::fromTheme("document-open"), "Laden...", this, SLOT(slot_file_open_action()), QKeySequence::Open)->setIconVisibleInMenu(true);
    menu_file->addAction(QIcon::fromTheme("document-save"), "Speichern", this, SLOT(slot_file_save_action()), QKeySequence::Save)->setIconVisibleInMenu(true);
    menu_file->addAction(QIcon::fromTheme("document-save-as"), "Speichern unter...", this, SLOT(slot_file_save_as_action()), QKeySequence::SaveAs)->setIconVisibleInMenu(true);
    menu_file->addSeparator();
    menu_file->addAction(QIcon::fromTheme("document-print"), "Plotten...", this, SLOT(slot_file_print_action()), QKeySequence::Print)->setIconVisibleInMenu(true);
    menu_file->addAction(QIcon::fromTheme("gnome-mime-application-pdf"), "Pdf...", this, SLOT(slot_file_pdf_export_action()))->setIconVisibleInMenu(true);
    menu_file->addSeparator();
    menu_file->addAction(QIcon::fromTheme("window-close"), "Zeichnung schließen", this, SLOT(slot_file_close_action()), QKeySequence::Close)->setIconVisibleInMenu(true);
    menu_file->addAction(QIcon::fromTheme("gtk-quit"), "Beenden", qApp, SLOT(quit()), QKeySequence::Quit)->setIconVisibleInMenu(true);

    // **** Draw Menu ****
    menu_draw->addAction(QIcon("://icons/Linie.svg"), "Linie", this, SLOT(slot_draw_line_action()), QKeySequence("L"))->setIconVisibleInMenu(true);
    menu_draw->addAction(QIcon("://icons/Bogen.svg"), "Bogen", this, SLOT(slot_draw_arc_action()), QKeySequence("B"))->setIconVisibleInMenu(true);
    menu_draw->addAction(QIcon("://icons/Kreis.svg"), "Kreis", this, SLOT(slot_draw_circle_action()), QKeySequence("K"))->setIconVisibleInMenu(true);

    // **** Format Menu ****

    // **** Fenster Menu ****
    menu_fenster->addAction(QIcon(), "Neues Schnittfenster", this, SLOT(slot_newGeometryDisplay()), QKeySequence("G"))->setIconVisibleInMenu(true);
    menu_fenster->addActions(actionList_Fenster);

    // ** Layer Manager **

    this->layerManager = new LayerManager(this, topLevelLayer);
    QAction* action_layerManager = this->layerManager->toggleViewAction();
    action_layerManager->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    connect(itemDB, SIGNAL(signal_layerAdded(Layer*,Layer*)), layerManager, SLOT(slot_layerAdded(Layer*,Layer*)));

    menu_format->addAction(action_layerManager);

    // **** CAD window *****

//    this->cadview = new CADview();
//    connect(cadview, SIGNAL(signal_sceneCoordinateChanged(QPointF)), this, SLOT(slot_sceneCoordinateChanged(QPointF)));
//    connect(cadview, SIGNAL(signal_selectionCountChanged(int)), this, SLOT(slot_selectionCountChanged(int)));

    // **** CAD window (2nd version) *****
    mainGeometryDisplay = new GeometryDisplay(itemDB, this);
    //mainGeometryDisplay->setTitleBarWidget(new QWidget());
    connect(mainGeometryDisplay, SIGNAL(signal_sceneCoordinateChanged(QVector3D)), this, SLOT(slot_sceneCoordinateChanged(QVector3D)));
    //this->addDockWidget(Qt::TopDockWidgetArea, geometryDisplay);
    mainGeometryDisplay->setFeatures(QDockWidget::NoDockWidgetFeatures);
    this->setCentralWidget(this->mainGeometryDisplay);

    // **** CAD window (2nd version) *****
//    this->geometryDisplay = new GeometryDisplay(itemDB, this);
//    connect(geometryDisplay, SIGNAL(signal_sceneCoordinateChanged(QVector3D)), this, SLOT(slot_sceneCoordinateChanged(QVector3D)));
//    this->addDockWidget(Qt::TopDockWidgetArea, geometryDisplay);
//    this->actionList_Fenster.append(geometryDisplay->toggleViewAction());
    //slot_newGeometryDisplay();



    // **** Main Menu fillup ****
    ui->menuBar->addMenu(menu_file);
    ui->menuBar->addMenu(menu_draw);
    ui->menuBar->addMenu(menu_format);
    ui->menuBar->addMenu(menu_fenster);







    // ***** Spielwiese *****

    // Text
//    CADtext* cadtext = new CADtext("SME PowerCAD");
//    this->scene->addItem(cadtext);
//    cadtext->setScale(5.0);


    // Linie
//    CADline* cadline = new CADline();
//    this->scene->addItem(cadline);
//    cadline->setLine(0, 0, 300, 300);
}

MainWindow::~MainWindow()
{
    delete layerManager;
    delete itemDB;
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    foreach (GeometryDisplay* gd, geometryDisplays)
    {
        gd->slot_changeCuttingplane();
    }
//    QMainWindow::keyPressEvent(event);
    event->accept();
}

// **** Status bar functions ****

void MainWindow::slot_sceneCoordinateChanged(QVector3D pos)
{
    this->statusBar_sceneCoordinate = pos;
    this->slot_update_statusBar();
}

void MainWindow::slot_selectionCountChanged(int num)
{
    this->statusBar_selectionCount = num;
    this->slot_update_statusBar();
}

void MainWindow::slot_update_statusBar()
{
    QString statusString = QString().sprintf("[%010.3lf, %010.3lf, %010.3lf]  |  ", this->statusBar_sceneCoordinate.x(), this->statusBar_sceneCoordinate.y(), this->statusBar_sceneCoordinate.z());
    statusString += QString().sprintf("%i selektiert", this->statusBar_selectionCount);

    this->statusBar()->showMessage(statusString);
}

// **** File functions ****

void MainWindow::slot_file_open_action()
{
    qDebug() << "slot_file_open_action()";

    QString filename;
    filename = QFileDialog::getOpenFileName(this, "Zeichnung laden", QString(), "dxf-Datei (*.dxf)");

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

// **** Window functions ****

void MainWindow::slot_newGeometryDisplay()
{
    GeometryDisplay* newGeometryDisplay = new GeometryDisplay(itemDB, this);
    connect(newGeometryDisplay, SIGNAL(signal_sceneCoordinateChanged(QVector3D)), this, SLOT(slot_sceneCoordinateChanged(QVector3D)));
    connect(this, SIGNAL(signal_repaintNeeded()), newGeometryDisplay, SLOT(slot_redrawScene()));
    connect(layerManager, SIGNAL(signal_repaintNeeded()), newGeometryDisplay, SLOT(slot_redrawScene()));
    this->addDockWidget(Qt::TopDockWidgetArea, newGeometryDisplay);
    this->actionList_Fenster.append(newGeometryDisplay->toggleViewAction());
    newGeometryDisplay->setFocusPolicy(Qt::StrongFocus);
    geometryDisplays.append(newGeometryDisplay);
}

/*void MainWindow::on_doubleSpinBox_heightOfIntersection_valueChanged(double arg1)
{
    foreach (GeometryDisplay* gd, geometryDisplays)
        gd->slot_setHeightOfIntersection(arg1);
}*/
