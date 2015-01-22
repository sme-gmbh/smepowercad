#include "itemwizard.h"
#include "ui_itemwizard.h"

ItemWizard::ItemWizard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemWizard)
{
    ui->setupUi(this);
    connect(this, SIGNAL(rejected()), this, SLOT(slot_rejected()));
    connect(this, SIGNAL(accepted()), this, SLOT(slot_accepted()));
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
            ((QSpinBox*)wdg)->setMaximum(INT_MAX);
            ((QSpinBox*)wdg)->setMinimum(INT_MIN);
            ((QSpinBox*)wdg)->setValue(it.value().toInt());
            break;
        case QVariant::Double:
            wdg = new QDoubleSpinBox(this);
            ((QDoubleSpinBox*)wdg)->setMaximum(10e+20);
            ((QDoubleSpinBox*)wdg)->setMinimum(-10e+20);
            ((QDoubleSpinBox*)wdg)->setValue(it.value().toDouble());
            break;
        default:
            qDebug() << "ItemWizard::showWizard() Unhandled value type:" << it.value().type();
            break;
        }
        wdg->setObjectName(it.key());


        ui->formLayout->addRow(it.key(), wdg);
    }

    this->setWindowTitle(tr("Item Wizard: %1").arg(item->description()));

    // Show item graphic description
    ui->label_itemGraphic->setPixmap(QPixmap::fromImage(this->wizardImage(item)));

    this->show();
}

void ItemWizard::on_buttonBox_accepted()
{
//    this->save();
    this->accept();
}

void ItemWizard::on_buttonBox_rejected()
{
    currentItem->calculate();
    this->reject();
}

void ItemWizard::slot_rejected()
{
    this->deleteWdgs(ui->formLayout);
}

void ItemWizard::slot_accepted()
{
    this->save();
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
    ((QVBoxLayout*)this->layout())->insertLayout(1, ui->formLayout);
}

QImage ItemWizard::wizardImage(CADitem *item)
{
    return item->wizardImage().scaledToWidth(400, Qt::SmoothTransformation);
}
