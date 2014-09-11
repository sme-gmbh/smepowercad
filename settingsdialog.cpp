#include "settingsdialog.h"
#include "ui_settingsdialog.h"


SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    stackedLayout(new QStackedLayout)
{
    ui->setupUi(this);

    QT_TR_NOOP("Design");
    QT_TR_NOOP("Colors");
    QT_TR_NOOP("backgroundColor");
    QT_TR_NOOP("Userinterface");
    QT_TR_NOOP("Cursor");
    QT_TR_NOOP("cursorSize");
    QT_TR_NOOP("cursorPickboxSize");
    QT_TR_NOOP("Snap");
    QT_TR_NOOP("snapIndicatorSize");


    stackedLayout->setMargin(0);
    stackedLayout->addWidget(new QWidget(this));

    ui->buttonBox->button(QDialogButtonBox::Ok)->setDefault(true);

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(on_buttonBox_apply()));

    ((QGridLayout *)this->layout())->addLayout(stackedLayout, 1, 1, 1, 1);

    this->loadCategorys();

    foreach (Category *category, categories)
    {
        QListWidgetItem *item = new QListWidgetItem(category->icon, category->displayName);
        ui->listWidget->addItem(item);
    }

    this->showCategory();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    this->save();
    this->accept();
}

void SettingsDialog::on_buttonBox_rejected()
{
    this->reject();
}

void SettingsDialog::on_buttonBox_apply()
{
    this->save();
}

void SettingsDialog::showCategory(int category)
{
    if (category < categories.size())
    {
        Category *cat = categories.at(category);
        ensureCategoryWidget(cat);

        int currentTabIndex = cat->tabWidget->currentIndex();
        if (currentTabIndex != -1)
        {
            OptionsPage *page = cat->pages.at(currentTabIndex);
            currentPage = page;
        }
        ui->listWidget->setCurrentRow(category);
        stackedLayout->setCurrentIndex(cat->index);
        ui->labelHeader->setText(cat->displayName);
    }
}

void SettingsDialog::ensureCategoryWidget(Category *category)
{
    if (category->tabWidget != 0)
        return;

    QTabWidget *tabWidget = new QTabWidget;
    for (int j = 0; j < category->pages.size(); j++)
    {
        OptionsPage *page = category->pages.at(j);
        QWidget *widget = page->widget;
        tabWidget->addTab(widget, tr(page->name.toStdString().c_str()));
    }

    category->tabWidget = tabWidget;
    category->index = stackedLayout->addWidget(tabWidget);
}

void SettingsDialog::showPage(int category, int page)
{
    if (category < categories.size())
    {
        Category *cat = categories.at(category);

        if (page < cat->pages.size())
        {

        }
    }
}

void SettingsDialog::save()
{
    qDebug() << "save";
    foreach (Category *cat, categories)
    {
        qDebug() << "saving cat" << cat->name;
        foreach (OptionsPage *page, cat->pages)
        {
            qDebug() << "saving page" << page->name;
            QFormLayout *layout = (QFormLayout *)page->widget->layout();
            for (int r = 0; r < layout->rowCount(); r++)
            {
                //QString label = ((QLabel *)layout->itemAt(r, QFormLayout::LabelRole)->widget())->text();
                QWidget *wdg = layout->itemAt(r, QFormLayout::FieldRole)->widget();
                QString name = wdg->objectName();
                qDebug() << "saving wdg" << name;


                Attribute at = page->attributes.value(name);
                QString key = cat->name + "_" + page->name + "_" + at.name;
                if (at.type == "int")
                {
                    settings.setValue(key, QVariant::fromValue(((QSpinBox *)wdg)->value()));
                }
                else if (at.type == "color")
                {
                    QColor col = ((QPushButton *)wdg)->icon().pixmap(1, 1).toImage().pixel(0, 0);
                    settings.setValue(key, QVariant::fromValue(col));
                    qDebug() << key << col;
                    qDebug() << settings.value(key);
                }
            }
        }
    }
}

void SettingsDialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    this->showCategory(ui->listWidget->currentRow());
}

