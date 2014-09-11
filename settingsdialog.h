#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QStackedLayout>
#include <QPushButton>
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

namespace Ui {
class SettingsDialog;
}

class Attribute
{
public:
    QString name;
    QString desc;
    QString type;
    QString value;
    QString min;
    QString max;
};

class OptionsPage : public QObject
{
    Q_OBJECT

public:
    QWidget *widget;
    QString displayName;

    static OptionsPage* newPage(QString displayName, QList<Attribute> attributes);

public slots:
    void slot_showColorDialog();
};

class Category
{
public:
    QString displayName;
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
    void on_buttonBox_apply();

    void on_listWidget_itemClicked(QListWidgetItem *item);

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
    void showPage(int category = 0, int page = 0);
};

#endif // SETTINGSDIALOG_H
