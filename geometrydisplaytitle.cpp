#include "geometrydisplaytitle.h"
#include "ui_geometrydisplaytitle.h"

GeometryDisplayTitle::GeometryDisplayTitle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GeometryDisplayTitle)
{
    ui->setupUi(this);
}

GeometryDisplayTitle::~GeometryDisplayTitle()
{
    delete ui;
}

void GeometryDisplayTitle::slot_sceneCoordinatesChanged(QVector3D coords)
{
    ui->labelSceneCoordinates_x->setText(QString().sprintf("x: %010.3lf", coords.x()));
    ui->labelSceneCoordinates_y->setText(QString().sprintf("y: %010.3lf", coords.y()));
    ui->labelSceneCoordinates_z->setText(QString().sprintf("z: %010.3lf", coords.z()));
}
