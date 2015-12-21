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

    this->setStyleSheet(StylesheetProvider::getInstance()->getStylesheet("CalculatingLineEdit"));

    this->setFixedWidth(150);

    QToolButton *btnUp = new QToolButton(this);
    btnUp->setIcon(QIcon(":/ui/CalculatingLineEdit/icons/arrow-up.png"));
    btnUp->setIconSize(QSize(6, 12));
    btnUp->setCursor(Qt::ArrowCursor);
    btnUp->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btnUp->setStyleSheet("QToolButton { border: 0px; background: 0; padding: 0px; }");
    btnUp->setFixedHeight(12);
    btnUp->setFixedWidth(12);
    btnUp->move(this->rect().width() - 15, 0);
    QToolButton *btnDown = new QToolButton(this);
    btnDown->setIcon(QIcon(":/ui/CalculatingLineEdit/icons/arrow-down.png"));
    btnDown->setIconSize(QSize(6, 12));
    btnDown->setCursor(Qt::ArrowCursor);
    btnDown->setStyleSheet("QToolButton { border: 0px; background: 0; padding: 0px; }");
    btnDown->setFixedHeight(12);
    btnDown->setFixedWidth(12);
    btnDown->move(this->rect().width() - 15, this->rect().height() - 20);

    connect(btnUp, &QToolButton::clicked, this, &CalculatingLineEdit::on_buttonUp_clicked);
    connect(btnDown, &QToolButton::clicked, this, &CalculatingLineEdit::on_buttonDown_clicked);
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
    return this->text().remove(m_einheit).trimmed().toFloat();
}

void CalculatingLineEdit::setValue(const float &value)
{
    this->setText(QString::number(value));
}

void CalculatingLineEdit::setText(const QString &text)
{
    QString val = text;

    if (!m_einheit.isEmpty()) {
        val.append(" " + m_einheit);
    }

    QLineEdit::setText(val);
}

void CalculatingLineEdit::on_editingFinished()
{
    // validate input & calculate
    QString text = this->text().remove(m_einheit).trimmed();

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

    this->setText(text);
    this->clearFocus();
}

void CalculatingLineEdit::on_buttonUp_clicked()
{
    float val = this->text().remove(m_einheit).trimmed().toFloat();
    val = val + 1.0;
    this->setText(QString::number(val));
}

void CalculatingLineEdit::on_buttonDown_clicked()
{
    float val = this->text().remove(m_einheit).trimmed().toFloat();
    val = val - 1.0;
    this->setText(QString::number(val));
}
