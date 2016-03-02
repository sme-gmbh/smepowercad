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

#ifndef ITEMGRIPMODIFIER_H
#define ITEMGRIPMODIFIER_H

#include <QAction>
#include <QDialog>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLayout>
#include <QList>
#include <QSpinBox>
#include <QToolButton>

#include "caditem.h"
#include "itemdb.h"
#include "itemwizard.h"
#include "math/manglecalculations.h"

namespace Ui {
class ItemGripModifier;
}

class ItemGripModifier : public QDialog
{
    Q_OBJECT

public:
    explicit ItemGripModifier(ItemDB* itemDB, ItemWizard* itemWizard, QWidget *parent = 0);
    ~ItemGripModifier();

    enum ItemGripType
    {
        Grip_None,
        Grip_Append,
        Grip_Copy,
        Grip_CopyMulti,
        Grip_Move,
        Grip_Length,
        Grip_Rotate_X,
        Grip_Rotate_Y,
        Grip_Rotate_Z,
        Grip_Rotate_aroundPoint
    };

    void setItem(CADitem* item);
    void setItems(QList< CADitem*> items);
    void setScenePosSource(QVector3D pos);
    CADitem* getItem();
    QList<CADitem *> getItems();
    QString getItemDescription();
    QVector3D getScenePosSource();
    void activateGrip(ItemGripModifier::ItemGripType gripType, QPoint mousePos, QVector3D scenePos);
    void moveItemsTo(QVector3D new_scenePos);
    void copyItemsTo(QVector3D new_scenePos);
    void finishGrip();
    ItemGripType getActiveGrip();

private slots:
    void slot_rejected();
    void slot_button_clicked();
    void slot_button_copyMulty();
    void slot_button_rotateAroundPoint();

private:
    Ui::ItemGripModifier *ui;
    ItemDB* itemDB;
    ItemWizard* itemWizard;
    CADitem* item;
    QList<CADitem*> items;
    QVector3D scenePos;
    ItemGripType activeGrip;

    QSpinBox* copyMulti_spinBox_countX;
    QSpinBox* copyMulti_spinBox_countY;
    QSpinBox* copyMulti_spinBox_countZ;
    QDoubleSpinBox* copyMulti_doubleSpinBox_distanceX;
    QDoubleSpinBox* copyMulti_doubleSpinBox_distanceY;
    QDoubleSpinBox* copyMulti_doubleSpinBox_distanceZ;

    QDoubleSpinBox* rotate_doubleSpinBox_centerX;
    QDoubleSpinBox* rotate_doubleSpinBox_centerY;
    QDoubleSpinBox* rotate_doubleSpinBox_centerZ;
    QDoubleSpinBox* rotate_doubleSpinBox_angleX;
    QDoubleSpinBox* rotate_doubleSpinBox_angleY;
    QDoubleSpinBox* rotate_doubleSpinBox_angleZ;


    void deleteWdgs();
    void showAppendBox();
    void showCopyMultiBox();
    void showRotateAroundPointBox();

signals:
    void signal_sceneRepaintNeeded();
};

#endif // ITEMGRIPMODIFIER_H
