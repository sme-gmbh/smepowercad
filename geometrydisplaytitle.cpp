#include "geometrydisplaytitle.h"
#include "ui_geometrydisplaytitle.h"

#include <QDebug>

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

void GeometryDisplayTitle::hideButtons()
{
    ui->pushButtonFloat->setHidden(true);
    ui->pushButtonClose->setHidden(true);
    this->layout()->removeItem(ui->layoutButtons);
}

void GeometryDisplayTitle::slot_sceneCoordinatesChanged(QVector3D coords)
{
    ui->labelSceneCoordinates_x->setText(QString().sprintf("x: %010.3lf", coords.x()));
    ui->labelSceneCoordinates_y->setText(QString().sprintf("y: %010.3lf", coords.y()));
    ui->labelSceneCoordinates_z->setText(QString().sprintf("z: %010.3lf", coords.z()));
}

void GeometryDisplayTitle::on_pushButtonFloat_clicked()
{
    emit signal_float();
}

void GeometryDisplayTitle::on_pushButtonClose_clicked()
{
    emit signal_close();
}

void GeometryDisplayTitle::on_pushButtonWireframe_clicked(bool checked)
{
    emit signal_wireframe(checked);
}

void GeometryDisplayTitle::on_pushButtonSolid_clicked(bool checked)
{
    emit signal_solid(checked);
}

void GeometryDisplayTitle::on_comboBox_view_currentIndexChanged(const QString &arg1)
{
    QString str = arg1;
    str.replace(QChar(18, 32), '-');
    emit signal_cuttingplane_changed(str);
}
