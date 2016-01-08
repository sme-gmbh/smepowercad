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

#ifndef ITEMCATALOG_H
#define ITEMCATALOG_H

#include <QDir>
#include <QDirIterator>
#include <QDockWidget>
#include <QFile>
#include <QInputDialog>
#include <QList>
#include <QStringList>
#include <QMessageBox>
#include <QProcess>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QUrl>

#include "itemdb.h"
#include "itemwizard.h"

#define JSON_KEY_PARAMETERS "parameters"
#define JSON_KEY_DESCRIPTION "description"

namespace Ui {
class ItemCatalog;
}

class ItemCatalog : public QDockWidget
{
    Q_OBJECT

public:
    explicit ItemCatalog(ItemDB* m_itemdb,  ItemWizard* itemWizard, QWidget *parent = 0);
    ~ItemCatalog();

private slots:
    void slot_processGit_started();
    void slot_processGit_finished(int exitCode ,QProcess::ExitStatus exitStatus);
    void slot_processGit_readyRead();
    void slot_processGit_error(QProcess::ProcessError error);


    void on_comboBox_domain_currentIndexChanged(const QString &arg1);
    void on_comboBox_itemType_currentIndexChanged(const QString &arg1);
    void on_comboBox_vendor_currentIndexChanged(const QString &arg1);
    void on_comboBox_model_currentIndexChanged(const QString &arg1);

    void on_toolButton_addVendor_clicked();
    void on_toolButton_removeVendor_clicked();
    void on_toolButton_addModel_clicked();
    void on_toolButton_removeModel_clicked();

    void on_pushButton_db_gitClone_clicked();
    void on_pushButton_db_gitStatus_clicked();
    void on_pushButton_db_gitLog_clicked();
    void on_pushButton_db_gitPull_clicked();
    void on_pushButton_db_gitCommit_clicked();
    void on_pushButton_db_gitPush_clicked();
    void on_lineEdit_db_gitUserName_editingFinished();
    void on_lineEdit_db_gitUserEmail_editingFinished();

    void on_pushButton_insert_clicked();
    void on_pushButton_insert_customContextMenuRequested(const QPoint &pos);
    void on_pushButton_save_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::ItemCatalog *ui;
    ItemDB* m_itemdb;
    ItemWizard* itemWizard;
    QDir m_catalogDir;
    QDir m_currentDomainDir;
    QDir m_currentItemDir;
    QDir m_currentVendorDir;
    CADitem *m_currentItem;
    QWidget *m_currentWidget;
    QList<int> m_currentDomainItemTypes;
    QProcess process_git;
    QString git_Output;

    void initialize();
    bool setupLocalDirectory();
    bool setupGitProcess();
    void getGitConfig();

    QStringList toPercentEncoding(QStringList list);
    QStringList fromPercentEncoding(QStringList list);

    QJsonObject readDataFromModelFile(QString filename);
    bool saveModelFile(QString filename, CADitem *item, QString description, QString newFilename = QString());
    void showModelData(CADitem *item, QString name, QString description);
    void clearModelData();

    bool removeDir(const QString &dirName);

signals:
    void insertItem(CADitemTypes::ItemType type, WizardParams wizardParams);
};

#endif // ITEMCATALOG_H