void SettingsDialog::loadCategorys()
{
    while (!categories.isEmpty())
        delete categories.takeFirst();
    categories.clear();

    QDomDocument doc;
    QFile file("../../src/settings.xml");
    if (!file.open(QFile::ReadOnly))
        return;

    if (!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root = doc.documentElement();

    if (root.tagName() != "settings")
        return;

    for (int c = 0; c < root.childNodes().size(); c++)
    {
        QDomNode category = root.childNodes().at(c);
        Category *cat = new Category;
        cat->tabWidget = 0;
        cat->name = category.attributes().namedItem("name").nodeValue();
        cat->displayName = tr(cat->name.toStdString().c_str());
        cat->icon = QIcon(":/settings/icons/" + category.attributes().namedItem("icon").nodeValue());

        QList<OptionsPage *> pages;
        for (int p = 0; p < category.childNodes().size(); p++)
        {
            QDomNode page = category.childNodes().at(p);
            QList<Attribute> attributes;
            for (int a = 0; a < page.childNodes().size(); a++)
            {
                QDomNode attr = page.childNodes().at(a);
                Attribute at;
                at.name = attr.attributes().namedItem("name").nodeValue();
                at.displayName = tr(at.name.toStdString().c_str());
                at.desc = attr.attributes().namedItem("desc").nodeValue();
                at.type = attr.attributes().namedItem("type").nodeValue();
                QVariant defaultVal;
                QString dflt = attr.attributes().namedItem("default").nodeValue();
                if (at.type == "int")
                    defaultVal = QVariant::fromValue(dflt.toInt());
                else if (at.type == "color")
                {
                    QStringList parts = dflt.split(",");
                    QColor col = QColor(parts.at(0).toInt(), parts.at(1).toInt(), parts.at(2).toInt(), parts.at(3).toInt());
                    defaultVal = QVariant::fromValue(col);
                }
                at.value = settings.value(cat->name + "_" + page.attributes().namedItem("name").nodeValue() + "_" + at.name, defaultVal);
                qDebug() << cat->name + "_" + page.attributes().namedItem("name").nodeValue() + "_" + at.name << at.value;
                at.min = attr.attributes().namedItem("min").nodeValue();
                at.max = attr.attributes().namedItem("max").nodeValue();
                attributes.append(at);
            }
            OptionsPage *npage = OptionsPage::newPage(page.attributes().namedItem("name").nodeValue(), attributes);
            pages.append(npage);
        }
        cat->pages = pages;
        categories.append(cat);
    }
}


OptionsPage* OptionsPage::newPage(QString name, QList<Attribute> attributes)
{
    OptionsPage *page = new OptionsPage;

    page->displayName = tr(name.toStdString().c_str());
    page->name = name;

    QFormLayout *layout = new QFormLayout();

    QMap<QString, Attribute> attrs;
    foreach (Attribute attr, attributes)
    {
        attrs.insert(attr.name, attr);
        if (attr.type == "int")
        {
            QSpinBox *box = new QSpinBox;
            box->setObjectName(attr.name);
            box->setMinimum(attr.min.toInt());
            box->setMaximum(attr.max.toInt());
            box->setValue(attr.value.toInt());
            box->setMaximumWidth(150);
            layout->addRow(attr.displayName, box);
        }
        else if (attr.type == "color")
        {
            QColor col = attr.value.value<QColor>();
            qDebug() << col;
            QPixmap pxmp = QPixmap(24, 24);
            pxmp.fill(col);
            QPushButton *btn = new QPushButton(QIcon(pxmp), "");
            btn->setObjectName(attr.name);
            btn->setMaximumWidth(34);
            connect(btn, SIGNAL(clicked()), page, SLOT(slot_showColorDialog()));
            layout->addRow(attr.displayName, btn);


        }
    }
    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    page->widget = widget;
    page->attributes = attrs;

    return page;
}

void OptionsPage::slot_showColorDialog()
{
    QPushButton *btn = (QPushButton *)this->sender();
    QColor init = btn->icon().pixmap(1, 1).toImage().pixel(0, 0);
    QColorDialog colorDialog(init, 0);
    colorDialog.setWindowTitle(tr("Select color"));
    colorDialog.setModal(true);
    colorDialog.setOption(QColorDialog::ShowAlphaChannel, true);
    if (colorDialog.exec() == QDialog::Rejected)
        return;

    QPixmap pxmp = QPixmap(24, 24);
    pxmp.fill(colorDialog.currentColor());
    btn->setIcon(QIcon(pxmp));
}
