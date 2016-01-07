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

#include "expandabletoolbutton.h"

#include "logging.h"

ExpandableToolButton::ExpandableToolButton(QWidget *parent) :
    QToolButton(parent),
    m_subwidget(new QWidget(Q_NULLPTR)),
    m_layout(new QGridLayout(m_subwidget)),
    isShowingSubbuttons(false),
    currentRow(0),
    currentColumn(0)
{
//    this->setStyleSheet(StylesheetProvider::getStylesheet("ExpandableToolButton"));
    m_subwidget->setStyleSheet(StylesheetProvider::getStylesheet("ExpandableToolButton"));

    m_layout->setSpacing(0);
    m_layout->setMargin(0);
    m_subwidget->setLayout(m_layout);
    m_subwidget->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    m_subwidget->setContentsMargins(2, 2, 2, 2);
    m_subwidget->hide();

    connect(this, &ExpandableToolButton::clicked, this, &ExpandableToolButton::toggleShowSubbuttons);
}

ExpandableToolButton::~ExpandableToolButton()
{

}

void ExpandableToolButton::addSubbuttons(QList<QToolButton*> buttons)
{
    int lastRowCount = buttons.size() % BUTTONS_PER_ROW;
    int lastRow = buttons.size() - BUTTONS_PER_ROW;
    if (lastRowCount > 0) {
        lastRow = buttons.size() - lastRowCount;
    }

    foreach (QToolButton *btn, buttons) {
        m_layout->addWidget(btn, currentRow, currentColumn);
        connect(btn, &QToolButton::clicked, this, &ExpandableToolButton::subbuttonLeftClicked);
        connect(btn, &QToolButton::customContextMenuRequested, this, &ExpandableToolButton::subbuttonRightClicked);

        int currentCount = (currentRow * BUTTONS_PER_ROW + currentColumn);

        if (currentColumn == 0 && (currentCount >= lastRow)) {
            btn->setObjectName("first-column-last-row");
        } else if (currentColumn == 0) {
            btn->setObjectName("first-column");
        } else if (currentCount >= lastRow) {
            btn->setObjectName("last-row");
        }

        if (currentColumn == BUTTONS_PER_ROW -1) {
            currentColumn = 0;
            currentRow++;
        } else {
            currentColumn++;
        }
    }

    m_subwidget->adjustSize();
}

void ExpandableToolButton::showSubbuttons()
{
    emit showingSubbuttons();
    m_subwidget->show();
    m_subwidget->move(this->mapToGlobal(this->rect().bottomLeft()));
    isShowingSubbuttons = true;
}

void ExpandableToolButton::hideSubbuttons()
{
    m_subwidget->hide();
    isShowingSubbuttons = false;
}

void ExpandableToolButton::toggleShowSubbuttons()
{
    if (isShowingSubbuttons) {
        hideSubbuttons();
    } else {
        showSubbuttons();
    }
}

void ExpandableToolButton::subbuttonLeftClicked()
{
    hideSubbuttons();
}

void ExpandableToolButton::subbuttonRightClicked()
{
    hideSubbuttons();
}
