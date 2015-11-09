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
