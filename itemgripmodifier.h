#ifndef ITEMGRIPMODIFIER_H
#define ITEMGRIPMODIFIER_H

#include <QWidget>

#include "caditem.h"
#include "itemdb.h"

namespace Ui {
class ItemGripModifier;
}

class ItemGripModifier : public QWidget
{
    Q_OBJECT

public:
    explicit ItemGripModifier(QWidget *parent = 0);
    ~ItemGripModifier();

    enum ItemGripType
    {
        Grip_Append,
        Grip_Move,
        Grip_Length,
        Grip_Rotate_X,
        Grip_Rotate_Y,
        Grip_Rotate_Z,
    };

    void setItem(CADitem* item);
    void activateGrip(ItemGripModifier::ItemGripType gripType, QPoint mousePos, QVector3D scenePos);
    void finishGrip();

private slots:
    void slot_rejected();

private:
    Ui::ItemGripModifier *ui;
    CADitem* item;

    void showAppendBox();

signals:
    void signal_sceneRepaintNeeded();
};

#endif // ITEMGRIPMODIFIER_H
