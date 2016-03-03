/**********************************************************************
** smepowercad
** Copyright (C) 2015 Smart Micro Engineering GmbH
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/

#include "itemcatalog.h"
#include "ui_itemcatalog.h"

#include "logging.h"

ItemCatalog::ItemCatalog(ItemDB* itemDB,  ItemWizard* itemWizard, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ItemCatalog),
    m_currentItem(Q_NULLPTR)
{
    this->setStyleSheet(StylesheetProvider::getStylesheet("Button,QLineEdit+QTextEdit"));
    ui->setupUi(this);
    m_currentWidget = ui->widgetParametersPlaceholder;

    this->m_itemdb = itemDB;
    this->itemWizard = itemWizard;
    if (!setupLocalDirectory())
        return;
    setupGitProcess();
    getGitConfig();
    initialize();

    ui->comboBox_vendor->setInsertPolicy(QComboBox::InsertAlphabetically);
    ui->comboBox_model->setInsertPolicy(QComboBox::InsertAlphabetically);
}

ItemCatalog::~ItemCatalog()
{
    delete ui;
}

void ItemCatalog::initialize()
{
    QStringList domains = this->m_itemdb->getDomains();
    ui->comboBox_domain->addItems(domains);
}

bool ItemCatalog::setupLocalDirectory()
{
    m_catalogDir = QDir::home();
    if (!m_catalogDir.cd(".smepowercad/catalog")) {    // local catalog directory does not exist
        if (m_catalogDir.mkpath(".smepowercad/catalog")) {
            m_catalogDir.cd(".smepowercad/catalog");
            ui->textEdit_terminalOutput->append(tr("The local directory .smepowercad/catalog did not exist and has been created."));
        } else {
            ui->textEdit_terminalOutput->append(tr("The local directory .smepowercad/catalog could not be created!"));
            return false;
        }
    } else {
        ui->textEdit_terminalOutput->append(tr("The local directory .smepowercad/catalog does exist."));
    }

    ui->lineEdit_db_localDirectory->setText(m_catalogDir.canonicalPath());
    return true;
}

bool ItemCatalog::setupGitProcess()
{
    process_git.setProgram("git");
    process_git.setProcessChannelMode(QProcess::MergedChannels);
    process_git.setTextModeEnabled(true);
    process_git.setWorkingDirectory(m_catalogDir.canonicalPath());

    QStringList args;
    args << "--version";
    process_git.setArguments(args);

    connect(&process_git, SIGNAL(started()), this, SLOT(slot_processGit_started()));
    connect(&process_git, SIGNAL(readyRead()), this, SLOT(slot_processGit_readyRead()));
    connect(&process_git, SIGNAL(error(QProcess::ProcessError)), this, SLOT(slot_processGit_error(QProcess::ProcessError)));
    connect(&process_git, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(slot_processGit_finished(int,QProcess::ExitStatus)));
    process_git.start();
    process_git.waitForStarted(1000);
    process_git.waitForFinished(30000);

    return true;
}

void ItemCatalog::getGitConfig()
{
    this->git_Output.clear();
    QStringList args;
    args << "config";
    args << "--list";
    process_git.setArguments(args);
    process_git.start();
    process_git.waitForStarted(1000);
    process_git.waitForFinished(30000);

    QTextStream stream(&this->git_Output, QIODevice::ReadOnly);
    while(!stream.atEnd())
    {
        QString line = stream.readLine();
        QStringList keyValuePair = line.split('=');
        if (keyValuePair.size() != 2)
            continue;

        QString key = keyValuePair.at(0);
        QString value = keyValuePair.at(1);

        if (key == "user.email")
            ui->lineEdit_db_gitUserEmail->setText(value);

        else if (key == "user.name")
            ui->lineEdit_db_gitUserName->setText(value);

        else if (key == "remote.origin.url")
            ui->lineEdit_db_gitRemote->setText(value);
    }

    this->git_Output.clear();
}

QString ItemCatalog::toPercentEncoding(QString arg)
{
    return QUrl::toPercentEncoding(arg);
}

QStringList ItemCatalog::toPercentEncoding(QStringList list)
{
    for (int i = 0; i < list.length(); i++) {
        list.replace(i, QUrl::toPercentEncoding(list.at(i)));
    }

    return list;
}

QString ItemCatalog::fromPercentEncoding(QString arg)
{
    return QUrl::fromPercentEncoding(arg.toUtf8());
}

QStringList ItemCatalog::fromPercentEncoding(QStringList list)
{
    for (int i = 0; i < list.length(); i++) {
        list.replace(i, QUrl::fromPercentEncoding(list.at(i).toUtf8()));
    }

    return list;
}

QJsonObject ItemCatalog::readDataFromModelFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("Load model"), tr("Could not load model from disk!"));
        return QJsonObject();
    }

    QJsonDocument d = QJsonDocument::fromJson(file.readAll());
    return d.object();
}

bool ItemCatalog::saveModelFile(QString filename, CADitem *item, QString description, QString newFilename)
{
    QJsonDocument d = QJsonDocument();
    QJsonObject o = QJsonObject();
    ItemParametersWidget *wdg = qobject_cast<ItemParametersWidget*>(m_currentWidget);
    item->wizardParams = wdg->getParameters();
    o.insert(JSON_KEY_PARAMETERS, item->wizardParams.serialize());
    o.insert(JSON_KEY_DESCRIPTION, QJsonValue::fromVariant(description));
    d.setObject(o);

    QFile file(filename);
    if (!file.open(QFile::WriteOnly)) {
        return false;
    }
    file.write(d.toJson());
    file.close();
    if (!newFilename.isEmpty() && newFilename != filename) {
        if (!file.rename(newFilename)) return false;

        ui->comboBox_model->removeItem(ui->comboBox_model->currentIndex());
        QString readableName = QUrl::fromPercentEncoding(QFileInfo(newFilename).baseName().toUtf8());
        ui->comboBox_model->addItem(readableName);
        ui->comboBox_model->setCurrentText(readableName);
    }

    return true;
}

void ItemCatalog::showModelData(CADitem *item, QString name, QString description)
{
    QWidget *wdg = new ItemParametersWidget(item, m_itemdb, false, false, this);
    ui->contentLayout->replaceWidget(m_currentWidget, wdg);
    delete m_currentWidget;
    m_currentWidget = wdg;

    ui->lineEdit_modelName->setText(name);
    ui->textEdit_modelDescription->setText(description);
    this->adjustSize();
}

void ItemCatalog::clearModelData()
{
    QWidget *wdg = new QWidget();
    ui->contentLayout->replaceWidget(m_currentWidget, wdg);
    delete m_currentWidget;
    m_currentWidget = wdg;

    ui->lineEdit_modelName->setText("");
    ui->textEdit_modelDescription->setText("");
}

void ItemCatalog::slot_processGit_started()
{
    ui->pushButton_db_gitClone->setEnabled(false);
    ui->pushButton_db_gitCommit->setEnabled(false);
    ui->pushButton_db_gitLog->setEnabled(false);
    ui->pushButton_db_gitPull->setEnabled(false);
    ui->pushButton_db_gitPush->setEnabled(false);
    ui->pushButton_db_gitStatus->setEnabled(false);
    ui->textEdit_terminalOutput->append("$ " + process_git.program() + " " + process_git.arguments().join(" "));
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

    QString finishedProcess = process_git.arguments().first();
    if (finishedProcess == "clone" || finishedProcess == "pull") {
        // reload vendors and models...
        on_comboBox_itemType_currentIndexChanged(ui->comboBox_itemType->currentText());
    }
}

void ItemCatalog::slot_processGit_readyRead()
{
    QByteArray output = process_git.readAll();
    this->git_Output.append(QString::fromUtf8(output));
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

void ItemCatalog::on_comboBox_domain_currentIndexChanged(const QString &arg1)
{
    ui->comboBox_itemType->clear();
    ui->comboBox_vendor->clear();
    ui->comboBox_model->clear();

    QString domain = arg1;
    m_currentDomainItemTypes = m_itemdb->getItemTypesByDomain(domain);

    QString dir = QUrl::toPercentEncoding(domain);
    m_currentDomainDir = m_catalogDir;
    if (!m_currentDomainDir.exists(dir)) {
        m_currentDomainDir.mkpath(dir);
    }
    m_currentDomainDir.cd(dir);

    foreach(int type, m_currentDomainItemTypes) {
        QIcon icon = m_itemdb->getIconByItemType((CADitemTypes::ItemType)type, QSize(64, 64));
        QString description = m_itemdb->getItemDescriptionByItemType((CADitemTypes::ItemType)type).remove(QRegExp(".+\\|"));

        ui->comboBox_itemType->addItem(icon, description, QVariant(type));
    }
}

void ItemCatalog::on_comboBox_itemType_currentIndexChanged(const QString &arg1)
{
    if (arg1.isEmpty()) return;

    ui->comboBox_vendor->clear();
    ui->comboBox_model->clear();

    // user selected an CADitem -> look for vendors
    QString dir = QUrl::toPercentEncoding(arg1);
    m_currentItemDir = m_currentDomainDir;
    if (!m_currentItemDir.exists(dir)) {
        m_currentItemDir.mkpath(dir);
    }
    m_currentItemDir.cd(dir);

    CADitem *item = m_itemdb->createItem((CADitemTypes::ItemType)ui->comboBox_itemType->currentData().toInt());
    ui->label_itemGraphic->setPixmap(QPixmap().fromImage(item->wizardImage().scaledToWidth(400, Qt::SmoothTransformation)));
    delete item;

    QStringList dirs = fromPercentEncoding(m_currentItemDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name));
    ui->comboBox_vendor->addItems(dirs);
}

void ItemCatalog::on_comboBox_vendor_currentIndexChanged(const QString &arg1)
{
    ui->comboBox_model->clear();
    // user selected a vendor -> look for models
    QString dir = QUrl::toPercentEncoding(arg1);
    m_currentVendorDir = m_currentItemDir;
    m_currentVendorDir.cd(dir);

    QStringList files = fromPercentEncoding(m_currentVendorDir.entryList(QDir::Files, QDir::Name));
    files.replaceInStrings(".json", "");
    ui->comboBox_model->addItems(files);
}

void ItemCatalog::on_comboBox_model_currentIndexChanged(const QString &arg1)
{
    if (arg1.isEmpty()) {
        clearModelData();
        return;
    }

    QJsonObject o = readDataFromModelFile(m_currentVendorDir.absoluteFilePath(toPercentEncoding(arg1) + ".json"));
    delete m_currentItem;
    m_currentItem = m_itemdb->createItem((CADitemTypes::ItemType)ui->comboBox_itemType->currentData().toInt());
    m_currentItem->wizardParams.deserialize(o.value(JSON_KEY_PARAMETERS).toArray());

    showModelData(m_currentItem, arg1, o.value(JSON_KEY_DESCRIPTION).toString());
}

void ItemCatalog::on_toolButton_addVendor_clicked()
{
    bool ok;
    QString newVendor = QInputDialog::getText(this, tr("Add Vendor"), tr("Enter Vendor Name"), QLineEdit::Normal, QString(), &ok);
    if (!ok)
        return;

    if (!m_currentItemDir.mkpath(QUrl::toPercentEncoding(newVendor))) {
        QMessageBox::warning(this, tr("Add Vendor"), tr("Could not add new vendor!"));
        return;
    }

    ui->comboBox_vendor->addItem(newVendor);
    ui->comboBox_vendor->setCurrentText(newVendor);
}

void ItemCatalog::on_toolButton_removeVendor_clicked()
{
    QString vendor = ui->comboBox_vendor->currentText();
    if (vendor.isEmpty())
        return;

    if (ui->comboBox_model->count() > 0) {
        QMessageBox::warning(this, tr("Remove Vendor"), tr("Model list for vendor is not empty! Could not delete vendor."));
        return;
    }

    if (QMessageBox::question(this, tr("Remove Vendor"), tr("You are going to delete the vendor %1.").arg(vendor),
                              tr("Abort"), tr("Proceed"), "", 1, 0) == 0) {
        return;
    }

    m_currentItemDir.rmdir(QUrl::toPercentEncoding(vendor));
    ui->comboBox_vendor->removeItem(ui->comboBox_vendor->currentIndex());
}

void ItemCatalog::on_toolButton_addModel_clicked()
{
    bool ok;
    QString newModel = QInputDialog::getText(this, tr("Add Model"), tr("Enter Model name"), QLineEdit::Normal, QString(), &ok);
    if (!ok)
        return;

    // add new model file
    CADitem *item = m_itemdb->createItem((CADitemTypes::ItemType)ui->comboBox_itemType->currentData().toInt());
    showModelData(item, newModel, "");
    if (!saveModelFile(m_currentVendorDir.absoluteFilePath(QUrl::toPercentEncoding(newModel) + ".json"),
                  item,
                  "")) {
        QMessageBox::warning(this, tr("Add Model"), tr("Could not store model!"));
    }
    delete item;

    ui->comboBox_model->addItem(newModel);
    ui->comboBox_model->setCurrentText(newModel);
}

void ItemCatalog::on_toolButton_removeModel_clicked()
{
    QString model = ui->comboBox_model->currentText();
    if (model.isEmpty())
        return;

    if (QMessageBox::question(this, tr("Remove Model"), tr("You are going to delete the model %1.").arg(model),
                              tr("Abort"), tr("Proceed"), "", 1, 0) == 0) {
        return;
    }

    QFile::remove(m_currentVendorDir.absoluteFilePath(QUrl::toPercentEncoding(model) + ".json"));
    ui->comboBox_model->removeItem(ui->comboBox_model->currentIndex());
}

void ItemCatalog::on_pushButton_db_gitClone_clicked()
{
    QString remote = ui->lineEdit_db_gitRemote->text();
    if (remote.isEmpty()) {
        QMessageBox::critical(this, tr("Git Clone"), tr("You cannot clone without remote origin!"));
        return;
    }

    // check if destination folder is empty
    QDirIterator it(m_catalogDir.absolutePath(), QStringList() << "*.json", QDir::Files, QDirIterator::Subdirectories);
    if (it.hasNext()) {
        // destination is not empty
        QMessageBox::critical(this, tr("Git Clone"), tr("Cannot clone into an non-empty folder!"));
        return;
    } else {
        // destination folder may have subfolders but no files
        foreach (QString dir, m_catalogDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            removeDir(m_catalogDir.absoluteFilePath(dir));
        }
    }

    this->git_Output.clear();
    QStringList args;
    args << "clone";
    args << remote;
    args << ".";
    process_git.setArguments(args);
    process_git.start();
    process_git.waitForStarted(1000);
}

void ItemCatalog::on_pushButton_db_gitStatus_clicked()
{
    this->git_Output.clear();
    QStringList args;
    args << "status";
    process_git.setArguments(args);
    process_git.start();
    process_git.waitForStarted(1000);
}

void ItemCatalog::on_pushButton_db_gitLog_clicked()
{
    this->git_Output.clear();
    QStringList args;
    args << "log";
    process_git.setArguments(args);
    process_git.start();
    process_git.waitForStarted(1000);
}

void ItemCatalog::on_pushButton_db_gitPull_clicked()
{
    this->git_Output.clear();
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

    this->git_Output.clear();
    QStringList args;
    args << "add";
    args << ".";
    process_git.setArguments(args);
    process_git.start();
    process_git.waitForStarted(1000);
    process_git.waitForFinished(30000);

    this->git_Output.clear();
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
    this->git_Output.clear();
    QStringList args;
    args << "push";
    process_git.setArguments(args);
    process_git.start();
    process_git.waitForStarted(1000);
}

void ItemCatalog::on_lineEdit_db_gitUserName_editingFinished()
{
    this->git_Output.clear();
    QStringList args;
    args << "config";
    args << "user.name";
    args << ui->lineEdit_db_gitUserName->text();
    process_git.setArguments(args);
    process_git.start();
    process_git.waitForStarted(1000);
}

void ItemCatalog::on_lineEdit_db_gitUserEmail_editingFinished()
{
    this->git_Output.clear();
    QStringList args;
    args << "config";
    args << "user.email";
    args << ui->lineEdit_db_gitUserEmail->text();
    process_git.setArguments(args);
    process_git.start();
    process_git.waitForStarted(1000);
}

void ItemCatalog::on_pushButton_insert_clicked()
{
    emit insertItem(m_currentItem->getType(), m_currentItem->wizardParams);
}

void ItemCatalog::on_pushButton_insert_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)
    // right click
}

void ItemCatalog::on_pushButton_save_clicked()
{
    if (!saveModelFile(m_currentVendorDir.absoluteFilePath(QUrl::toPercentEncoding(ui->comboBox_model->currentText()) + ".json"),
                       m_currentItem,
                       ui->textEdit_modelDescription->toPlainText(),
                       m_currentVendorDir.absoluteFilePath(QUrl::toPercentEncoding(ui->lineEdit_modelName->text()) + ".json"))) {
        QMessageBox::warning(this, tr("Save Model"), tr("Could not save model!"));
    }
}

void ItemCatalog::on_pushButton_cancel_clicked()
{
    // reload file
    on_comboBox_model_currentIndexChanged(ui->comboBox_model->currentText());
}

bool ItemCatalog::removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        foreach (QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            } else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }

        result = dir.rmdir(dirName);
    }

    return result;
}
