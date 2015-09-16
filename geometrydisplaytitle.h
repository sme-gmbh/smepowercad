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

#ifndef GEOMETRYDISPLAYTITLE_H
#define GEOMETRYDISPLAYTITLE_H

#include <QVector3D>
#include <QWidget>

namespace Ui {
class GeometryDisplayTitle;
}

class GeometryDisplayTitle : public QWidget
{
    Q_OBJECT

public:
    explicit GeometryDisplayTitle(QWidget *parent = 0);
    ~GeometryDisplayTitle();

    void hideButtons();

private:
    Ui::GeometryDisplayTitle *ui;

public slots:
    void slot_sceneCoordinatesChanged(QVector3D coords);
    void slot_selectionCountChanged(int num);
    void slot_setDirectionOfViewInvalid();

private slots:
    void on_pushButtonFloat_clicked();
    void on_pushButtonClose_clicked();
    void on_pushButtonPerspective_clicked(bool checked);
    void on_pushButtonWireframe_clicked(bool checked);
    void on_pushButtonSolid_clicked(bool checked);

    void on_comboBox_view_currentIndexChanged(const QString &arg1);

    void on_spinBoxDepthOfView_valueChanged(const QString &arg1);
    void on_spinBoxHeightOfIntersection_valueChanged(const QString &arg1);


signals:
    void signal_float();
    void signal_close();
    void signal_perspective(bool on);
    void signal_wireframe(bool on);
    void signal_solid(bool on);
    void signal_cuttingplane_changed(QString directionOfView);
    void signal_cuttingplane_values_changed(qreal height, qreal depth);
};

#endif // GEOMETRYDISPLAYTITLE_H
