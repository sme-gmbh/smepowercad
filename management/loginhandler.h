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
#ifndef LOGINHANDLER_H
#define LOGINHANDLER_H

#include <QObject>
#include <QQuickImageProvider>
#include <QtQml>

class LoginHandler : public QObject, public QQuickImageProvider
{
    Q_OBJECT

    struct User {
        User(QString name, QString username, QImage image)
            : name(name), username(username), image(image) {}
        QString name;
        QString username;
        QImage image;
    };

public:
    explicit LoginHandler(QObject *parent = 0);
    ~LoginHandler();

    virtual QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

private:
    QQmlEngine *m_qmlEngine;
    QList<User> m_recentUsers;

signals:

public slots:
    Q_INVOKABLE QString getName(int index);
    Q_INVOKABLE QString getUsername(int index);

    void on_loginTriggered(QString username, QString password);
};

#endif // LOGINHANDLER_H
