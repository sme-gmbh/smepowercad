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
#include "caditemheaderincludes.h"

#include <QDebug>

CollisionDetection::CollisionDetection(ItemDB *itemDB, GeometryDisplay* geometrydisplay, QOffscreenSurface *offscreensurface, QObject *parent) :
    QThread(parent)
{
    this->itemDB = itemDB;
    this->geometrydisplay = geometrydisplay;
    this->offscreensurface = offscreensurface;
    this->context = new QOpenGLContext();
    this->context->moveToThread(this);
    connect(this, SIGNAL(finished()), this, SLOT(slot_checkNextItem()));
}

void CollisionDetection::setContext(QOpenGLContext* context)
{
    this->context = context;
}

void CollisionDetection::setGeometryDisplay(GeometryDisplay *display)
{
    this->geometrydisplay = display;
}

void CollisionDetection::run()
{
    testLayers(itemDB->layers);
}

void CollisionDetection::testLayers(QList<Layer *> layers)
{
    foreach (Layer* layer, layers)
    {
        foreach (CADitem* item, layer->items)
        {
            testItems(item);
        }
        testLayers(layer->subLayers);
    }
}

void CollisionDetection::testItems(CADitem* item)
{
    if (!this->context->isValid())
    {
        this->context->setShareContext(this->geometrydisplay->getContext());
        this->context->create();
    }
    this->context->makeCurrent(this->offscreensurface);

    // Don't check with itself
    if (item == currentItem)
        return;

        // Bounding Box intersection test
        if (!currentItem_boundingBox.intersectsWith(item->boundingBox))
            return;

    qDebug() << "Bounding Boxes intersect, start Triangle vs Triangle test";

    // Triangle vs. triangle intersection test
    QList<MTriangle> item_triangleList = item->getTriangles();
    MIntersection intersectionTester;
    foreach(MTriangle item_triangle, item_triangleList)
    {
        foreach(MTriangle currentItem_triangle, currentItem_triangleList)
        {
            if(intersectionTester.trianglesIntersect(item_triangle, currentItem_triangle))
            {
                quint64 msec_stop = datetime->currentMSecsSinceEpoch();
                emit signal_itemsDoCollide(currentItem, item);
                qDebug() << "Collision!";
                qDebug() << "Triangle 1:" << currentItem_triangle.getV0() << currentItem_triangle.getV1() << currentItem_triangle.getV2();
                qDebug() << "Triangle 2:" << item_triangle.getV0() << item_triangle.getV1() << item_triangle.getV2();

                this->context->doneCurrent();
                return;
            }
        }
    }
    quint64 msec_stop = datetime->currentMSecsSinceEpoch();
    this->context->doneCurrent();

}

void CollisionDetection::slot_checkNextItem()
{
    if (!itemsToCheck.isEmpty())
    {
        currentItem = itemsToCheck.takeFirst();
        currentItem_boundingBox = currentItem->boundingBox;
        currentItem_triangleList = currentItem->getTriangles();
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
