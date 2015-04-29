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

#include "wizardparams.h"

#include <QDebug>

//QList<QString> WizardParams::globalKeys;

WizardParams::WizardParams()
{
}

WizardParams::WizardParams(const WizardParams &params)
{
    this->keys_double = params.keys_double;
    this->keys_int = params.keys_int;
    this->keys_string = params.keys_string;
//    this->keys_indices_double = params.keys_indices_double;
//    this->keys_indices_int = params.keys_indices_int;
//    this->keys_indices_string = params.keys_indices_string;
    this->values_double = params.values_double;
    this->values_int = params.values_int;
    this->values_string = params.values_string;
}

WizardParams::~WizardParams()
{

}

void WizardParams::insert(QString key, QVariant value)
{
//    qDebug() << "WizardParams::insert()";
    if (!this->keys().contains(key))
    {
        QMetaType::Type type = (QMetaType::Type)value.type();
        if (type == QMetaType::Float)
            type = QMetaType::Double;
        switch (type)
        {
        case QVariant::String:
            keys_string.append(key);
//            keys_indices_string.append(indexOfGlobalKey(key));
            values_string.append(value.toString());
            break;
        case QVariant::Int:
            keys_int.append(key);
//            keys_indices_int.append(indexOfGlobalKey(key));
            values_int.append(value.toInt());
            break;
        case QVariant::Double:
            keys_double.append(key);
//            keys_indices_double.append(indexOfGlobalKey(key));
            values_double.append(value.toDouble());
            break;
        default:
            qDebug() << "WizardParams::insert() Unhandled value type:" << value.type();
            break;
        }
    }
    else
    {
        int index;
        QMetaType::Type type = (QMetaType::Type)value.type();
        if (type == QMetaType::Float)
            type = QMetaType::Double;
        switch (type)
        {
        case QVariant::String:
            index = keys_string.indexOf(key);
//            index = keys_indices_string.indexOf(indexOfGlobalKey(key));
            if (index < 0)
                qDebug() << "WizardParams::insert() wrong replacement data type";
            values_string.replace(index, value.toString());
            break;
        case QVariant::Int:
            index = keys_int.indexOf(key);
//            index = keys_indices_int.indexOf(indexOfGlobalKey(key));
            if (index < 0)
                qDebug() << "WizardParams::insert() wrong replacement data type";
            values_int.replace(index, value.toInt());
            break;
        case QVariant::Double:
            index = keys_double.indexOf(key);
//            index = keys_indices_double.indexOf(indexOfGlobalKey(key));
            if (index < 0)
                qDebug() << "WizardParams::insert() wrong replacement data type";
            values_double.replace(index, value.toDouble());
            break;
        default:
            qDebug() << "WizardParams::insert() Unhandled value type:" << value.type();
            break;
        }
    }
}

void WizardParams::insert(QString key, double value)
{
    int index = keys_double.indexOf(key);
//    int index = keys_indices_double.indexOf(indexOfGlobalKey(key));
    if (index < 0)
    {
        keys_double.append(key);
//        keys_indices_double.append(indexOfGlobalKey(key));
        values_double.append(value);
    }
    else
        values_double.replace(index, value);
}

void WizardParams::insert(QString key, int value)
{
    int index = keys_int.indexOf(key);
//    int index = keys_indices_int.indexOf(indexOfGlobalKey(key));
    if (index < 0)
    {
        keys_int.append(key);
//        keys_indices_int.append(indexOfGlobalKey(key));
        values_int.append(value);
    }
    else
        values_int.replace(index, value);
}

void WizardParams::insert(QString key, QString value)
{
    int index = keys_string.indexOf(key);
//    int index = keys_indices_string.indexOf(indexOfGlobalKey(key));
    if (index < 0)
    {
        keys_string.append(key);
//        keys_indices_string.append(indexOfGlobalKey(key));
        values_string.append(value);
    }
    else
        values_string.replace(index, value);
}

void WizardParams::insert(WizardParams newParams)
{
    foreach(QString newKey, newParams.keys())
    {
        QVariant newValue = newParams.value(newKey);
        this->insert(newKey, newValue);
    }
}

QVariant WizardParams::value(QString key)
{
//    qDebug() << "WizardParams::value() key =" << key;
    int index_double = keys_double.indexOf(key);
    int index_int    = keys_int.indexOf(key);
    int index_string = keys_string.indexOf(key);
//    int index_double = keys_indices_double.indexOf(indexOfGlobalKey(key));
//    int index_int    = keys_indices_int.indexOf(indexOfGlobalKey(key));
//    int index_string = keys_indices_string.indexOf(indexOfGlobalKey(key));

//    qDebug() << "index_double" << index_double;
//    qDebug() << "index_int" << index_int;
//    qDebug() << "index_string" << index_string;

    if (index_double >= 0)
        return QVariant(values_double.at(index_double));
    if (index_int >= 0)
        return QVariant(values_int.at(index_int));
    if (index_string >= 0)
        return QVariant(values_string.at(index_string));
    return QVariant();
}

QVariant WizardParams::value(int index)
{
//    qDebug() << "WizardParams::value(int)";
    if (index < values_double.count())
        return QVariant(values_double.at(index));
    if (index < (values_double.count() + values_int.count()))
        return QVariant(values_int.at(index - values_double.count()));
    if (index < (values_double.count() + values_int.count() + values_string.count()))
        return QVariant(values_string.at(index - values_double.count() - values_int.count()));

    return QVariant();
}

QList<QString> WizardParams::keys()
{
//    qDebug() << "WizardParams::keys()";
    QList<QString> keys;

    keys.append(keys_double);
    keys.append(keys_int);
    keys.append(keys_string);

//    foreach (int index, keys_indices_double)
//    {
//        keys.append(globalKeys.at(index));
//    }

//    foreach (int index, keys_indices_int)
//    {
//        keys.append(globalKeys.at(index));
//    }

//    foreach (int index, keys_indices_string)
//    {
//        keys.append(globalKeys.at(index));
//    }

    return keys;
}

QList<QVariant> WizardParams::values()
{
//    qDebug() << "WizardParams::values()";
    QList<QVariant> values;

    foreach(double val, values_double)
        values.append(QVariant(val));
    foreach(int val, values_int)
        values.append(QVariant(val));
    foreach(QString val, values_string)
        values.append(QVariant(val));

    return values;
}

bool WizardParams::isEmpty()
{
//    qDebug() << "WizardParams::isEmpty()";
    bool empty = true;

    if (values_double.isEmpty())
        empty = false;
    if (values_int.isEmpty())
        empty = false;
    if (values_string.isEmpty())
        empty = false;

    return empty;
}

//quint16 WizardParams::indexOfGlobalKey(QString key)
//{
//    int index = globalKeys.indexOf(key);

//    if (index >= 0)
//        return index;

//    globalKeys.append(key);
//    return (quint16)(globalKeys.size() - 1);
//}
