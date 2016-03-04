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
#include "loginview.h"

LoginView::LoginView(QWidget *parent)
    : QWebView(parent),
      m_mouseClickX(0),
      m_mouseClickY(0)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(800, 600);

    QUrl url = QUrl("qrc:/management/html/login.html");
    setUrl(url);
}

void LoginView::mousePressEvent(QMouseEvent *event)
{
    QWebView::mousePressEvent(event);

    m_mouseClickX = event->x();
    m_mouseClickY = event->y();
}

void LoginView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton && QRect(0, 0, 800, 30).contains(event->pos())) {
        move(event->globalX() - m_mouseClickX, event->globalY() - m_mouseClickY);
        event->accept();
    } else {
        QWebView::mouseMoveEvent(event);
    }
}
