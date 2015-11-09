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

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QLoggingCategory>
#include <QObject>
#include <QTcpServer>
#include <QThread>
#include "tcpconnections.h"
#include "tcpconnection.h"

Q_DECLARE_LOGGING_CATEGORY(tcpserver)

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    ~TcpServer();

    virtual bool listen(const QHostAddress &address, quint16 port);
    virtual void close();
    virtual qint64 port();

protected:
    QThread *m_thread;
    TcpConnections *m_connections;
    virtual void incomingConnection(qintptr descriptor);
    virtual void accept(qintptr descriptor, TcpConnection *connection);

signals:
    void accepting(qintptr handle, TcpConnection *connection);
    void finished();

public slots:
    void complete();
};

#endif // TCPSERVER_H
