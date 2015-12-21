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

ItemParametersWidget::ItemParametersWidget(CADitem *item, ItemDB *itemdb, QWidget *parent)
    : QWidget(parent),
      m_itemdb(itemdb),
      m_item(item)
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

    m_layout = new QFormLayout(this);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

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
            if (key.contains("angle", Qt::CaseInsensitive)) {
                ((CalculatingLineEdit*)wdg)->setEinheit("°");
            } else {
                ((CalculatingLineEdit*)wdg)->setEinheit("mm");
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

        m_layout->addRow(key, wdg);
        i++;
    }

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
    QWidget *wdg;
    for (int r = 0; r < m_layout->rowCount(); r++) {
        wdg = m_layout->itemAt(r, QFormLayout::FieldRole)->widget();

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
            stringList.append(m_item->wizardParams.value(r).toStringList().at(0));   // Insert available texts of ComboBox
            stringList.append(box->currentText());                                              // Insert current text of ComboBox
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

