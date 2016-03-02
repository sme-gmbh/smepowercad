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
    this->setStyleSheet(StylesheetProvider::getStylesheet("ItemWizard,Button,QLineEdit+QTextEdit"));
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
    this->m_itemDB = itemDB;

    // Do not show an empty wizard
    if(item->wizardParams.isEmpty()) {
        item->calculate();
        emit signal_sceneRepaintNeeded();
        return;
    }

    clear();
    m_currentItem = item;
    this->oldParams = m_currentItem->wizardParams;

    m_itemParametersWidget = new ItemParametersWidget(item, itemDB, true, true, this);
    connect(m_itemParametersWidget, &ItemParametersWidget::sceneRepaintNeeded, this, &ItemWizard::signal_sceneRepaintNeeded);
    connect(m_itemParametersWidget, &ItemParametersWidget::parameterChanged, this, &ItemWizard::slot_parameterChanged);
    connect(m_itemParametersWidget, &ItemParametersWidget::parameterChangedComboBox, this, &ItemWizard::slot_parameterChanged);
    ui->verticalLayout->insertWidget(2, m_itemParametersWidget);

    this->widgetLastFocus = qApp->focusWidget();
    this->setWindowTitle(tr("Item Wizard: %1").arg(item->description()));

    // Show item graphic description
    ui->label_itemGraphic->setPixmap(QPixmap::fromImage(this->wizardImage(item)));
    ui->lineEdit_layer->setText(item->layer->path());

    this->show();
    this->setFocus();
}


void ItemWizard::slot_parameterChanged()
{
    m_currentItem->wizardParams.insert(m_itemParametersWidget->getParameters());
    m_currentItem->processWizardInput();
    m_currentItem->calculate();
    emit m_itemDB->signal_itemModified(m_currentItem);
    emit m_itemDB->signal_repaintNeeded();
}

void ItemWizard::save()
{
    WizardParams params = m_itemParametersWidget->getParameters();

    if(!(params == oldParams))
    {
        m_currentItem->wizardParams.insert(oldParams);
        m_currentItem->processWizardInput();
        m_currentItem->calculate();
        this->m_itemDB->setRestorePoint();
        this->m_itemDB->modifyItem_withRestorePoint(m_currentItem, params);
        emit signal_sceneRepaintNeeded();
    }
}

void ItemWizard::clear()
{
    if (m_itemParametersWidget != NULL) {
        delete m_itemParametersWidget;
        m_itemParametersWidget = NULL;
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


QImage ItemWizard::wizardImage(CADitem *item)
{
    return item->wizardImage().scaledToWidth(400, Qt::SmoothTransformation);
}

void ItemWizard::slot_itemDeleted(CADitem *item)
{
    if (m_currentItem == item)
    {
        this->clear();
        this->hide();
        this->giveFocusBack();
    }
}

void ItemWizard::on_pushButton_setToCurrentLayer_clicked()
{
    m_itemDB->setRestorePoint();
    m_itemDB->changeLayerOfItem_withRestorePoint(m_currentItem, m_itemDB->getCurrentLayer());
    ui->lineEdit_layer->setText(m_currentItem->layer->path());
}

void ItemWizard::on_pushButton_cancel_clicked()
{
    if(m_currentItem)
    {
        m_currentItem->wizardParams.insert(this->oldParams);
        m_currentItem->processWizardInput();
        m_currentItem->calculate();
    }
    this->clear();
    this->hide();
    this->giveFocusBack();
}

void ItemWizard::on_pushButton_ok_clicked()
{
    this->save();
    this->clear();
    this->hide();
    this->giveFocusBack();
}
