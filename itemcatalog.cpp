#include "itemcatalog.h"
#include "ui_itemcatalog.h"

ItemCatalog::ItemCatalog(ItemDB* itemDB,  ItemWizard* itemWizard, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ItemCatalog)
{
    ui->setupUi(this);
    this->itemDB = itemDB;
    this->itemWizard = itemWizard;
    if (!setupLocalDirectory())
        return;
    setupGitProcess();
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

bool ItemCatalog::setupLocalDirectory()
{
    catalogDir = QDir::home();
    if (!catalogDir.cd(".smepowercad/catalog"))     // local catalog directory does not exist
    {
        if (catalogDir.mkpath(".smepowercad/catalog"))
            ui->textEdit_terminalOutput->append(tr("The local directory .smepowercad/catalog did not exist and has been created."));
        else
        {
            ui->textEdit_terminalOutput->append(tr("The local directory .smepowercad/catalog could not be created!"));
            return false;
        }
    }
    else
    {
        ui->textEdit_terminalOutput->append(tr("The local directory .smepowercad/catalog does exist."));
    }

    ui->lineEdit_db_localDirectory->setText(catalogDir.canonicalPath());
    return true;
}

bool ItemCatalog::setupGitProcess()
{
    process_git.setProgram("git");
    process_git.setProcessChannelMode(QProcess::MergedChannels);
    process_git.setTextModeEnabled(true);
    process_git.setWorkingDirectory(catalogDir.canonicalPath());

    QStringList args;
    args << "--version";
    process_git.setArguments(args);

    connect(&process_git, SIGNAL(started()), this, SLOT(slot_processGit_started()));
    connect(&process_git, SIGNAL(readyRead()), this, SLOT(slot_processGit_readyRead()));
    connect(&process_git, SIGNAL(error(QProcess::ProcessError)), this, SLOT(slot_processGit_error(QProcess::ProcessError)));
    connect(&process_git, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(slot_processGit_finished(int,QProcess::ExitStatus)));
    process_git.start();
    process_git.waitForStarted(1000);
}


void ItemCatalog::slot_processGit_started()
{
    ui->pushButton_db_gitClone->setEnabled(false);
    ui->pushButton_db_gitCommit->setEnabled(false);
    ui->pushButton_db_gitLog->setEnabled(false);
    ui->pushButton_db_gitPull->setEnabled(false);
    ui->pushButton_db_gitPush->setEnabled(false);
    ui->pushButton_db_gitStatus->setEnabled(false);
//    ui->textEdit_terminalOutput->append(tr("Git started."));
}

void ItemCatalog::slot_processGit_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    switch (exitStatus)
    {
    case QProcess::NormalExit:
        if (exitCode == 0)
            ui->textEdit_terminalOutput->append(tr("Git finished successfully.\n\n"));
        else
            ui->textEdit_terminalOutput->append(tr("Git finished unsuccessfully with exit code %1.\n\n").arg(QString().setNum(exitCode)));
        break;
    case QProcess::CrashExit:
        ui->textEdit_terminalOutput->append(tr("Git crashed, exit code %1.\n\n").arg(QString().setNum(exitCode)));
        break;
    }

    ui->pushButton_db_gitClone->setEnabled(true);
    ui->pushButton_db_gitCommit->setEnabled(true);
    ui->pushButton_db_gitLog->setEnabled(true);
    ui->pushButton_db_gitPull->setEnabled(true);
    ui->pushButton_db_gitPush->setEnabled(true);
    ui->pushButton_db_gitStatus->setEnabled(true);
}

void ItemCatalog::slot_processGit_readyRead()
{
    QByteArray output = process_git.readAll();
    ui->textEdit_terminalOutput->append(QString::fromUtf8(output));
}

void ItemCatalog::slot_processGit_error(QProcess::ProcessError error)
{
    switch (error)
    {
    case QProcess::FailedToStart:
        ui->textEdit_terminalOutput->append(tr("Git failed to start!"));
        break;
    case QProcess::Crashed:
        ui->textEdit_terminalOutput->append(tr("Git crashed!"));
        break;
    case QProcess::Timedout:
        ui->textEdit_terminalOutput->append(tr("Git timed out!"));
        break;
    case QProcess::WriteError:
        ui->textEdit_terminalOutput->append(tr("Git process write error!"));
        break;
    case QProcess::ReadError:
        ui->textEdit_terminalOutput->append(tr("Git process read error!"));
        break;
    case QProcess::UnknownError:
        ui->textEdit_terminalOutput->append(tr("Git unknown error!"));
        break;
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

void ItemCatalog::on_pushButton_db_gitClone_clicked()
{
    QStringList args;
    args << "clone";
    process_git.setArguments(args);
    process_git.start();
    process_git.waitForStarted(1000);
}

void ItemCatalog::on_pushButton_db_gitStatus_clicked()
{
    QStringList args;
    args << "push";
    process_git.setArguments(args);
    process_git.start();
    process_git.waitForStarted(1000);
}

void ItemCatalog::on_pushButton_db_gitLog_clicked()
{
    QStringList args;
    args << "log";
    process_git.setArguments(args);
    process_git.start();
    process_git.waitForStarted(1000);
}

void ItemCatalog::on_pushButton_db_gitPull_clicked()
{
    QStringList args;
    args << "pull";
    process_git.setArguments(args);
    process_git.start();
    process_git.waitForStarted(1000);
}

void ItemCatalog::on_pushButton_db_gitCommit_clicked()
{
    bool ok;
    QString commitMessage = QInputDialog::getText(this, tr("Git Commit"), tr("Enter commit message"), QLineEdit::Normal, QString(), &ok);
    if (!ok)
        return;
    if (commitMessage.isEmpty())
    {
        QMessageBox::critical(this, tr("Git Commit"), tr("Committing without message is not allowed!\n Commit aborted."));
        return;
    }

    QStringList args;
    args << "add";
    args << ".";
    process_git.setArguments(args);
    process_git.start();
    process_git.waitForStarted(1000);
    process_git.waitForFinished(30000);

    args.clear();
    args << "commit";
    args << "-m";
    args << commitMessage;
    process_git.setArguments(args);
    process_git.start();
    process_git.waitForStarted(1000);
}

void ItemCatalog::on_pushButton_db_gitPush_clicked()
{
    QString remote = ui->lineEdit_db_gitRemote->text();
    if (remote.isEmpty())
    {
        QMessageBox::critical(this, tr("Git Clone"), tr("You cannot clone without remote origin!"));
        return;
    }

    QStringList args;
    args << "clone";
    args << remote;
    process_git.setArguments(args);
    process_git.start();
    process_git.waitForStarted(1000);
}
