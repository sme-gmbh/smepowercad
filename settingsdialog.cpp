#include "settingsdialog.h"
#include "ui_settingsdialog.h"


SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    stackedLayout(new QStackedLayout)
{
    ui->setupUi(this);

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
    // save
    this->accept();
}

void SettingsDialog::on_buttonBox_rejected()
{
    this->reject();
}

void SettingsDialog::on_buttonBox_apply()
{
    // save
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
        qDebug() << "for" << page->displayName;
        QWidget *widget = page->widget;
        tabWidget->addTab(widget, page->displayName);
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
        cat->displayName = category.attributes().namedItem("name").nodeValue();
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
                at.desc = attr.attributes().namedItem("desc").nodeValue();
                at.type = attr.attributes().namedItem("type").nodeValue();
                at.value = attr.attributes().namedItem("value").nodeValue();
                at.min = attr.attributes().namedItem("min").nodeValue();
                at.max = attr.attributes().namedItem("max").nodeValue();
                attributes.append(at);
            }
            OptionsPage *npage = OptionsPage::newPage(page.attributes().namedItem("name").nodeValue(),
                                                      attributes);
            pages.append(npage);
        }
        cat->pages = pages;
        categories.append(cat);
    }
}


OptionsPage* OptionsPage::newPage(QString displayName, QList<Attribute> attributes)
{
    OptionsPage *page = new OptionsPage;

    page->displayName = displayName;

    QFormLayout *layout = new QFormLayout();

    foreach (Attribute attr, attributes)
    {
        if (attr.type == "int")
        {
            QSpinBox *box = new QSpinBox;
            box->setMinimum(attr.min.toInt());
            box->setMaximum(attr.max.toInt());
            box->setValue(attr.value.toInt());
            box->setMaximumWidth(150);
            layout->addRow(attr.name, box);
        }
        else if (attr.type == "color")
        {
            QColor col;
            QStringList parts = attr.value.split(",");
            col = QColor(parts.at(0).toInt(), parts.at(1).toInt(), parts.at(2).toInt(), parts.at(3).toInt());

            QPixmap pxmp = QPixmap(24, 24);
            pxmp.fill(col);
            QPushButton *btn = new QPushButton(QIcon(pxmp), "");
            btn->setMaximumWidth(34);
            connect(btn, SIGNAL(clicked()), page, SLOT(slot_showColorDialog()));
            layout->addRow(attr.name, btn);


        }
    }
    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    page->widget = widget;

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
