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
