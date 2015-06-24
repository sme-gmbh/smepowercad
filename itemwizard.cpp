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

#include "itemwizard.h"
#include "ui_itemwizard.h"
#include "wizardparams.h"

ItemWizard::ItemWizard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemWizard)
{
    ui->setupUi(this);
    connect(this, SIGNAL(rejected()), this, SLOT(slot_rejected()));
    connect(this, SIGNAL(accepted()), this, SLOT(slot_accepted()));
}

ItemWizard::~ItemWizard()
{
    delete ui;
}

void ItemWizard::showWizard(CADitem *item, ItemDB* itemDB)
{
    if (item == NULL)
    {
        qDebug("CADitem is NULL");
        return;
    }
    this->itemDB = itemDB;

    // Do not show an empty wizard
    if(item->wizardParams.isEmpty())
    {
        item->calculate();
        emit signal_sceneRepaintNeeded();
        return;
    }

    currentItem = item;

    int i = 0;
    foreach(QString key, item->wizardParams.keys())
    {
        QWidget *wdg;
        QVariant value = item->wizardParams.value(i);
//        QVariant value = item->wizardParams.value(key);       // DEBUG TEST
        switch (value.type())
        {
        case QVariant::String:
            wdg = new QLineEdit(value.toString(), this);
            break;
        case QVariant::Int:
            wdg = new QSpinBox(this);
            ((QSpinBox*)wdg)->setMaximum(INT_MAX);
            ((QSpinBox*)wdg)->setMinimum(INT_MIN);
            ((QSpinBox*)wdg)->setValue(value.toInt());
            break;
        case QVariant::Double:
            wdg = new QDoubleSpinBox(this);
            ((QDoubleSpinBox*)wdg)->setMaximum(10e+20);
            ((QDoubleSpinBox*)wdg)->setMinimum(-10e+20);
            ((QDoubleSpinBox*)wdg)->setValue(value.toDouble());
            break;
        default:
            qDebug() << "ItemWizard::showWizard() Unhandled value type:" << value.type();
            break;
        }
        wdg->setObjectName(key);


        ui->formLayout->addRow(key, wdg);
        i++;
    }

    this->setWindowTitle(tr("Item Wizard: %1").arg(item->description()));

    // Show item graphic description
    ui->label_itemGraphic->setPixmap(QPixmap::fromImage(this->wizardImage(item)));

    this->show();
}

void ItemWizard::on_buttonBox_accepted()
{
    this->accept();
}

void ItemWizard::on_buttonBox_rejected()
{
    currentItem->calculate();
    this->reject();
}

void ItemWizard::slot_rejected()
{
    this->deleteWdgs(ui->formLayout);
}

void ItemWizard::slot_accepted()
{
    this->save();
}

void ItemWizard::save()
{
    WizardParams params;
    QVariant val;
    QWidget *wdg;
    for (int r = 0; r < ui->formLayout->rowCount(); r++)
    {
        wdg = ui->formLayout->itemAt(r, QFormLayout::FieldRole)->widget();

        switch (currentItem->wizardParams.value(wdg->objectName()).type())
        {
        case QVariant::String:
            val = (((QLineEdit*)wdg)->text());
            break;
        case QVariant::Int:
            val = ((int)((QSpinBox*)wdg)->value());
            break;
        case QVariant::Double:
            val = ((double)((QDoubleSpinBox*)wdg)->value());
            break;
        default:
            break;
        }

        params.insert(wdg->objectName(), val);
    }

    this->deleteWdgs(ui->formLayout);

    this->itemDB->modifyItem_withRestorePoint(currentItem, params);
//    currentItem->wizardParams = params;
//    currentItem->processWizardInput();
//    currentItem->calculate();
    emit signal_sceneRepaintNeeded();
}

void ItemWizard::deleteWdgs(QLayout *layout)
{
    QLayoutItem *item;
    while ((item = layout->takeAt(0)))
    {
        if (item->layout()) {
            deleteWdgs(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    this->layout()->removeItem(ui->formLayout);
    ui->formLayout->deleteLater();
    ui->formLayout = new QFormLayout;
    ((QVBoxLayout*)this->layout())->insertLayout(1, ui->formLayout);
}

QImage ItemWizard::wizardImage(CADitem *item)
{
    return item->wizardImage().scaledToWidth(400, Qt::SmoothTransformation);
}
