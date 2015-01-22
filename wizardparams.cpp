#include "wizardparams.h"

#include <QDebug>

WizardParams::WizardParams()
{
}

WizardParams::WizardParams(const WizardParams &params)
{
    this->keys_double = params.keys_double;
    this->keys_int = params.keys_int;
    this->keys_string = params.keys_string;
    this->values_double = params.values_double;
    this->values_int = params.values_int;
    this->values_string = params.values_string;
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
            if (index < 0)
                qDebug() << "WizardParams::insert() wrong replacement data type";
            values_string.replace(index, value.toString());
            break;
        case QVariant::Int:
            index = keys_int.indexOf(key);
            if (index < 0)
                qDebug() << "WizardParams::insert() wrong replacement data type";
            values_int.replace(index, value.toInt());
            break;
        case QVariant::Double:
            index = keys_double.indexOf(key);
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

QVariant WizardParams::value(QString key)
{
//    qDebug() << "WizardParams::value() key =" << key;
    int index_double = keys_double.indexOf(key);
    int index_int    = keys_int.indexOf(key);
    int index_string = keys_string.indexOf(key);

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
    if (index < keys_double.count())
        return QVariant(values_double.at(index));
    if (index < (keys_double.count() + keys_int.count()))
        return QVariant(values_int.at(index - keys_double.count()));
    if (index < (keys_double.count() + keys_int.count() + keys_string.count()))
        return QVariant(keys_string.at(index - keys_double.count() - keys_int.count()));

    return QVariant();
}

QList<QString> WizardParams::keys()
{
//    qDebug() << "WizardParams::keys()";
    QList<QString> keys;

    keys.append(keys_double);
    keys.append(keys_int);
    keys.append(keys_string);

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

    if (keys_double.isEmpty())
        empty = false;
    if (keys_int.isEmpty())
        empty = false;
    if (keys_string.isEmpty())
        empty = false;

    return empty;
}
