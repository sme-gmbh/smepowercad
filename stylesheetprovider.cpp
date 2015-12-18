#include "stylesheetprovider.h"
#include "logging.h"

StylesheetProvider* StylesheetProvider::m_instance = 0;

StylesheetProvider::StylesheetProvider() :
    m_stylesheets()
{
}

StylesheetProvider *StylesheetProvider::getInstance()
{
    if (!m_instance) {
        m_instance = new StylesheetProvider();
    }

    return m_instance;
}

QString StylesheetProvider::getStylesheet(const QString &name)
{
    if (m_stylesheets.contains(name)) {
        return m_stylesheets.value(name);
    }

    QFile file(QString(":/styles/%1.css").arg(name));
    if (!file.open(QFile::ReadOnly)) {
        return QString();
    }

    QString stylesheet = QString::fromUtf8(file.readAll()).remove("\n").simplified();
    m_stylesheets.insert(name, stylesheet);

    return stylesheet;
}

