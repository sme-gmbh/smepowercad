#ifndef ITEMCATALOG_H
#define ITEMCATALOG_H

#include <QDockWidget>
#include <QString>
#include <QList>
#include <QRegExp>
#include <QMessageBox>
#include <QInputDialog>

#include "itemdb.h"
#include "itemwizard.h"

namespace Ui {
class ItemCatalog;
}

class ItemCatalog : public QDockWidget
{
    Q_OBJECT

public:
    explicit ItemCatalog(ItemDB* itemDB,  ItemWizard* itemWizard, QWidget *parent = 0);
    ~ItemCatalog();

private slots:
    void on_comboBox_domain_activated(const QString &arg1);

    void on_comboBox_vendor_activated(const QString &arg1);

    void on_toolButton_addVendor_clicked();

    void on_toolButton_removeVendor_clicked();

    void on_comboBox_model_activated(const QString &arg1);

    void on_toolButton_addModel_clicked();

    void on_toolButton_removeModel_clicked();

private:
    Ui::ItemCatalog *ui;
    ItemDB* itemDB;
    ItemWizard* itemWizard;

    void initialize();
};

#endif // ITEMCATALOG_H
