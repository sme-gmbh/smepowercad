#include "itemwizard.h"
#include "ui_itemwizard.h"

ItemWizard::ItemWizard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemWizard)
{
    ui->setupUi(this);
}

ItemWizard::~ItemWizard()
{
    delete ui;
}

void ItemWizard::showWizard(CADitem *item)
{
    currentItem = item;
    QMap<QString, QVariant>::iterator it;
    for (it = item->wizardParams.begin(); it != item->wizardParams.end(); it++)
    {
        QWidget *wdg;
        wdg->setObjectName(it.key());
        switch (it.value().type())
        {
        case QVariant::String:
            wdg = new QLineEdit(it.value().toString(), this);
            break;
        case QVariant::Int:
            wdg = new QSpinBox(this);
            ((QSpinBox*)wdg)->setValue(it.value().toInt());
            break;
        case QVariant::Double:
            wdg = new QDoubleSpinBox(this);
            ((QDoubleSpinBox*)wdg)->setValue(it.value().toDouble());
            break;
        }


        ui->formLayout->addRow(it.key(), wdg);
    }

    this->show();
}

void ItemWizard::on_buttonBox_accepted()
{
    //this->save();
}

void ItemWizard::on_buttonBox_rejected()
{

}

void ItemWizard::save()
{
    QMap<QString, QVariant> map;
    QVariant val;
    QWidget *wdg;
    for (int r = 0; r < ui->formLayout->rowCount(); r++)
    {
        wdg = ui->formLayout->itemAt(r, QFormLayout::FieldRole)->widget();


        switch (currentItem->wizardParams.value(wdg->objectName()).type())
        {
        case QVariant::String:
            val = QVariant::fromValue(((QLineEdit*)wdg)->text());
            break;
        case QVariant::Int:
            val = QVariant::fromValue(((QSpinBox*)wdg)->value());
            break;
        case QVariant::Double:
            val = QVariant::fromValue(((QDoubleSpinBox*)wdg)->value());
            break;
        }

        map.insert(wdg->objectName(), val);
    }


    //currentItem->wizardParams = ;
    //currentItem->calculate();
}
