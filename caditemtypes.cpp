#include "caditemtypes.h"

CADitemTypes::CADitemTypes(QObject *parent) :
    QObject(parent)
{
}

QString CADitemTypes::getEnumNameOfItemType(CADitemTypes::ItemType type)
{
    int enumeratorIndex = metaObject()->indexOfEnumerator("ItemType");
    return QString(metaObject()->enumerator(enumeratorIndex).key((int)type));
}

int CADitemTypes::countOfItemTypes()
{
    return metaObject()->enumeratorCount();
}
