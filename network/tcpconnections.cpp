#include "tcpconnections.h"

Q_LOGGING_CATEGORY(tcpconnections, "powercad.network.server.tcpconnections")

TcpConnections::TcpConnections(QObject *parent) : QObject(parent)
{
    qCDebug(tcpconnection) << this << "created";
}

TcpConnections::~TcpConnections()
{
    qCDebug(tcpconnection) << this << "destroyed";
}

int TcpConnections::count()
{
    QReadWriteLock lock;
    lock.lockForRead();

    int value = m_connections.count();

    lock.unlock();

    return value;
}

void TcpConnections::removeSocket(QTcpSocket *socket)
{
    if (!socket) return;
    if (!m_connections.contains(socket)) return;

    qCDebug(tcpconnection) << this << "removing socket =" << socket;

    if (socket->isOpen()) {
        qCDebug(tcpconnection) << this << "socket is open. attempting to close it" << socket;
        socket->disconnect();
        socket->close();
    }

    qCDebug(tcpconnection) << this << "deleting socket" << socket;
    m_connections.remove(socket);
    socket->deleteLater();

    qCDebug(tcpconnection) << this << "client count =" << m_connections.count();
}

void TcpConnections::disconnected()
{
    if (!sender()) return;

    qCDebug(tcpconnection) << this << "disconnecting socket" << sender();

    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    if (!socket) return;

    removeSocket(socket);
}

void TcpConnections::error(QAbstractSocket::SocketError socketError)
{
    if (!sender()) return;

    qCDebug(tcpconnection) << this << "error in socket" << sender() << "error =" << socketError;

    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    if (!socket) return;

    removeSocket(socket);
}

void TcpConnections::start()
{
    qCDebug(tcpconnection) << this << "connections started on" << QThread::currentThread();
}

void TcpConnections::quit()
{
    if (!sender()) return;

    qCDebug(tcpconnection) << this << "connections quitting";

    foreach (QTcpSocket *socket, m_connections.keys()) {
        qCDebug(tcpconnection) << this << "closing socket" << socket;
        removeSocket(socket);
    }

    qCDebug(tcpconnection) << this << "finishing";
    emit finished();
}

void TcpConnections::accept(qintptr handle, TcpConnection *connection)
{
    QTcpSocket *socket = new QTcpSocket(this);

    if (!socket->setSocketDescriptor(handle)) {
        qCWarning(tcpconnection) << this << "could not accept connection" << handle;
        connection->deleteLater();
        return;
    }

    connect(socket, &QTcpSocket::disconnected, this, &TcpConnections::disconnected);
    connect(socket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &TcpConnections::error);

    connection->moveToThread(QThread::currentThread());
    connection->setSocket(socket);
    m_connections.insert(socket, connection);

    qCDebug(tcpconnection) << this << "clients =" << m_connections.count();
    emit socket->connected();
}

