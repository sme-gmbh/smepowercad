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

CADitem::ItemType CADitem::getType()
{
    return type;
}
