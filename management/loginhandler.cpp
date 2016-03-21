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
#include "loginhandler.h"
#include "logging.h"

LoginHandler::LoginHandler(QObject *parent)
    : QObject(parent),
      QQuickImageProvider(QQuickImageProvider::Image),
      m_qmlEngine(new QQmlEngine(this))
{
    // Create dummy data
    m_recentUsers = QList<User>();
    m_recentUsers.append(User("Roland Dierks", "rdierks", QImage(":/management/images/face1.jpg")));
    m_recentUsers.append(User("Scott Summers", "ssummers", QImage(":/management/images/face2.jpg")));
    m_recentUsers.append(User("Ryan Hause", "rhause", QImage(":/management/images/face3.jpg")));

    m_qmlEngine->addImageProvider(QStringLiteral("login"), this);
    m_qmlEngine->rootContext()->setContextProperty("LoginHandler", this);
    QQmlComponent component(m_qmlEngine, QUrl(QStringLiteral("qrc:/management/LoginView.qml")));
    QObject *object = component.create();
    connect(object, SIGNAL(loginTriggered(QString,QString)), this, SLOT(on_loginTriggered(QString,QString)));
}

LoginHandler::~LoginHandler()
{
    delete m_qmlEngine;
}

QImage LoginHandler::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    int num = id.toInt();
    if (num >= m_recentUsers.length()) return QImage();

    QImage img = m_recentUsers.at(num).image;
    if (size)
        *size = img.size();

    if (requestedSize.width() > 0 && requestedSize.height() > 0)
        return img.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    return img;
}

QString LoginHandler::getName(int index)
{
    if (index >= m_recentUsers.length()) return QString();

    return m_recentUsers.at(index).name;
}

QString LoginHandler::getUsername(int index)
{
    if (index >= m_recentUsers.length()) return QString();

    return m_recentUsers.at(index).username;
}

void LoginHandler::on_loginTriggered(QString username, QString password)
{
    qCDebug(powercad) << "Login triggered (" << username << ", " << password << ")";
}
