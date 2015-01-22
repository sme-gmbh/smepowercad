#include "caditem.h"
#include "layer.h"

CADitem::CADitem(CADitemTypes::ItemType type)
{
    this->type = type;
    this->layer = NULL;
    this->highlight = false;
    this->selected = false;
    this->index = 0;
    this->color_brush = Qt::transparent;
    this->color_pen = Qt::transparent;
    angle_x = 0.0;
    angle_y = 0.0;
    angle_z = 0.0;
    position = QVector3D(0.0, 0.0, 0.0);
    matrix_rotation.setToIdentity();
}

QColor CADitem::getColorPen()
{
    QColor color_pen_tmp = this->color_pen;

    if (color_pen_tmp == Qt::transparent)   // BYLAYER
    {
        color_pen_tmp = layer->pen.color();
    }
    else if (color_pen_tmp.value() < 50)
        color_pen_tmp = Qt::white;

    if (highlight || selected)
    {
        if (color_pen_tmp.value() > 127)
        {
            color_pen_tmp.setHsv(color_pen_tmp.hsvHue(), color_pen_tmp.hsvSaturation(), color_pen_tmp.value() - 100);
        }
        else
        {
            color_pen_tmp.setHsv(color_pen_tmp.hsvHue(), color_pen_tmp.hsvSaturation(), color_pen_tmp.value() + 100);
        }
    }

    return color_pen_tmp;
}

QColor CADitem::getColorBrush()
{
    QColor color_brush_tmp = this->color_brush;

    if (color_brush_tmp == Qt::transparent)   // BYLAYER
    {
        color_brush_tmp = layer->brush.color();
    }
    else if (color_brush_tmp.value() < 50)
        color_brush_tmp = Qt::white;

    if (highlight || selected)
    {
        if (color_brush_tmp.value() > 127)
        {
            color_brush_tmp.setHsv(color_brush_tmp.hsvHue(), color_brush_tmp.hsvSaturation(), color_brush_tmp.value() - 100);
        }
        else
        {
            color_brush_tmp.setHsv(color_brush_tmp.hsvHue(), color_brush_tmp.hsvSaturation(), color_brush_tmp.value() + 100);
        }
    }

    return color_brush_tmp;
}

void CADitem::setLayer(Layer *layer)
{
    this->layer = layer;
    this->setLayer_processItems(this->subItems);
}

void CADitem::setLayer_processItems(QList<CADitem *> subItems)
{
    foreach (CADitem* item, subItems)
    {
        item->layer = this->layer;
        this->setLayer_processItems(item->subItems);
    }
}

void CADitem::setID(quint64 id)
{
    this->id = id;
    this->setID_processItems(this->subItems);
}

void CADitem::setID_processItems(QList<CADitem*> subItems)
{
    foreach (CADitem* item, subItems)
    {
        item->id = this->id;
        this->setID_processItems(item->subItems);
    }
}

void CADitem::serialOut(QByteArray* out)
{
    QList<QString> keys = wizardParams.keys();

    *out += "Item" + QString().setNum(this->type).toUtf8() + ";" + layer->name.toUtf8() + ";" + QString().setNum(id) + ";";

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

CADitemTypes::ItemType CADitem::getType()
{
    return type;
}
