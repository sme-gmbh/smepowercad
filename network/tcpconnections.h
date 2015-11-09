#ifndef TCPCONNECTIONS_H
#define TCPCONNECTIONS_H

#include <QLoggingCategory>
#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QMap>
#include <QReadWriteLock>
#include "tcpconnection.h"

Q_DECLARE_LOGGING_CATEGORY(tcpconnections)

class TcpConnections : public QObject
{
    Q_OBJECT
public:
    explicit TcpConnections(QObject *parent = 0);
    ~TcpConnections();

    virtual int count();

protected:
    QMap<QTcpSocket*, TcpConnection*> m_connections;
    void removeSocket(QTcpSocket *socket);

signals:
    void quitting();
    void finished();

protected slots:
    void disconnected();
    void error(QAbstractSocket::SocketError socketError);

public slots:
    void start();
    void quit();
    void accept(qintptr handle, TcpConnection *connection);
};

#endif // TCPCONNECTIONS_H
