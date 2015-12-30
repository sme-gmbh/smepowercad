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

#include "itemtypetoolbutton.h"

#include "logging.h"

ItemTypeToolButton::ItemTypeToolButton(ItemDB *itemDB, QWidget *parent) :
    ExpandableToolButton(parent),
    m_itemDB(itemDB)
{

}

ItemTypeToolButton::~ItemTypeToolButton()
{

}

void ItemTypeToolButton::setDomain(const QString &domain)
{
    m_domain = domain;
    this->setToolTip(domain);
    this->setText(domain);
    // TODO: Set Icon

    // Set sub buttons

    QList<int> items = m_itemDB->getItemTypesByDomain(domain);
    qCDebug(powercad) << items;

    QList<QToolButton*> buttons = QList<QToolButton*>();
    foreach (int type, items) {
        QIcon icon = m_itemDB->getIconByItemType((CADitemTypes::ItemType)type, QSize(32, 32));
        QString description = m_itemDB->getItemDescriptionByItemType((CADitemTypes::ItemType)type);
        QToolButton *button = new QToolButton(m_subwidget);
        button->setStyleSheet("border = 0;");
        button->setContextMenuPolicy(Qt::CustomContextMenu);
        button->setMinimumSize(42, 42);
        button->setMaximumSize(42, 42);
        button->setFocusPolicy(Qt::NoFocus);
        button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        button->setIconSize(QSize(32, 32));
        button->setIcon(icon);
        button->setToolTip(description);
        button->setProperty("ItemType", QVariant(type));

        qCDebug(powercad) << "inserting button, desc:" << description;
        buttons.append(button);
    }

    this->addSubbuttons(buttons);
}

void ItemTypeToolButton::subbuttonLeftClicked()
{
    ExpandableToolButton::subbuttonLeftClicked();
    QToolButton *btn = (QToolButton*)this->sender();
    CADitemTypes::ItemType type = (CADitemTypes::ItemType)btn->property("ItemType").toInt();
    emit newItemRequested(type);
}

void ItemTypeToolButton::subbuttonRightClicked()
{
    ExpandableToolButton::subbuttonRightClicked();
}
