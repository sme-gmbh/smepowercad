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

#include "itemparameterswidget.h"

Q_LOGGING_CATEGORY(itemparameterswidget, "itemparameterswidget")

ItemParametersWidget::ItemParametersWidget(CADitem *item, ItemDB *itemdb, bool showPosition, bool showAngle, QWidget *parent)
    : QWidget(parent),
      m_itemdb(itemdb),
      m_item(item),
      m_showPosition(showPosition),
      m_showAngle(showAngle)
{
    if (!item) {
        qWarning(itemparameterswidget) << "CADitem is NULL";
        return;
    }

    if (item->wizardParams.isEmpty()) {
        item->calculate();
        emit sceneRepaintNeeded();
        return;
    }

    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(4);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

    QFormLayout *posLayout = new QFormLayout();
    QFormLayout *angLayout = new QFormLayout();

    int i = 0;
    foreach(QString key, item->wizardParams.keys()) {
        QWidget *wdg;
        QVariant value = item->wizardParams.value(i);
        switch (value.type()) {
        case QVariant::String:
            wdg = new QLineEdit(value.toString(), this);
            break;
        case QVariant::Int:
        case QVariant::Double:
            wdg = new CalculatingLineEdit(this);
            if (key.contains("angle", Qt::CaseInsensitive)
                    || key.contains("alpha", Qt::CaseInsensitive)) {
                ((CalculatingLineEdit*)wdg)->setEinheit("°");
            } else {
                ((CalculatingLineEdit*)wdg)->setEinheit("mm"); // WARNING: Not always right
            }
            ((CalculatingLineEdit*)wdg)->setValue(value.toFloat());
            break;
        case QVariant::StringList:
            wdg = new QComboBox(this);
            if (value.toStringList().size() == 2) {
                ((QComboBox*)wdg)->addItems(value.toStringList().at(0).split('*'));
                ((QComboBox*)wdg)->setCurrentText(value.toStringList().at(1));
            }
            else
                qCDebug(itemparameterswidget) << "StringList has invalid size:" << value.toStringList().size() << ";Key:" << key;
            break;
        default:
            qCDebug(itemparameterswidget) << "Unhandled value type:" << value.type();
            break;
        }
        wdg->setObjectName(key);

        if (key.startsWith("Position")) {
            QString k = key.remove("Position").trimmed().toUpper();
            posLayout->addRow(k, wdg);
            if (!showPosition) wdg->hide();
        } else if (key.startsWith("Angle")) {
            QString k = key.remove("Angle").trimmed().toUpper();
            angLayout->addRow(k, wdg);
            if (!showAngle) wdg->hide();
        } else {
            QHBoxLayout *layout = new QHBoxLayout();
            QLabel *lbl = new QLabel(key);
            lbl->setFixedWidth(50);
            layout->addWidget(lbl);
            layout->addWidget(wdg, 1);
            m_layout->addLayout(layout);
        }

        i++;
    }

    QVBoxLayout *posVLayout = new QVBoxLayout();
    posVLayout->setSpacing(0);
    posLayout->setSpacing(4);
    QLabel *lblPosition = new QLabel(tr("Position"));
    lblPosition->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    posVLayout->addWidget(lblPosition);
    posVLayout->addLayout(posLayout);

    QVBoxLayout *angVLayout = new QVBoxLayout();
    angVLayout->setSpacing(0);
    angLayout->setSpacing(4);
    QLabel *lblAngle = new QLabel(tr("Angle"));
    lblAngle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    angVLayout->addWidget(lblAngle);
    angVLayout->addLayout(angLayout);

    QHBoxLayout *coreHLayout = new QHBoxLayout();
    if (showPosition) coreHLayout->addLayout(posVLayout, 1);
    if (showPosition && showAngle) coreHLayout->addSpacing(10);
    if (showAngle) coreHLayout->addLayout(angVLayout, 1);
    coreHLayout->setMargin(0);
    coreHLayout->setContentsMargins(0, 0, 0, 0);

    if (showPosition || showAngle) m_layout->insertLayout(0, coreHLayout, 0);

    this->setLayout(m_layout);
}

ItemParametersWidget::~ItemParametersWidget()
{
    QLayoutItem *item;
    while (m_layout->count() > 0) {
        item = m_layout->takeAt(0);
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    delete m_layout;
}

WizardParams ItemParametersWidget::getParameters()
{
    WizardParams params;
    QVariant val;
    QList<QWidget*> widgets = getWidgets(m_layout);

    foreach (QWidget *wdg, widgets) {
        if (wdg->objectName().isEmpty()) continue;
        if (wdg->objectName().startsWith("Position") && !m_showPosition) continue;
        if (wdg->objectName().startsWith("Angle") && !m_showAngle) continue;

        switch (m_item->wizardParams.value(wdg->objectName()).type()) {
        case QVariant::String:
            val = ((QLineEdit*)wdg)->text();
            break;
        case QVariant::Int:
        case QVariant::Double:
            val = ((CalculatingLineEdit*)wdg)->getValue();
            break;
        case QVariant::StringList: {
            QComboBox* box = (QComboBox*)wdg;
            QStringList stringList;
            stringList.append(m_item->wizardParams.value(wdg->objectName()).toStringList().at(0));   // Insert available texts of ComboBox
            stringList.append(box->currentText());                                                   // Insert current text of ComboBox
            val = stringList;
            break;
        }
        default:
            break;
        }

        params.insert(wdg->objectName(), val);
    }

    return params;
}

QList<QWidget*> ItemParametersWidget::getWidgets(QLayout *layout)
{
    QList<QWidget*> widgets = QList<QWidget*>();

    QLayoutItem *item;
    for (int i = 0; i < layout->count(); i++) {
        item = layout->itemAt(i);

        if (item->layout() != 0) {
            widgets.append(getWidgets(item->layout()));
        } else if (item->widget()) {
            widgets.append(item->widget());
        }
    }

    return widgets;
}

