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

#ifndef WIZARDPARAMS_H
#define WIZARDPARAMS_H

#include <QString>
#include <QStringList>
#include <QVariant>

class WizardParams
{
public:
    WizardParams();
    WizardParams(const WizardParams &params);
    ~WizardParams();

    void insert(QString key, QVariant value);
    void insert(QString key, double value);
    void insert(QString key, int value);
    void insert(QString key, QString value);
    void insert(QString key, QStringList value);    // This is used for QComboBox in ItemWizard
                                                    // StringList = {String1*String2*...*Stringn}{activeString}
    void insertComboBox(QString key, QStringList texts_available, QString currentText);
    void insert(WizardParams newParams);
    QVariant value(QString key);
    QVariant value(int index);
    QList<QString> keys();
    QList<QVariant> values();
    bool isEmpty();


private:
    QList<QString> keys_double;
    QList<double> values_double;
    QList<QString> keys_int;
    QList<int> values_int;
    QList<QString> keys_string;
    QList<QString> values_string;
    QList<QString> keys_stringlist;
    QList<QStringList> values_stringlist;
};

#endif // WIZARDPARAMS_H
