#include "mainwindow.h"
#include <QApplication>
//#include <QGuiApplication>
#include <QTranslator>
#include <QDebug>
#include <QSettings>
//#include <QtOpenGL/QGL>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("SME GmbH");
    QCoreApplication::setOrganizationDomain("sme-gmbh.net");
    QCoreApplication::setApplicationName("SME PowerCAD");

//    QGL::setPreferredPaintEngine(QPaintEngine::OpenGL);



    QApplication a(argc, argv);

    // Qt 5 specific opengl settings
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 3);
    format.setSamples(4);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
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


    return a.exec();
}
