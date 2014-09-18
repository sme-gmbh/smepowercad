#include "layer.h"

Layer::Layer(QObject *parent) :
    QObject(parent)
{
    on = true;
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
