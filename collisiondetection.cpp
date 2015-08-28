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
    emit signal_itemsDoNotCollide(currentItem);
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

//    qDebug() << "Bounding Boxes intersect, start Triangle vs Triangle test";

    // Triangle vs. triangle intersection test
    QList<MTriangle> item_triangleList = item->getTriangles();
    MIntersection intersectionTester;
    foreach(MTriangle item_triangle, item_triangleList)
    {
        foreach(MTriangle currentItem_triangle, currentItem_triangleList)
        {
            if(intersectionTester.trianglesIntersect(item_triangle, currentItem_triangle))
            {
//                qDebug() << "<I65 "
//                         << "Position_x=" << '"' << currentItem_triangle.getV0().x() << '"'
//                         << "Position_y=" << '"' << currentItem_triangle.getV0().y() << '"'
//                         << "Position_z=" << '"' << currentItem_triangle.getV0().z() << '"'
//                         << "Position_x1=" << '"' << currentItem_triangle.getV1().x() << '"'
//                         << "Position_y1=" << '"' << currentItem_triangle.getV1().y() << '"'
//                         << "Position_z1=" << '"' << currentItem_triangle.getV1().z() << '"'
//                         << "Position_x2=" << '"' << currentItem_triangle.getV2().x() << '"'
//                         << "Position_y2=" << '"' << currentItem_triangle.getV2().y() << '"'
//                         << "Position_z2=" << '"' << currentItem_triangle.getV2().z() << '"'
//                         << "/>" ;
//                qDebug() << "<I65 "
//                         << "Position_x=" << '"' << item_triangle.getV0().x() << '"'
//                         << "Position_y=" << '"' << item_triangle.getV0().y() << '"'
//                         << "Position_z=" << '"' << item_triangle.getV0().z() << '"'
//                         << "Position_x1=" << '"' << item_triangle.getV1().x() << '"'
//                         << "Position_y1=" << '"' << item_triangle.getV1().y() << '"'
//                         << "Position_z1=" << '"' << item_triangle.getV1().z() << '"'
//                         << "Position_x2=" << '"' << item_triangle.getV2().x() << '"'
//                         << "Position_y2=" << '"' << item_triangle.getV2().y() << '"'
//                         << "Position_z2=" << '"' << item_triangle.getV2().z() << '"'
//                         << "/>" ;
                qDebug() << "Collision!";
                emit signal_itemsDoCollide(currentItem, item);
                this->context->doneCurrent();
                return;
            }
        }
    }
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

void CollisionDetection::slot_testModifiedItems(QList<CADitem *> *list)
{
    itemsToCheck.append(*list);

    if (!this->isRunning())
        slot_checkNextItem();
}

void CollisionDetection::slot_itemDeleted(CADitem *item)
{
    itemsToCheck.removeOne(item);
}
