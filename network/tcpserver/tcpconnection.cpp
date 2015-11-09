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

#include "tcpconnection.h"

Q_LOGGING_CATEGORY(tcpconnection, "powercad.network.server.tcpconnection")

TcpConnection::TcpConnection(QObject *parent) : QObject(parent)
{
    qCDebug(tcpconnection) << this << "Created";
}

TcpConnection::~TcpConnection()
{
    qCDebug(tcpconnection) << this << "Destroyed";
}

void TcpConnection::setSocket(QTcpSocket *socket)
{
    m_socket = socket;
    connect(m_socket, &QTcpSocket::connected, this, &TcpConnection::connected);
    connect(m_socket, &QTcpSocket::disconnected, this, &TcpConnection::disconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &TcpConnection::readyRead);
    connect(m_socket, &QTcpSocket::bytesWritten, this, &TcpConnection::bytesWritten);
    connect(m_socket, &QTcpSocket::stateChanged, this, &TcpConnection::stateChanged);
    connect(m_socket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &TcpConnection::error);
}

QTcpSocket *TcpConnection::getSocket()
{
    if (!sender()) return 0;

    return static_cast<QTcpSocket*>(sender());
}

void TcpConnection::connected()
{
    if (!sender()) return;

    qCDebug(tcpconnection) << this << "connected" << sender();
}

void TcpConnection::disconnected()
{
    if (!sender()) return;

    qCDebug(tcpconnection) << this << "disconnected" << getSocket();
}

void TcpConnection::readyRead()
{
    if (!sender()) return;

    qCDebug(tcpconnection) << this << "readyRead" << getSocket();
}

void TcpConnection::bytesWritten(qint64 bytes)
{
    if (!sender()) return;

    qCDebug(tcpconnection) << this << "bytesWritten" << getSocket() << "number of bytes =" << bytes;
}

void TcpConnection::stateChanged(QAbstractSocket::SocketState socketState)
{
    if (!sender()) return;

    qCDebug(tcpconnection) << this << "stateChanged" << getSocket() << "state =" << socketState;
}

void TcpConnection::error(QAbstractSocket::SocketError socketError)
{
    if (!sender()) return;

    qCDebug(tcpconnection) << this << "error" << getSocket() << "error =" << socketError;
}

