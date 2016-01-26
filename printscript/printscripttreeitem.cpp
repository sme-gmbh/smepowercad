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
#include "printscripttreeitem.h"

PrintscriptTreeItem::PrintscriptTreeItem(const QString &name, PrintscriptTreeItem *parentItem, QObject *parent)
    : QObject(parent),
      name(name),
      m_parentItem(parentItem)
{
    m_childItems = QList<PrintscriptTreeItem*>();
    if (parentItem) parentItem->appendChild(this);
}

PrintscriptTreeItem::PrintscriptTreeItem(PrintscriptTreeItem *item)
    : name(item->name),
      m_childItems(item->getChildItems()),
      m_parentItem(item->getParentItem())
{
}

PrintscriptTreeItem::~PrintscriptTreeItem()
{
    qDeleteAll(m_childItems);
}

void PrintscriptTreeItem::appendChild(PrintscriptTreeItem *item)
{
    m_childItems.append(item);
}

void PrintscriptTreeItem::insertChild(int position, PrintscriptTreeItem *item)
{
    m_childItems.insert(position, item);
}

bool PrintscriptTreeItem::removeChild(PrintscriptTreeItem *item)
{
    return m_childItems.removeOne(item);
}

PrintscriptTreeItem *PrintscriptTreeItem::child(int row)
{
    return m_childItems.value(row);
}

int PrintscriptTreeItem::childCount() const
{
    return m_childItems.count();
}

int PrintscriptTreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->getChildIndex(const_cast<PrintscriptTreeItem*>(this));

    return 0;
}

PrintscriptTreeItem *PrintscriptTreeItem::parentItem()
{
    return m_parentItem;
}

QList<PrintscriptTreeItem *> PrintscriptTreeItem::getChildItems() const
{
    return m_childItems;
}

PrintscriptTreeItem *PrintscriptTreeItem::findByName(const QString &name) const
{
    PrintscriptTreeItem *ret = NULL;

    foreach (PrintscriptTreeItem *item, m_childItems) {
        if (item->name == name) return item;

        ret = item->findByName(name);
    }

    return ret;
}

bool PrintscriptTreeItem::isEmpty() const
{
    return m_childItems.isEmpty();
}

void PrintscriptTreeItem::setParentItem(PrintscriptTreeItem *item)
{
    m_parentItem = item;
}

PrintscriptTreeItem *PrintscriptTreeItem::getParentItem() const
{
    return m_parentItem;
}

int PrintscriptTreeItem::getChildIndex(PrintscriptTreeItem *childItem) const
{
    return m_childItems.indexOf(childItem);
}
