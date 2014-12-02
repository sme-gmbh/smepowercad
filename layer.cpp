#include "layer.h"

Layer::Layer(QObject *parent) :
    QObject(parent)
{
    on = true;
    solo = false;
    writable = true;
    width = 0;
    lineType.clear();
    parentLayer = NULL;
}

bool Layer::isEmpty()
{
    if (items.isEmpty() && subLayers.isEmpty())
        return true;
    else
        return false;
}

void Layer::serialOut(QByteArray *out)
{
    *out += "Layer;" + name.toUtf8() + ";" + pen.color().name().toUtf8() + ";" + brush.color().name().toUtf8() + ";" + QByteArray().setNum(width) + ";" + lineType.toUtf8();
    if (parentLayer)
        *out += ";" + parentLayer->name.toUtf8();
    *out += "\n";
}

void Layer::serialIn(QByteArray *in)
{

}
