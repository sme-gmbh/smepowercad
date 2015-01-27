#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include <QThread>
#include <QList>

#include "itemdb.h"
#include "math/m3dboundingbox.h"

class CollisionDetection : public QThread
{
    Q_OBJECT
public:
    explicit CollisionDetection(ItemDB* itemDB, QObject *parent = 0);

    void run();
    void finished();

private:
    ItemDB* itemDB;
    QList<CADitem*> itemsToCheck;
    CADitem* currentItem;
    CADitem* currentReferenceItem;
    M3dBoundingBox currentBoundingBox;

    void testLayers(QList<Layer*> layers);
    void testItems(QList<CADitem *> items, Layer *layer, bool isSubItem = false);

private slots:
    void slot_checkNextItem();

signals:
    void signal_itemsDoCollide(CADitem* item_1, CADitem* item_2);

public slots:
    void slot_testModifiedItem(CADitem* item);
    void slot_itemDeleted(CADitem* item);
};

#endif // COLLISIONDETECTION_H
