#ifndef STYLESHEETPROVIDER_H
#define STYLESHEETPROVIDER_H

#include <QMap>
#include <QDir>
#include <QFile>

class StylesheetProvider
{
    typedef QMap<QString, QString> StylesheetMap;

private:
    StylesheetProvider();
    static StylesheetProvider *m_instance;
    StylesheetMap m_stylesheets;

public:
    static StylesheetProvider* getInstance();

    QString getStylesheet(const QString &name);
};

#endif // STYLESHEETPROVIDER_H
