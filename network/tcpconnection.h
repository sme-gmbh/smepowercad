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

#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <QLoggingCategory>
#include <QObject>
#include <QTcpSocket>

Q_DECLARE_LOGGING_CATEGORY(tcpconnection)

class TcpConnection : public QObject
{
    Q_OBJECT
public:
    explicit TcpConnection(QObject *parent = 0);
    ~TcpConnection();

    virtual void setSocket(QTcpSocket *socket);

protected:
    QTcpSocket *m_socket;
    QTcpSocket *getSocket();

signals:

public slots:
    virtual void connected();
    virtual void disconnected();
    virtual void readyRead();
    virtual void bytesWritten(qint64 bytes);
    virtual void stateChanged(QAbstractSocket::SocketState socketState);
    virtual void error(QAbstractSocket::SocketError socketError);
};

#endif // TCPCONNECTION_H
