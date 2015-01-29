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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QStackedLayout>
#include <QPushButton>
#include <QComboBox>
#include <QSettings>
#include <QTabWidget>
#include <QListWidgetItem>
#include <QDomDocument>
#include <QFile>
#include <QFormLayout>
#include <QSpinBox>
#include <QDebug>
#include <QColorDialog>
#include <QSettings>
#include <QObject>
#include <QMessageBox>

namespace Ui {
class SettingsDialog;
}

class Attribute
{
public:
    QString displayName;
    QString name;
    QString desc;
    QString type;
    QVariant value;
    QString min;
    QString max;
    QStringList displayValues;
    QStringList values;
};

class OptionsPage : public QObject
{
    Q_OBJECT

public:
    QWidget *widget;
    QString displayName;
    QString name;
    QMap<QString, Attribute> attributes;

    static OptionsPage* newPage(QString displayName, QList<Attribute> attributes);

public slots:
    void slot_showColorDialog();
};

class Category
{
public:
    QString displayName;
    QString name;
    QTabWidget *tabWidget;
    QList<OptionsPage *> pages;
    QIcon icon;
    int index;
};

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_listWidget_itemClicked(QListWidgetItem *item);

    void slot_reload();

private:
    Ui::SettingsDialog *ui;
    QSettings settings;

    QList<Category *> categories;

    Category *currentCategory;
    OptionsPage *currentPage;
    QStackedLayout *stackedLayout;

    void loadCategorys();
    void showCategory(int category = 0);
    void ensureCategoryWidget(Category *category);

    void save();
    void reset();

signals:
    void signal_settingsChanged();
};

#endif // SETTINGSDIALOG_H
