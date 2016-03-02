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

#include "logging.h"



WizardParams::WizardParams()
{
}

WizardParams::WizardParams(const WizardParams &params)
{
    this->keys_double = params.keys_double;
    this->keys_int = params.keys_int;
    this->keys_string = params.keys_string;
    this->keys_stringlist = params.keys_stringlist;

    this->values_double = params.values_double;
    this->values_int = params.values_int;
    this->values_string = params.values_string;
    this->values_stringlist = params.values_stringlist;
}

WizardParams::~WizardParams()
{

}

void WizardParams::insert(QString key, QVariant value)
{
//    qCDebug(powercad) << "WizardParams::insert()";
    if (!this->keys().contains(key))
    {
        QMetaType::Type type = (QMetaType::Type)value.type();
        if (type == QMetaType::Float)
            type = QMetaType::Double;
        switch (type)
        {
        case QVariant::String:
            keys_string.append(key);
            values_string.append(value.toString());
            break;
        case QVariant::Int:
            keys_int.append(key);
            values_int.append(value.toInt());
            break;
        case QVariant::Double:
            keys_double.append(key);
            values_double.append(value.toDouble());
            break;
        case QVariant::StringList:
            keys_stringlist.append(key);
            values_stringlist.append(value.toStringList());
            break;
        default:
            qCDebug(powercad) << "WizardParams::insert() Unhandled value type:" << value.type();
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
            if (index < 0)
                qCDebug(powercad) << "WizardParams::insert() wrong replacement data type";
            values_string.replace(index, value.toString());
            break;
        case QVariant::Int:
            index = keys_int.indexOf(key);
            if (index < 0)
                qCDebug(powercad) << "WizardParams::insert() wrong replacement data type";
            values_int.replace(index, value.toInt());
            break;
        case QVariant::Double:
            index = keys_double.indexOf(key);
            if (index < 0)
                qCDebug(powercad) << "WizardParams::insert() wrong replacement data type";
            values_double.replace(index, value.toDouble());
            break;
        case QVariant::StringList:
            index = keys_stringlist.indexOf(key);
            if (index < 0)
                qCDebug(powercad) << "WizardParams::insert() wrong replacement data type";
            values_stringlist.replace(index, value.toStringList());
            break;
        default:
            qCDebug(powercad) << "WizardParams::insert() Unhandled value type:" << value.type();
            break;
        }
    }
}

void WizardParams::insert(QString key, double value)
{
    int index = keys_double.indexOf(key);
    if (index < 0)
    {
        keys_double.append(key);
        values_double.append(value);
    }
    else
        values_double.replace(index, value);
}

void WizardParams::insert(QString key, int value)
{
    int index = keys_int.indexOf(key);
    if (index < 0)
    {
        keys_int.append(key);
        values_int.append(value);
    }
    else
        values_int.replace(index, value);
}

void WizardParams::insert(QString key, QString value)
{
    int index = keys_string.indexOf(key);
    if (index < 0)
    {
        keys_string.append(key);
        values_string.append(value);
    }
    else
        values_string.replace(index, value);
}

void WizardParams::insert(QString key, QStringList value)
{
    int index = keys_stringlist.indexOf(key);
    if (index < 0)
    {
        keys_stringlist.append(key);
        values_stringlist.append(value);
    }
    else
        values_stringlist.replace(index, value);
}

void WizardParams::insertComboBox(QString key, QStringList texts_available, QString currentText)
{
    QStringList stringList;

    stringList << texts_available.join('*');
    stringList << currentText;

    insert(key, stringList);
}

void WizardParams::insert(WizardParams newParams)
{
    foreach(QString newKey, newParams.keys())
    {
        QVariant newValue = newParams.value(newKey);
        this->insert(newKey, newValue);
    }
}

QVariant WizardParams::value(QString key) const
{
//    qCDebug(powercad) << "WizardParams::value() key =" << key;
    int index_double     = keys_double.indexOf(key);
    int index_int        = keys_int.indexOf(key);
    int index_string     = keys_string.indexOf(key);
    int index_stringlist = keys_stringlist.indexOf(key);

//    qCDebug(powercad) << "index_double" << index_double;
//    qCDebug(powercad) << "index_int" << index_int;
//    qCDebug(powercad) << "index_string" << index_string;
//    qCDebug(powercad) << "index_stringlist" << index_stringlist;

    if (index_double >= 0)
        return QVariant(values_double.at(index_double));
    if (index_int >= 0)
        return QVariant(values_int.at(index_int));
    if (index_string >= 0)
        return QVariant(values_string.at(index_string));
    if (index_stringlist >= 0)
        return QVariant(values_stringlist.at(index_stringlist));
    return QVariant();
}

QVariant WizardParams::value(int index) const
{
//    qCDebug(powercad) << "WizardParams::value(int)";
    if (index < values_double.count())
        return QVariant(values_double.at(index));
    if (index < (values_double.count() + values_int.count()))
        return QVariant(values_int.at(index - values_double.count()));
    if (index < (values_double.count() + values_int.count() + values_string.count()))
        return QVariant(values_string.at(index - values_double.count() - values_int.count()));
    if (index < (values_double.count() + values_int.count() + values_string.count() + values_stringlist.count()))
        return QVariant(values_stringlist.at(index - values_double.count() - values_int.count() - values_string.count()));

    return QVariant();
}

QList<QString> WizardParams::keys() const
{
//    qCDebug(powercad) << "WizardParams::keys()";
    QList<QString> keys;

    keys.append(keys_double);
    keys.append(keys_int);
    keys.append(keys_string);
    keys.append(keys_stringlist);

    return keys;
}

QList<QVariant> WizardParams::values() const
{
//    qCDebug(powercad) << "WizardParams::values()";
    QList<QVariant> values;

    foreach(double val, values_double)
        values.append(QVariant(val));
    foreach(int val, values_int)
        values.append(QVariant(val));
    foreach(QString val, values_string)
        values.append(QVariant(val));
    foreach(QStringList val, values_stringlist)
        values.append(QVariant(val));

    return values;
}

bool WizardParams::isEmpty() const
{
//    qCDebug(powercad) << "WizardParams::isEmpty()";

    if (!values_double.isEmpty())
        return false;
    if (!values_int.isEmpty())
        return false;
    if (!values_string.isEmpty())
        return false;
    if (!values_stringlist.isEmpty())
        return false;

    return true;
}

QJsonArray WizardParams::serialize() const
{
    QJsonArray ret = QJsonArray();

    int i = 0;
    foreach (QString key, this->keys()) {
        QVariant value = this->value(i);
        QJsonObject o = QJsonObject();
        o.insert("key", key);
        o.insert("value", QJsonValue::fromVariant(value));
        ret.append(o);
        i++;
    }

    return ret;
}

void WizardParams::deserialize(QJsonArray data)
{
    foreach (QJsonValue v, data) {
        QJsonObject o = v.toObject();

        this->insert(o.value("key").toString(), o.value("value").toVariant());
    }
}

bool WizardParams::operator==(const WizardParams &toBeCompared)
{
    bool ret = true;

    foreach(QString key, this->keys())
    {
        QVariant value_this = this->value(key);
        QVariant value_tbc = toBeCompared.value(key);
        ret &= (value_this == value_tbc);
    }

    foreach(QString key, toBeCompared.keys())
    {
        QVariant value_this = this->value(key);
        QVariant value_tbc = toBeCompared.value(key);
        ret &= (value_this == value_tbc);
    }

    return ret;
}
