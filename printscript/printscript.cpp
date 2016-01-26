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
#include "printscript.h"

Printscript::Printscript(const QString &name, const QString &script, PrintscriptTreeItem *parentItem, QObject *parent)
    : PrintscriptTreeItem(name, parentItem, parent),
      script(script),
      m_variables(QMap<QString,QString>())
{
}

Printscript::Printscript(Printscript *item)
    : PrintscriptTreeItem(item),
      script(item->script),
      m_variables(item->getVariables())
{
}

void Printscript::insertVariable(const QString &key, const QString &value)
{
    m_variables.insert(key, value);
}

void Printscript::insertVariables(const QMap<QString, QString> variables)
{
    QMapIterator<QString,QString> it(variables);
    while (it.hasNext() && (it.next() != NULL)) {
        m_variables.insert(it.key(), it.value());
    }
}

QMap<QString, QString> Printscript::getVariables() const
{
    return m_variables;
}

void Printscript::removeVariable(const QString &key)
{
    m_variables.remove(key);
}
