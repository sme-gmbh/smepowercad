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

#include "server.h"

Q_LOGGING_CATEGORY(server, "powercad.network.server")

Server::Server(ItemDB *itemDB, QObject *parent) :
    TcpServer(parent)
{
    this->itemDB = itemDB;

    connect(&tcpServer, SIGNAL(newConnection()),  this, SLOT(slot_new_connection()));
    tcpServer.listen(QHostAddress::Any, 16001);
}

Server::~Server()
{

}

void Server::incomingConnection(qintptr descriptor)
{
    qCDebug(server) << this << "incoming connection:" << descriptor;
    TcpConnection *connection = new TcpConnection();

    accept(descriptor, connection);
}

void Server::slot_new_connection()
{
    QTcpSocket* newSocket = tcpServer.nextPendingConnection();
    this->socket_list.append(newSocket);

    ClientHandler* clientHandler = new ClientHandler(this, newSocket, this->itemDB);
    connect(clientHandler, SIGNAL(signal_broadcast(QTcpSocket*,QByteArray)),
            this, SLOT(slot_broadcast(QTcpSocket*,QByteArray)));
    connect(clientHandler, SIGNAL(signal_connectionClosed(QTcpSocket*,ClientHandler*)),
            this, SLOT(slot_connectionClosed(QTcpSocket*,ClientHandler*)));
}

void Server::slot_broadcast(QTcpSocket *fromClient, QByteArray data)
{
    foreach(QTcpSocket* socket, this->socket_list)
    {
        if (fromClient != socket)
            socket->write(data);
    }
}

void Server::slot_connectionClosed(QTcpSocket *socket, ClientHandler *clientHandler)
{
    this->socket_list.removeOne(socket);
    //delete socket;
    delete clientHandler;
}
