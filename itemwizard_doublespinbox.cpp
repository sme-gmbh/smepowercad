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

#include "itemwizard_doublespinbox.h"

ItemWizard_DoubleSpinBox::ItemWizard_DoubleSpinBox(QWidget *parent) :
    QLineEdit(parent)
{
    QRegExpValidator* validator = new QRegExpValidator(QRegExp("\\-?\\d+\\.?\\d{0,2}([+\\-*/]{1}\\d+\\.?\\d{0,2})?"));
    this->setValidator(validator);
}

void ItemWizard_DoubleSpinBox::setValue(double value)
{
    this->setText(QString().sprintf("%.2f", value));
}

double ItemWizard_DoubleSpinBox::value()
{
    bool val_1_negative = false;
    QString rawText = this->text();

    if (rawText.startsWith('-'))
    {
        val_1_negative = true;
        rawText.remove(0, 1);   // Strip minus at the beginning of the string
    }
    QStringList splitText = rawText.split(QRegExp("[\\+\\-\\*\\/]"), QString::KeepEmptyParts);
    double val_1, val_2;

    if (splitText.size() >= 1)
    {
        val_1 = splitText.at(0).toDouble();
        if (val_1_negative)
            val_1 = -val_1;
    }
    if (splitText.size() >= 2)
        val_2 = splitText.at(1).toDouble();


    if (rawText.contains(QRegExp("\\+")))
    {
//        qDebug() << splitText << "+";
        return (val_1 + val_2);
    }
    else if (rawText.contains(QRegExp("\\-")))
    {
//        qDebug() << splitText << "-";
        return (val_1 - val_2);
    }
    else if (rawText.contains(QRegExp("\\*")))
    {
//        qDebug() << splitText << "*";
        return (val_1 * val_2);
    }
    else if (rawText.contains(QRegExp("\\/")))
    {
//        qDebug() << splitText << "/";
        return (val_1 / val_2);
    }
    else
        return val_1;
}
