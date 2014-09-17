#ifndef ITEMWIZARD_H
#define ITEMWIZARD_H

#include <QDialog>
#include <QDebug>
#include <QMap>
#include "caditem.h"

#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>

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

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::ItemWizard *ui;

    CADitem *currentItem;
    void save();
    void deleteWdgs(QLayout *layout);

signals:
    void signal_sceneRepaintNeeded();
};

#endif // ITEMWIZARD_H
