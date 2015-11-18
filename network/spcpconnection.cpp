/**********************************************************************
** smepowercad
** Copyright (C) 2015 Smart Micro Engineering GmbH
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/

#include "spcpconnection.h"

Q_LOGGING_CATEGORY(spcpconnection, "powercad.network.server.spcpconnection")

SPCPConnection::SPCPConnection(ItemDB *itemdb, QObject *parent) :
    TcpConnection(parent),
    m_itemdb(itemdb)
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

    while (m_socket->canReadLine()) {
        // Data format:
        // COMMAND id [$parentId] [key]>[value];[key]>[value];...
        QString line = QString::fromUtf8(m_socket->readLine());
        line.remove(QRegExp("[\\r\\n]"));
        int commandLength = line.indexOf(' ');
        if (commandLength == -1) commandLength = line.length();
        QString command = line.left(commandLength);
        line.remove(0, commandLength +1);
        int idLength = line.indexOf(' ');
        if (idLength == -1) idLength = line.length();

        qCDebug(spcpconnection) << "received data";

        quint64 id = 0;
        bool idValid = true;

        id = line.left(idLength).toULongLong(&idValid);

        line.remove(0, idLength +1); // remove id and space

        // Parse key/value pairs
        QStringList commandChunks = line.split(';', QString::SkipEmptyParts);
        QMap<QString, QString> data;

        foreach (QString commandChunk, commandChunks) {
            qCDebug(spcpconnection) << "decoding chunk" << commandChunk;

            QStringList pair = commandChunk.split('>');
            if (pair.length() != 2) {
                // TODO: better error message format
                m_socket->write("ERROR: key_value_pair length invalid\r\n");
                continue;
            }
            // key & value are base64 encoded
//            // DEBUG: disable base64 decoding to use in a terminal without client application
//            QString key = QString::fromUtf8(QByteArray::fromBase64(pair.at(0).toUtf8()));
//            QString value = QString::fromUtf8(QByteArray::fromBase64(pair.at(1).toUtf8()));
            QString key = pair.at(0);
            QString value = pair.at(1);
            data.insert(key, value);
        }

        if (command == "A")     // Get All
        {
            m_socket->write(m_itemdb->network_getAll());
            continue;
        }

        if (command == "M")     // Modify item
        {
            m_socket->write(m_itemdb->network_modifyItem(id, data));
            continue;
        }

        if (command == "D")     // Delete item
        {
            m_socket->write(m_itemdb->network_deleteItem(id));
            continue;
        }

        if (command == "G")     // Get item
        {
            m_socket->write(m_itemdb->network_getItem(id));
            continue;
        }

        if (command == "N")     // New item; id field is used as requested item type!
        {
            QByteArray response = m_itemdb->network_newItem(id, data);
            m_socket->write(response);
            if (!response.startsWith("Error"))
                emit broadcast(this->m_socket, response);
            continue;
        }

        if (command == "C")     // Change Layer of item
        {
            QByteArray response = m_itemdb->network_changeLayerOfItem(id, data);
            m_socket->write(response);
            if (!response.startsWith("Error"))
                emit broadcast(this->m_socket, response);
            continue;
        }

        if (command == "n")     // New layer
        {
            QByteArray response = m_itemdb->network_newLayer(data);
            m_socket->write(response);
            if (!response.startsWith("Error"))
                emit broadcast(this->m_socket, response);
            continue;
        }

        if (command == "m")     // Modify layer
        {
            QByteArray response = m_itemdb->network_modifyLayer(data);
            m_socket->write(response);
            if (!response.startsWith("Error"))
                emit broadcast(this->m_socket, response);
            continue;
        }

        if (command == "d")     // Delete layer
        {
            QByteArray response = m_itemdb->network_deleteLayer(data);
            m_socket->write(response);
            if (!response.startsWith("Error"))
                emit broadcast(this->m_socket, response);
            continue;
        }

        if (command == "c")     // Change Position of Layer (move layer)
        {
            QByteArray response = m_itemdb->network_moveLayer(data);
            m_socket->write(response);
            if (!response.startsWith("Error"))
                emit broadcast(this->m_socket, response);
            continue;
        }

//        if (command == "N") // Creates a new set of data in the parent identified by id
//        {
//            QByteArray response = itemDB->create(data).toUtf8();
//            socket->write(response);
//            if (!response.startsWith("ERROR"))
//                emit signal_broadcast(this->m_socket, response);
//            continue;
//        }

//        if (command == "S") // Set a set of data, overwrites all keys
//        {
//            if (!idValid)
//            {
//                socket->write("ERROR: unable to decode id\r\n");
//                continue;
//            }
//            socket->write(itemDB->set(id, data).toUtf8());
//            continue;
//        }

//        if (command == "C") // Change a set of data, overwrites the given keys
//        {
//            if (!idValid)
//            {
//                socket->write("ERROR: unable to decode id\r\n");
//                continue;
//            }
//            QByteArray response = itemDB->change(id, data).toUtf8();
//            socket->write(response);
//            if (!response.startsWith("ERROR"))
//                emit signal_broadcast(this->m_socket, response);
//            continue;
//        }

//        // Move id to the parent "toId"
//        if (command == "M") // Move a set of data from a node point to another
//        {
//            if (!idValid)
//            {
//                socket->write("ERROR: unable to decode id\r\n");
//                continue;
//            }

//            quint64 parentIdSource = data.value("parentIdSource").toULongLong();
//            quint64 parentIdDest = data.value("parentIdDest").toULongLong();
//            quint64 index_dest = data.value("index_dest").toULongLong();
//            QByteArray response = itemDB->move(id, parentIdSource, parentIdDest, index_dest).toUtf8();
//            socket->write(response);
//            if (!response.startsWith("ERROR"))
//                emit signal_broadcast(this->m_socket, response);
//            continue;
//        }

//        // Copy id to the parent "toId"
//        if (command == "C") // Move a set of data from a node point to another
//        {
//            if (!idValid)
//            {
//                socket->write("ERROR: unable to decode id\r\n");
//                continue;
//            }

//            quint64 parentIdSource = data.value("parentIdSource").toULongLong();
//            quint64 parentIdDest = data.value("parentIdDest").toULongLong();
//            quint64 index_dest = data.value("index_dest").toULongLong();
//            QByteArray response = itemDB->copy(id, parentIdSource, parentIdDest, index_dest).toUtf8();
//            socket->write(response);
//            if (!response.startsWith("ERROR"))
//                emit signal_broadcast(this->m_socket, response);
//            continue;
//        }

//        if (command == "R") // Remove a set of data
//        {
//            socket->write(itemDB->remove(id).toUtf8());
//            continue;
//        }

//        if (command == "L") // Lock: Disable changing, moving, deletion
//        {
//            if (!idValid)
//            {
//                socket->write("ERROR: unable to decode id\r\n");
//                continue;
//            }
//            socket->write(itemDB->lock(id).toUtf8());
//            continue;
//        }

//        if (command == "U") // Unlock: Enable changing, moving, deletion
//        {
//            if (!idValid)
//            {
//                socket->write("ERROR: unable to decode id\r\n");
//                continue;
//            }
//            socket->write(itemDB->unlock(id).toUtf8());
//            continue;
//        }

        // If control reaches this point, we have an unsupported command
        m_socket->write("ERROR: Command not supported: " + command.toUtf8() + "\r\n");
    }
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
