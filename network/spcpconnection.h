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

#ifndef SPCPCONNECTION_H
#define SPCPCONNECTION_H

#include <QLoggingCategory>
#include "tcpconnection.h"

Q_DECLARE_LOGGING_CATEGORY(spcpconnection)

class SPCPConnection : public TcpConnection
{
    Q_OBJECT
public:
    explicit SPCPConnection(QObject *parent = 0);
    ~SPCPConnection();

signals:

public slots:
    virtual void connected();
    virtual void disconnected();
    virtual void readyRead();
    virtual void bytesWritten(qint64 bytes);
    virtual void stateChanged(QAbstractSocket::SocketState socketState);
    virtual void error(QAbstractSocket::SocketError socketError);
};

#endif // SPCPCONNECTION_H
