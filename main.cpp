#include "mainwindow.h"
#include <QApplication>
//#include <QGuiApplication>
#include <QTranslator>
#include <QDebug>
#include <QSettings>
//#include <QtOpenGL/QGL>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("SME GmbH");
    QCoreApplication::setOrganizationDomain("sme-gmbh.net");
    QCoreApplication::setApplicationName("SME PowerCAD");

    QGL::setPreferredPaintEngine(QPaintEngine::OpenGL);



    QApplication a(argc, argv);

    QString locale = QLocale::system().name();
    qDebug("Locale: " + locale.toUtf8()); // de_DE , en_US, ru_RU

    QSettings settings;
    QString lang = settings.value("environment_general_language", "de_DE").toString();

    QTranslator translator;
    translator.load("powercad-" + lang);
    a.installTranslator(&translator);

    MainWindow w;
    w.showMaximized();


    return a.exec();
}
