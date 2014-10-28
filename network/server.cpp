#include "server.h"

Server::Server(ItemDB *itemDB, QObject *parent) :
    QObject(parent)
{
    this->itemDB = itemDB;

    connect(&tcpServer, SIGNAL(newConnection()),  this, SLOT(slot_new_connection()));
    tcpServer.listen(QHostAddress::Any, 16001);
}

void Server::slot_new_connection()
{
    QTcpSocket* newSocket = tcpServer.nextPendingConnection();
    this->socket_list.append(newSocket);

    ClientHandler* clientHandler = new ClientHandler(this, newSocket, this->itemDB);
    connect(clientHandler, SIGNAL(signal_broadcast(QTcpSocket*,QByteArray)),
            this, SLOT(slot_broadcast(QTcpSocket*,QByteArray)));
    connect(clientHandler, SIGNAL(signal_connectionClosed(QTcpSocket*,ClientHandler*)),
            this, SLOT(slot_connectionClosed(QTcpSocket*,ClientHandler*)));
}

void Server::slot_broadcast(QTcpSocket *fromClient, QByteArray data)
{
    foreach(QTcpSocket* socket, this->socket_list)
    {
        if (fromClient != socket)
            socket->write(data);
    }
}

void Server::slot_connectionClosed(QTcpSocket *socket, ClientHandler *clientHandler)
{
    this->socket_list.removeOne(socket);
    //delete socket;
    delete clientHandler;
}
