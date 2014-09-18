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
    if (item == NULL)
    {
        qDebug("CADitem is NULL");
        return;
    }

    // Do not show an empty wizard
    if(item->wizardParams.isEmpty())
    {
        item->calculate();
        emit signal_sceneRepaintNeeded();
        return;
    }

    currentItem = item;
    QMap<QString, QVariant>::iterator it;
    for (it = item->wizardParams.begin(); it != item->wizardParams.end(); it++)
    {
        QWidget *wdg;
        switch (it.value().type())
        {
        case QVariant::String:
            wdg = new QLineEdit(it.value().toString(), this);
            break;
        case QVariant::Int:
            wdg = new QSpinBox(this);
            ((QSpinBox*)wdg)->setValue(it.value().toInt());
            ((QSpinBox*)wdg)->setMaximum(INT_MAX);
            ((QSpinBox*)wdg)->setMinimum(INT_MIN);
            break;
        case QVariant::Double:
            wdg = new QDoubleSpinBox(this);
            ((QDoubleSpinBox*)wdg)->setValue(it.value().toDouble());
            ((QDoubleSpinBox*)wdg)->setMaximum(10e+20);
            ((QDoubleSpinBox*)wdg)->setMinimum(-10e+20);
            break;
        default:
            break;
        }
        wdg->setObjectName(it.key());


        ui->formLayout->addRow(it.key(), wdg);
    }

    this->setWindowTitle(tr("Item Wizard: %1").arg(item->description));

    this->show();
}

void ItemWizard::on_buttonBox_accepted()
{
    this->save();
    this->accept();
}

void ItemWizard::on_buttonBox_rejected()
{
    currentItem->calculate();
    this->deleteWdgs(ui->formLayout);
    this->reject();
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
        default:
            break;
        }

        map.insert(wdg->objectName(), val);
    }

    this->deleteWdgs(ui->formLayout);


    currentItem->wizardParams = map;
    currentItem->processWizardInput();
    currentItem->calculate();
    emit signal_sceneRepaintNeeded();
}

void ItemWizard::deleteWdgs(QLayout *layout)
{
    QLayoutItem *item;
    while ((item = layout->takeAt(0)))
    {
        if (item->layout()) {
            deleteWdgs(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    this->layout()->removeItem(ui->formLayout);
    ui->formLayout->deleteLater();
    ui->formLayout = new QFormLayout;
    ((QVBoxLayout*)this->layout())->insertLayout(0, ui->formLayout);

    qDebug() << ui->formLayout->rowCount();
}
