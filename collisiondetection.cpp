/**********************************************************************
** smepowercad
** Copyright (C) 2015 Smart Micro Engineering GmbH
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/

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
}
