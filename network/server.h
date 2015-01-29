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

#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <iostream>
#include "clienthandler.h"
#include "../itemdb.h"


class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(ItemDB* itemDB, QObject *parent = 0);

private:
    QTcpServer tcpServer;
    QList<QTcpSocket*> socket_list;
    ItemDB* itemDB;

signals:

public slots:

private slots:
    void slot_new_connection();
    void slot_broadcast(QTcpSocket* fromClient, QByteArray data);
    void slot_connectionClosed(QTcpSocket* socket, ClientHandler* clientHandler);
};

#endif // SERVER_H
