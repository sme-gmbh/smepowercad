#ifndef ITEMCATALOG_H
#define ITEMCATALOG_H

#include <QDockWidget>
#include <QString>
#include <QList>
#include <QRegExp>
#include <QMessageBox>
#include <QInputDialog>
#include <QProcess>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

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
    void slot_processGit_started();
    void slot_processGit_finished(int exitCode ,QProcess::ExitStatus exitStatus);
    void slot_processGit_readyRead();
    void slot_processGit_error(QProcess::ProcessError error);
    void on_comboBox_domain_activated(const QString &arg1);

    void on_comboBox_vendor_activated(const QString &arg1);

    void on_toolButton_addVendor_clicked();

    void on_toolButton_removeVendor_clicked();

    void on_comboBox_model_activated(const QString &arg1);

    void on_toolButton_addModel_clicked();

    void on_toolButton_removeModel_clicked();

    void on_pushButton_db_gitClone_clicked();

    void on_pushButton_db_gitStatus_clicked();

    void on_pushButton_db_gitLog_clicked();

    void on_pushButton_db_gitPull_clicked();

    void on_pushButton_db_gitCommit_clicked();

    void on_pushButton_db_gitPush_clicked();

private:
    Ui::ItemCatalog *ui;
    ItemDB* itemDB;
    ItemWizard* itemWizard;
    QDir catalogDir;
    QProcess process_git;

    void initialize();
    bool setupLocalDirectory();
    bool setupGitProcess();
};

#endif // ITEMCATALOG_H
