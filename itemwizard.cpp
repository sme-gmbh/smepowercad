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
    ui(new Ui::ItemWizard),
    m_itemParametersWidget(NULL)
{
    this->setStyleSheet(StylesheetProvider::getStylesheet("ItemWizard,Button"));
    ui->setupUi(this);
    this->widgetLastFocus = NULL;
}

ItemWizard::~ItemWizard()
{
    delete ui;
}

void ItemWizard::showWizard(CADitem *item, ItemDB* itemDB)
{
    if (item == NULL) {
        qCWarning(powercad) << "CADitem is NULL";
        return;
    }
    this->itemDB = itemDB;

    // Do not show an empty wizard
    if(item->wizardParams.isEmpty()) {
        item->calculate();
        emit signal_sceneRepaintNeeded();
        return;
    }

    clear();
    m_currentItem = item;

    m_itemParametersWidget = new ItemParametersWidget(item, itemDB, true, true, this);
    connect(m_itemParametersWidget, &ItemParametersWidget::sceneRepaintNeeded, this, &ItemWizard::signal_sceneRepaintNeeded);
    ui->verticalLayout->insertWidget(1, m_itemParametersWidget);

    this->widgetLastFocus = qApp->focusWidget();
    this->setWindowTitle(tr("Item Wizard: %1").arg(item->description()));

    // Show item graphic description
    ui->label_itemGraphic->setPixmap(QPixmap::fromImage(this->wizardImage(item)));

    this->show();
    this->setFocus();
}

void ItemWizard::slot_rejected()
{
    this->clear();
    this->hide();
    this->giveFocusBack();
}

void ItemWizard::slot_accepted()
{
    this->save();
    this->clear();
    this->hide();
    this->giveFocusBack();
}

void ItemWizard::save()
{
    WizardParams params = m_itemParametersWidget->getParameters();

    this->itemDB->modifyItem_withRestorePoint(m_currentItem, params);
    emit signal_sceneRepaintNeeded();
}

void ItemWizard::clear()
{
    if (m_itemParametersWidget != NULL) {
        delete m_itemParametersWidget;
        m_itemParametersWidget = NULL;
        QLayoutItem *item = ui->verticalLayout->itemAt(1);
        ui->verticalLayout->removeItem(ui->verticalLayout->itemAt(1));
        delete item;
        ui->label_itemGraphic->clear();
    }
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
    // TODO: different approach for closing dialog, maybe leave it open all the time
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
    if (m_currentItem == item)
        this->slot_rejected();
}
