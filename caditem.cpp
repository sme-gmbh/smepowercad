#include "caditem.h"

CADitem::CADitem(ItemType type)
{
    this->type = type;
    this->highlight = false;
    this->selected = false;
    this->index = 0;
    this->color_brush = Qt::transparent;
    this->color_pen = Qt::transparent;
    angle_x = 0.0;
    angle_y = 0.0;
    angle_z = 0.0;
}

void CADitem::serialOut(QByteArray* out)
{
    QList<QString> keys = wizardParams.keys();

    *out += "Item" + QString().setNum(this->type).toUtf8() + ";" + layerName.toUtf8() + ";" + QString().setNum(id) + ";";

    foreach (QString key, keys)
    {
        *out += key + ":";
        QVariant value = wizardParams.value(key);
        if (value.type() == QVariant::Double)
            *out += value.toString();
        *out += ";";
    }

    *out += "\n";
}

bool CADitem::serialIn(QByteArray* in)
{

}

CADitem::ItemType CADitem::getType()
{
    return type;
}
