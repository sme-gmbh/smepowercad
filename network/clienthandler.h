#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QMap>
#include <QByteArray>
#include <QRegExp>

#include "../itemdb.h"

class ClientHandler : public QObject
{
    Q_OBJECT
public:
    explicit ClientHandler(QObject *parent, QTcpSocket* socket, ItemDB* itemDB);

private:
    QTcpSocket* socket;
    ItemDB* itemDB;

signals:
    void signal_broadcast(QTcpSocket* fromClient, QByteArray data);
    void signal_connectionClosed(QTcpSocket* socket, ClientHandler* ClientHandler);

public slots:

private slots:
    void slot_read_ready();
    void slot_disconnected();
};

#endif // CLIENTHANDLER_H
