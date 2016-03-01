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

#ifndef EXPANDABLETOOLBUTTON_H
#define EXPANDABLETOOLBUTTON_H

#include <QToolButton>
#include <QGridLayout>
#include <QStyle>
#include <QThread>

#include "stylesheetprovider.h"
#include "buttongridwidget.h"

#define BUTTONS_PER_ROW 5

class ExpandableToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ExpandableToolButton(QWidget *parent = 0);
    ~ExpandableToolButton();

    void addSubbuttons(QList<QToolButton*> buttons);

    void showSubbuttons();
    void hideSubbuttons();
    void button_pressed();

protected:
    ButtonGridWidget *m_subwidget;
    QGridLayout *m_layout;
    bool isShowingSubbuttons;

    void setActive(bool active);

protected slots:
    virtual void subbuttonLeftClicked();
    virtual void subbuttonRightClicked();

private:
    int currentRow;
    int currentColumn;

signals:
    void showingSubbuttons();
};

#endif // EXPANDABLETOOLBUTTON_H
