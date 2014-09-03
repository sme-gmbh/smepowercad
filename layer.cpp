#include "layer.h"

Layer::Layer(QObject *parent) :
    QObject(parent)
{
    on = true;
    writable = true;
    width = 0;
    lineType.clear();
}
