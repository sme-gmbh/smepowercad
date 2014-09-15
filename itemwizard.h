#ifndef ITEMWIZARD_H
#define ITEMWIZARD_H

#include <QDialog>
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

private:
    Ui::ItemWizard *ui;
};

#endif // ITEMWIZARD_H
