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

#include "calculatinglineedit.h"

CalculatingLineEdit::CalculatingLineEdit(QWidget *parent)
    : QLineEdit(parent),
      m_einheit("")
{
    connect(this, &CalculatingLineEdit::editingFinished, this, &CalculatingLineEdit::on_editingFinished);
}

CalculatingLineEdit::~CalculatingLineEdit()
{

}

QString CalculatingLineEdit::einheit() const
{
    return m_einheit;
}

void CalculatingLineEdit::setEinheit(const QString &einheit)
{
    m_einheit = einheit;
}

float CalculatingLineEdit::getValue() const
{
    return this->text().toFloat();
}

void CalculatingLineEdit::setValue(const float &value)
{
    QString text = QString::number(value);

    if (!m_einheit.isEmpty()) {
        text.append(" " + m_einheit);
    }

    this->setText(text);
}

void CalculatingLineEdit::on_editingFinished()
{
    // validate input & calculate
    QString text = this->text();

    // replace all ',' with '.'
    text.replace(',', '.');

    if (true) { // TODO: if equation
        EquationResult r = EquationSolver(text).getResult();

        if (r.isValid) {
            text = QString::number(r.result);
        } else {
            this->selectAll();
            this->setFocus();
            return;
        }
    }

    if (!m_einheit.isEmpty()) {
        text.append(" " + m_einheit);
    }

    this->setText(text);
    this->clearFocus();
}
