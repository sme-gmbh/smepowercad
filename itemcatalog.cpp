#include "itemcatalog.h"
#include "ui_itemcatalog.h"

ItemCatalog::ItemCatalog(ItemDB* itemDB,  ItemWizard* itemWizard, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ItemCatalog)
{
    ui->setupUi(this);
    this->itemDB = itemDB;
    this->itemWizard = itemWizard;
    initialize();
}

ItemCatalog::~ItemCatalog()
{
    delete ui;
}

void ItemCatalog::initialize()
{
    QList<QString> domains = this->itemDB->getDomains();

    foreach(QString domain, domains)
    {
        ui->comboBox_domain->addItem(domain);
    }

    if (domains.size() > 0)
    {
        on_comboBox_domain_activated(domains.first());
    }
}

void ItemCatalog::on_comboBox_domain_activated(const QString &arg1)
{
    ui->comboBox_itemType->clear();
    ui->comboBox_vendor->clear();

    QString domain = arg1;
    QList<int> items = itemDB->getItemTypesByDomain(domain);

    foreach(int type, items)
    {
        QIcon icon = itemDB->getIconByItemType((CADitemTypes::ItemType)type, QSize(64, 64));
        QString description = itemDB->getItemDescriptionByItemType((CADitemTypes::ItemType)type).remove(QRegExp(".+\\|"));

        ui->comboBox_itemType->addItem(icon, description, QVariant(type));
    }
}

void ItemCatalog::on_comboBox_vendor_activated(const QString &arg1)
{

}

void ItemCatalog::on_toolButton_addVendor_clicked()
{
    bool ok;
    QString newVendor = QInputDialog::getText(this, tr("Add Vendor"), tr("Enter Vendor Name"), QLineEdit::Normal, QString(), &ok);
    if (!ok)
        return;
}

void ItemCatalog::on_toolButton_removeVendor_clicked()
{
    QString vendor = ui->comboBox_vendor->currentText();
    if (vendor.isEmpty())
        return;

    if (QMessageBox::question(this, tr("Remove Vendor"), tr("You are going to delete the vendor %1.").arg(vendor),
                              tr("Abort"), tr("Proceed"), "", 1, 0)
            == 0)
    return;


}

void ItemCatalog::on_comboBox_model_activated(const QString &arg1)
{

}

void ItemCatalog::on_toolButton_addModel_clicked()
{
    bool ok;
    QString newModel = QInputDialog::getText(this, tr("Add Model"), tr("Enter Model Description"), QLineEdit::Normal, QString(), &ok);
    if (!ok)
        return;
}

void ItemCatalog::on_toolButton_removeModel_clicked()
{
    QString model = ui->comboBox_model->currentText();
    if (model.isEmpty())
        return;

    if (QMessageBox::question(this, tr("Remove Model"), tr("You are going to delete the model %1.").arg(model),
                              tr("Abort"), tr("Proceed"), "", 1, 0)
            == 0)
    return;


}
