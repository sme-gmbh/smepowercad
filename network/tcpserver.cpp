#include "tcpserver.h"

Q_LOGGING_CATEGORY(tcpserver, "powercard.network.server.tcpserver")

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
    qCDebug(tcpconnection) << this << "created" << QThread::currentThread();
}

TcpServer::~TcpServer()
{
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

