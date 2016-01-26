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
#ifndef PRINTSCRIPTTREEITEM_H
#define PRINTSCRIPTTREEITEM_H

#include <QObject>

class PrintscriptTreeItem : public QObject
{
    Q_OBJECT
public:
    PrintscriptTreeItem(const QString &name, PrintscriptTreeItem *parentItem = 0, QObject *parent = 0);
    PrintscriptTreeItem(PrintscriptTreeItem *item);
    ~PrintscriptTreeItem();

    // Model stuff
    void appendChild(PrintscriptTreeItem *item);
    void insertChild(int position, PrintscriptTreeItem *item);
    bool removeChild(PrintscriptTreeItem *item);
    PrintscriptTreeItem* child(int row);
    int childCount() const;
    int row() const;
    PrintscriptTreeItem *parentItem();
    int getChildIndex(PrintscriptTreeItem *childItem) const;
    QList<PrintscriptTreeItem*> getChildItems() const;
    PrintscriptTreeItem* findByName(const QString &name) const;
    bool isEmpty() const;

    QString name;

    void setParentItem(PrintscriptTreeItem *item);
    PrintscriptTreeItem* getParentItem() const;

private:
    QList<PrintscriptTreeItem*> m_childItems;
    PrintscriptTreeItem *m_parentItem;
};

#endif // PRINTSCRIPTTREEITEM_H
