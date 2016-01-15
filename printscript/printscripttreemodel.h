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
#ifndef PRINTSCRIPTTREEMODEL_H
#define PRINTSCRIPTTREEMODEL_H

#include <QAbstractItemModel>
#include "printscripttreeitem.h"
#include "printscript.h"

class PrintscriptTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit PrintscriptTreeModel(QObject *parent = 0);
    ~PrintscriptTreeModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    void clear();
    PrintscriptTreeItem *getRootItem() const;

private:
    PrintscriptTreeItem *m_rootItem;
};

#endif // PRINTSCRIPTTREEMODEL_H
