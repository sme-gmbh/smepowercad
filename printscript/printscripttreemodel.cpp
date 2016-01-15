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
#include "printscripttreemodel.h"

PrintscriptTreeModel::PrintscriptTreeModel(QObject *parent)
    : QAbstractItemModel(parent),
      m_rootItem(NULL)
{
    this->clear();
}

PrintscriptTreeModel::~PrintscriptTreeModel()
{
    delete m_rootItem;
}

QVariant PrintscriptTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    PrintscriptTreeItem *item = static_cast<PrintscriptTreeItem*>(index.internalPointer());

    if (role == Qt::DisplayRole) {
        if (index.column() == 0)
            return item->name;
    } else if (role == Qt::UserRole +0) {
        Printscript *ps = dynamic_cast<Printscript*>(item);
        if (ps) {
            return ps->script;
        }
    }

    return QVariant();
}

Qt::ItemFlags PrintscriptTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant PrintscriptTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(role)
    return QVariant();
}

QModelIndex PrintscriptTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    PrintscriptTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<PrintscriptTreeItem*>(parent.internalPointer());

    PrintscriptTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex PrintscriptTreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    PrintscriptTreeItem *childItem = static_cast<PrintscriptTreeItem*>(child.internalPointer());
    PrintscriptTreeItem *parentItem = childItem->parentItem();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int PrintscriptTreeModel::rowCount(const QModelIndex &parent) const
{
    PrintscriptTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<PrintscriptTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int PrintscriptTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

void PrintscriptTreeModel::clear()
{
    if (m_rootItem)
        delete m_rootItem;

    m_rootItem = new PrintscriptTreeItem("$$RootItem", NULL, this);
}

PrintscriptTreeItem* PrintscriptTreeModel::getRootItem() const
{
    return m_rootItem;
}
