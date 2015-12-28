#ifndef STYLESHEETPROVIDER_H
#define STYLESHEETPROVIDER_H

#include <QMap>
#include <QDir>
#include <QFile>

class StylesheetProvider
{
    typedef QMap<QString, QString> StylesheetMap;

private:
    static StylesheetMap m_stylesheets;

public:
    static QString getStylesheet(const QString &name);
};

#endif // STYLESHEETPROVIDER_H
