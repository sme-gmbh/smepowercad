#include "caditem.h"

CADitem::CADitem(ItemType type)
{
    this->type = type;
}

CADitem::ItemType CADitem::getType()
{
    return type;
}
