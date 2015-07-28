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

#include "geometrydisplaytitle.h"
#include "ui_geometrydisplaytitle.h"

#include <QDebug>

GeometryDisplayTitle::GeometryDisplayTitle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GeometryDisplayTitle)
{
    ui->setupUi(this);
    ui->comboBox_view->setCurrentIndex(5);
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

void GeometryDisplayTitle::slot_selectionCountChanged(int num)
{
    ui->labelSelectionCount->setText(QString().setNum(num));
}

void GeometryDisplayTitle::slot_setDirectionOfViewInvalid()
{
    ui->comboBox_view->setCurrentIndex(-1);
}

void GeometryDisplayTitle::on_pushButtonFloat_clicked()
{
    emit signal_float();
}

void GeometryDisplayTitle::on_pushButtonClose_clicked()
{
    emit signal_close();
}

void GeometryDisplayTitle::on_pushButtonPerspective_clicked(bool checked)
{
    emit signal_perspective(checked);
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

void GeometryDisplayTitle::on_spinBoxDepthOfView_valueChanged(const QString &arg1)
{
    emit signal_cuttingplane_values_changed(this->ui->spinBoxHeightOfIntersection->value(), this->ui->spinBoxDepthOfView->value());
}

void GeometryDisplayTitle::on_spinBoxHeightOfIntersection_valueChanged(const QString &arg1)
{
    emit signal_cuttingplane_values_changed(this->ui->spinBoxHeightOfIntersection->value(), this->ui->spinBoxDepthOfView->value());
}

