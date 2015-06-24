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

#ifndef ITEMWIZARD_H
#define ITEMWIZARD_H

#include <QDialog>
#include <QDebug>
#include <QMap>
#include "caditem.h"
#include "itemdb.h"

#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QKeyEvent>

namespace Ui {
class ItemWizard;
}

class ItemWizard : public QDialog
{
    Q_OBJECT

public:
    explicit ItemWizard(QWidget *parent = 0);
    ~ItemWizard();

    void showWizard(CADitem *item, ItemDB *itemDB);
    static QImage wizardImage(CADitem *item);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void slot_rejected();
    void slot_accepted();

private:
    Ui::ItemWizard *ui;

    CADitem *currentItem;
    ItemDB* itemDB;
    void save();
    void deleteWdgs(QLayout *layout);

signals:
    void signal_sceneRepaintNeeded();
};

#endif // ITEMWIZARD_H
