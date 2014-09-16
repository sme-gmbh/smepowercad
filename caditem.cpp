#include "caditem.h"

CADitem::CADitem(ItemType type)
{
    this->type = type;
    this->highlight = false;
    this->selected = false;
    this->index = 0;
    this->color_brush = Qt::transparent;
    this->color_pen = Qt::transparent;
}

CADitem::ItemType CADitem::getType()
{
    return type;
}
