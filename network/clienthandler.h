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

#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QMap>
#include <QByteArray>
#include <QRegExp>

#include "../itemdb.h"

class ClientHandler : public QObject
{
    Q_OBJECT
public:
    explicit ClientHandler(QObject *parent, QTcpSocket* socket, ItemDB* itemDB);

private:
    QTcpSocket* socket;
    ItemDB* itemDB;

signals:
    void signal_broadcast(QTcpSocket* fromClient, QByteArray data);
    void signal_connectionClosed(QTcpSocket* socket, ClientHandler* ClientHandler);

public slots:

private slots:
    void slot_read_ready();
    void slot_disconnected();
};

#endif // CLIENTHANDLER_H
