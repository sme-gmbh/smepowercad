#include "collisiondetection.h"

#include <QDebug>

CollisionDetection::CollisionDetection(ItemDB *itemDB, QObject *parent) :
    QThread(parent)
{
    this->itemDB = itemDB;
    connect(this, SIGNAL(finished()), this, SLOT(slot_checkNextItem()));
}

void CollisionDetection::run()
{
    testLayers(itemDB->layers);
}

void CollisionDetection::testLayers(QList<Layer *> layers)
{
    foreach (Layer* layer, layers)
    {
        testItems(layer->items, layer);
        testLayers(layer->subLayers);
    }
}

void CollisionDetection::testItems(QList<CADitem *> items, Layer *layer, bool isSubItem)
{
    foreach (CADitem* item, items)
    {
        if (item == currentItem)
            continue;

        if (!currentBoundingBox.intersectsWith(item->boundingBox))
            continue;

        if (!isSubItem)
            currentReferenceItem = item;

        // simple test
        emit signal_itemsDoCollide(currentItem, currentReferenceItem);
        qDebug() << "Collision!";

        if (item->subItems.count() > 0)
            testItems(item->subItems, layer, true);
    }
}

void CollisionDetection::slot_checkNextItem()
{
    if (!itemsToCheck.isEmpty())
    {
        currentItem = itemsToCheck.takeFirst();
        currentBoundingBox = currentItem->boundingBox;
        this->start(QThread::IdlePriority);
    }
}

void CollisionDetection::slot_testModifiedItem(CADitem *item)
{
    itemsToCheck.append(item);

    if (!this->isRunning())
        slot_checkNextItem();
}

void CollisionDetection::slot_itemDeleted(CADitem *item)
{
    itemsToCheck.removeOne(item);
//    if (item == currentItem)

}
