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

#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include <QSettings>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("SME GmbH");
    QCoreApplication::setOrganizationDomain("sme-gmbh.net");
    QCoreApplication::setApplicationName("SME PowerCAD");

    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QApplication a(argc, argv);

    // Qt 5 specific opengl settings
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 3);
    format.setSamples(4);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setSwapInterval(0);
    QSurfaceFormat::setDefaultFormat(format);
    // End of Qt 5 specific opengl settings

    QString locale = QLocale::system().name();
    qDebug("Locale: " + locale.toUtf8()); // de_DE , en_US, ru_RU

    QSettings settings;
    QString lang = settings.value("environment_general_language", "de_DE").toString();

    QTranslator translator;
    translator.load("powercad-" + lang);
    a.installTranslator(&translator);

    MainWindow w;
    w.showMaximized();
//    w.showFullScreen();


    return a.exec();
}
