#ifndef ITEMWIZARD_H
#define ITEMWIZARD_H

#include <QDialog>
#include <QDebug>
#include <QMap>
#include "caditem.h"
#include "itemdb.h"

#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QKeyEvent>

namespace Ui {
class ItemWizard;
}

class ItemWizard : public QDialog
{
    Q_OBJECT

public:
    explicit ItemWizard(QWidget *parent = 0);
    ~ItemWizard();

    void showWizard(CADitem *item);
    static QImage wizardImage(CADitem *item);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void slot_rejected();
    void slot_accepted();

private:
    Ui::ItemWizard *ui;

    CADitem *currentItem;
    void save();
    void deleteWdgs(QLayout *layout);

signals:
    void signal_sceneRepaintNeeded();
    void signal_itemModified(CADitem* item);
};

#endif // ITEMWIZARD_H
