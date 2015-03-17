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

#include <QDialog>
#include <QLabel>
#include <QAction>
#include <QToolButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLayout>

#include "caditem.h"
#include "itemdb.h"
#include "itemwizard.h"

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
    };

    void setItem(CADitem* item);
    CADitem* getItem();
    void activateGrip(ItemGripModifier::ItemGripType gripType, QPoint mousePos, QVector3D scenePos);
    void moveItemTo(QVector3D new_scenePos);
    void copyItemTo(QVector3D new_scenePos);
    void finishGrip();
    ItemGripType getActiveGrip();

private slots:
    void slot_rejected();
    void slot_button_clicked();
    void slot_button_copyMulty();

private:
    Ui::ItemGripModifier *ui;
    ItemDB* itemDB;
    ItemWizard* itemWizard;
    CADitem* item;
    QVector3D scenePos;
    ItemGripType activeGrip;

    QSpinBox* copyMulti_spinBox_countX;
    QSpinBox* copyMulti_spinBox_countY;
    QSpinBox* copyMulti_spinBox_countZ;
    QDoubleSpinBox* copyMulti_doubleSpinBox_distanceX;
    QDoubleSpinBox* copyMulti_doubleSpinBox_distanceY;
    QDoubleSpinBox* copyMulti_doubleSpinBox_distanceZ;


    void deleteWdgs(QLayout *layout);
    void showAppendBox();
    void showCopyMultiBox();

signals:
    void signal_sceneRepaintNeeded();
};

#endif // ITEMGRIPMODIFIER_H
