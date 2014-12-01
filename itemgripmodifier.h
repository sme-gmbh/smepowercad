#ifndef ITEMGRIPMODIFIER_H
#define ITEMGRIPMODIFIER_H

#include <QDialog>
#include <QLabel>
#include <QAction>
//#include <QToolButton>
#include <QLayout>

#include "caditem.h"
#include "itemdb.h"
#include "itemwizard.h"

namespace Ui {
class ItemGripModifier;
}

class ItemGripModifier : public QDialog
{
    Q_OBJECT

public:
    explicit ItemGripModifier(ItemDB* itemDB, ItemWizard* itemWizard, QWidget *parent = 0);
    ~ItemGripModifier();

    enum ItemGripType
    {
        Grip_None,
        Grip_Append,
        Grip_Copy,
        Grip_Move,
        Grip_Length,
        Grip_Rotate_X,
        Grip_Rotate_Y,
        Grip_Rotate_Z,
    };

    void setItem(CADitem* item);
    CADitem* getItem();
    void activateGrip(ItemGripModifier::ItemGripType gripType, QPoint mousePos, QVector3D scenePos);
    void finishGrip();
    ItemGripType getActiveGrip();

private slots:
    void slot_rejected();
    void slot_button_clicked();

private:
    Ui::ItemGripModifier *ui;
    ItemDB* itemDB;
    ItemWizard* itemWizard;
    CADitem* item;
    QVector3D scenePos;
    ItemGripType activeGrip;

    void deleteWdgs(QLayout *layout);
    void showAppendBox();

signals:
    void signal_sceneRepaintNeeded();
};

#endif // ITEMGRIPMODIFIER_H
