#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString locale = QLocale::system().name();
    qDebug("Locale: " + locale.toUtf8()); // de_DE , en_US, ru_RU

    QTranslator translator;
    translator.load("powercad-de_DE");
    a.installTranslator(&translator);

    MainWindow w;
    w.showMaximized();

    return a.exec();
}
