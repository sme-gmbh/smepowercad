#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <iostream>
#include "clienthandler.h"
#include "../itemdb.h"


class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(ItemDB* itemDB, QObject *parent = 0);

private:
    QTcpServer tcpServer;
    QList<QTcpSocket*> socket_list;
    ItemDB* itemDB;

signals:

public slots:

private slots:
    void slot_new_connection();
    void slot_broadcast(QTcpSocket* fromClient, QByteArray data);
    void slot_connectionClosed(QTcpSocket* socket, ClientHandler* clientHandler);
};

#endif // SERVER_H
