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
