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

