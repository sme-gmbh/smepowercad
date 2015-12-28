#include "stylesheetprovider.h"
#include "logging.h"

StylesheetProvider::StylesheetMap StylesheetProvider::m_stylesheets = StylesheetProvider::StylesheetMap();


QString StylesheetProvider::getStylesheet(const QString &name)
{
    QStringList names = name.split(',');

    QStringList ret = QStringList();

    foreach (QString name, names) {
        name = name.trimmed();

        if (m_stylesheets.contains(name)) {
            ret.append(m_stylesheets.value(name));
        } else {
            QFile file(QString(":/styles/%1.css").arg(name));
            if (!file.open(QFile::ReadOnly)) {
                qCWarning(powercad) << "Could not open stylesheet:" << file.fileName();
                continue;
            }
            QString stylesheet = QString::fromUtf8(file.readAll()).remove("\n").simplified();
            ret.append(stylesheet);
            m_stylesheets.insert(name, stylesheet);
        }
    }

    return ret.join('\n');
}

