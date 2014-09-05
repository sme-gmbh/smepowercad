#include "caditem.h"

CADitem::CADitem(ItemType type)
{
    this->type = type;
    this->highlight = false;
    this->index = 0;
}

CADitem::ItemType CADitem::getType()
{
    return type;
}
