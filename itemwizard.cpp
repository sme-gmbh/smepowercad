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

#include "logging.h"
#include "wizardparams.h"

ItemWizard::ItemWizard(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ItemWizard)
{
    ui->setupUi(this);
    this->widgetLastFocus = NULL;
//    connect(this, SIGNAL(rejected()), this, SLOT(slot_rejected()));
//    connect(this, SIGNAL(accepted()), this, SLOT(slot_accepted()));
}

ItemWizard::~ItemWizard()
{
    delete ui;
}

void ItemWizard::showWizard(CADitem *item, ItemDB* itemDB)
{
    if (item == NULL)
    {
        qCDebug(powercad) << "CADitem is NULL";
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

    deleteWdgs();

    currentItem = item;

    int i = 0;
    foreach(QString key, item->wizardParams.keys())
    {
        QWidget *wdg;
        QVariant value = item->wizardParams.value(i);
        switch (value.type())
        {
        case QVariant::String:
            wdg = new QLineEdit(value.toString(), this);
            break;
        case QVariant::Int:
        case QVariant::Double:
            wdg = new CalculatingLineEdit(this);
            if (key.contains("angle", Qt::CaseInsensitive)) {
                ((CalculatingLineEdit*)wdg)->setEinheit("°");
            } else {
                ((CalculatingLineEdit*)wdg)->setEinheit("mm");
            }
            ((CalculatingLineEdit*)wdg)->setValue(value.toFloat());
            break;
        case QVariant::StringList:
            wdg = new QComboBox(this);
            if (value.toStringList().size() == 2)
            {
                ((QComboBox*)wdg)->addItems(value.toStringList().at(0).split('*'));
                ((QComboBox*)wdg)->setCurrentText(value.toStringList().at(1));
            }
            else
                qCDebug(powercad) << "ItemWizard::showWizard() StringList has invalid size:" << value.toStringList().size() << ";Key:" << key;
            break;
        default:
            qCDebug(powercad) << "ItemWizard::showWizard() Unhandled value type:" << value.type();
            break;
        }
        wdg->setObjectName(key);

        ui->formLayout->addRow(key, wdg);
        i++;
    }

    this->widgetLastFocus = qApp->focusWidget();
    this->setWindowTitle(tr("Item Wizard: %1").arg(item->description()));

    // Show item graphic description
    ui->label_itemGraphic->setPixmap(QPixmap::fromImage(this->wizardImage(item)));

    this->show();
    this->setFocus();
//    this->activateWindow();
}

void ItemWizard::slot_rejected()
{
    this->deleteWdgs();
    this->hide();
    this->giveFocusBack();
}

void ItemWizard::slot_accepted()
{
    this->save();
    this->deleteWdgs();
    this->hide();
    this->giveFocusBack();
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
            val = ((CalculatingLineEdit*)wdg)->getValue();
//            val = ((double)((QDoubleSpinBox*)wdg)->value());
            break;
        case QVariant::StringList:
        {
            QComboBox* box = (QComboBox*)wdg;
            QStringList stringList;
            stringList.append(this->currentItem->wizardParams.value(r).toStringList().at(0));   // Insert available texts of ComboBox
            stringList.append(box->currentText());                                              // Insert current text of ComboBox
            val = stringList;
            break;
        }
        default:
            break;
        }

        params.insert(wdg->objectName(), val);
    }

    this->itemDB->modifyItem_withRestorePoint(currentItem, params);
//    currentItem->wizardParams = params;
//    currentItem->processWizardInput();
//    currentItem->calculate();
    emit signal_sceneRepaintNeeded();
}

void ItemWizard::deleteWdgs()
{
    QLayoutItem *item;
    while (ui->formLayout->count() > 0)
    {
        item = ui->formLayout->takeAt(0);
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    ui->gridLayout->removeItem(ui->formLayout);
    delete ui->formLayout;
    ui->formLayout = new QFormLayout();
    ui->gridLayout->addLayout(ui->formLayout, 1, 0, 1, 4);

    // Remove itemGraphic
    ui->label_itemGraphic->clear();
}

void ItemWizard::giveFocusBack()
{
    if (this->widgetLastFocus != NULL)
        this->widgetLastFocus->setFocus();
}

void ItemWizard::enterEvent(QEvent *event)
{
    if (this->isFloating())
        this->activateWindow();
    else
        this->setFocus();
    event->accept();
}

void ItemWizard::leaveEvent(QEvent *event)
{
    event->accept();
}

void ItemWizard::keyPressEvent(QKeyEvent *event)
{
//    switch (event->key())
//    {
//    case Qt::Key_Return:
//    case Qt::Key_Enter:
//        this->slot_accepted();
//        break;
//    case Qt::Key_Escape:
//        this->slot_rejected();
//        break;
//    default:
//        break;
//    }
    event->accept();
}

void ItemWizard::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::RightButton)
        this->slot_accepted();
    event->accept();
}

QImage ItemWizard::wizardImage(CADitem *item)
{
    return item->wizardImage().scaledToWidth(400, Qt::SmoothTransformation);
}

void ItemWizard::on_pushButton_ok_clicked()
{
    this->slot_accepted();
}

void ItemWizard::on_pushButton_apply_clicked()
{
    this->save();
}

void ItemWizard::on_pushButton_cancel_clicked()
{
    this->slot_rejected();
}

void ItemWizard::slot_itemDeleted(CADitem *item)
{
    if (currentItem == item)
        this->slot_rejected();
}
