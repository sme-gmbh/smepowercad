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

#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include "itemdb.h"

namespace Ui {
class ToolWidget;
}

class ToolWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ToolWidget(ItemDB* itemDB, QWidget *parent = 0);
    ~ToolWidget();

    void setDomain(QString domain);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);

private:
    Ui::ToolWidget *ui;
    ItemDB* itemDB;
    QString domain;
    bool isOpen;

    void displayItemButtons();
    void deleteWdgs(QLayout *layout);
    void setBackgroundColor(QColor color);

private slots:
    void slot_button_clicked();
    void slot_button_rightClicked();

public slots:

signals:
    void signal_newItemRequested(CADitemTypes::ItemType type);
};

#endif // TOOLWIDGET_H
