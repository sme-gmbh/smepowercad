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

#include "tcpserver.h"

Q_LOGGING_CATEGORY(tcpserver, "powercard.network.server.tcpserver")

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
    qCDebug(tcpconnection) << this << "created" << QThread::currentThread();
}

TcpServer::~TcpServer()
{
    m_thread->quit();
    m_thread->wait();
    delete m_thread;
    qCDebug(tcpconnection) << this << "destroyed";
}

bool TcpServer::listen(const QHostAddress &address, quint16 port)
{
    if (!QTcpServer::listen(address, port)) return false;

    m_thread = new QThread(this);
    m_connections = new TcpConnections();

    connect(m_thread, &QThread::started, m_connections, &TcpConnections::start, Qt::QueuedConnection);
    connect(this, &TcpServer::accepting, m_connections, &TcpConnections::accept, Qt::QueuedConnection);
    connect(this, &TcpServer::finished, m_connections, &TcpConnections::quit, Qt::QueuedConnection);
    connect(m_connections, &TcpConnections::finished, this, &TcpServer::complete, Qt::QueuedConnection);

    m_connections->moveToThread(m_thread);
    m_thread->start();

    return true;
}

void TcpServer::close()
{
    qCDebug(tcpconnection) << this << "closing server";
    emit finished();
    QTcpServer::close();
}

qint64 TcpServer::port()
{
    if (isListening()) {
        return this->serverPort();
    }

    return 1000;
}

void TcpServer::incomingConnection(qintptr descriptor)
{
    qCDebug(tcpconnection) << this << "attempting to accept connection" << descriptor;
    TcpConnection *connection = new TcpConnection();
    accept(descriptor, connection);
}

void TcpServer::accept(qintptr descriptor, TcpConnection *connection)
{
    qCDebug(tcpconnection) << this << "accepting connection" << descriptor;
    connection->moveToThread(m_thread);
    emit accepting(descriptor, connection);
}

void TcpServer::complete()
{
    if (!m_thread) {
        qCWarning(tcpserver) << this << "exiting complete there was no thread!";
        return;
    }

    qCDebug(tcpconnection) << this << "complete called. destroying thread";
    delete m_connections;

    qCDebug(tcpconnection) << this << "quitting thread";
    m_thread->quit();
    m_thread->wait();

    delete m_thread;

    qCDebug(tcpconnection) << this << "complete";
}

