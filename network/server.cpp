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
    TcpServer(parent),
    m_itemDB(itemDB)
{
}

Server::~Server()
{
}

void Server::incomingConnection(qintptr descriptor)
{
    qCDebug(server) << this << "incoming connection:" << descriptor;
    SPCPConnection *connection = new SPCPConnection(this->m_itemDB);
    connect(connection, &SPCPConnection::broadcast, this, &Server::slot_broadcast);

    accept(descriptor, connection);
}

void Server::slot_broadcast(QTcpSocket *fromClient, QByteArray data)
{
    foreach (QTcpSocket *socket, m_connections->getConnections().keys()) {
        if (socket != fromClient)
            socket->write(data);
    }
}
