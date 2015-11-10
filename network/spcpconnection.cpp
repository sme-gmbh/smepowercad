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

#include "spcpconnection.h"

Q_LOGGING_CATEGORY(spcpconnection, "powercad.network.server.spcpconnection")

SPCPConnection::SPCPConnection(QObject *parent) : TcpConnection(parent)
{
    qCDebug(spcpconnection) << this << "created";
}

SPCPConnection::~SPCPConnection()
{
    qCDebug(spcpconnection) << this << "destroyed";
}

void SPCPConnection::connected()
{
    qCDebug(spcpconnection) << this << "connected" << getSocket();
}

void SPCPConnection::disconnected()
{
    qCDebug(spcpconnection) << this << "disconnected" << getSocket();
}

void SPCPConnection::readyRead()
{
    if (!m_socket) return;
    qCDebug(spcpconnection) << this << "readyRead" << m_socket;

    QByteArray data = m_socket->readAll();
    Q_UNUSED(data);
}

void SPCPConnection::bytesWritten(qint64 bytes)
{
    qCDebug(spcpconnection) << this << "bytesWritten:" << bytes << getSocket();
}

void SPCPConnection::stateChanged(QAbstractSocket::SocketState socketState)
{
    qCDebug(spcpconnection) << this << "stateChanged" << getSocket() << socketState;
}

void SPCPConnection::error(QAbstractSocket::SocketError socketError)
{
    qCDebug(spcpconnection) << this << "error" << getSocket() << socketError;
}
