#include "settingsdialog.h"
#include "ui_settingsdialog.h"


SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    stackedLayout(new QStackedLayout)
{
    ui->setupUi(this);

    QT_TR_NOOP("environment");
    QT_TR_NOOP("general");
    QT_TR_NOOP("language");
    QT_TR_NOOP("de_DE");
    QT_TR_NOOP("en_US");
    QT_TR_NOOP("Design");
    QT_TR_NOOP("Colors");
    QT_TR_NOOP("backgroundColor");
    QT_TR_NOOP("Userinterface");
    QT_TR_NOOP("Cursor");
    QT_TR_NOOP("cursorSize");
    QT_TR_NOOP("cursorLineWidth");
    QT_TR_NOOP("cursorPickboxSize");
    QT_TR_NOOP("Snap");
    QT_TR_NOOP("snapIndicatorSize");
    QT_TR_NOOP("pickbox");
    QT_TR_NOOP("pickboxOutlineWidth");
    QT_TR_NOOP("pickboxOutlineColorLeft");
    QT_TR_NOOP("pickboxOutlineColorRight");
    QT_TR_NOOP("pickboxFillColorLeft");
    QT_TR_NOOP("pickboxFillColorRight");
    QT_TR_NOOP("arcball");
    QT_TR_NOOP("arcballRadius");


    stackedLayout->setMargin(0);
    stackedLayout->addWidget(new QWidget(this));

    ui->buttonBox->button(QDialogButtonBox::Ok)->setDefault(true);

    ((QGridLayout *)this->layout())->addLayout(stackedLayout, 1, 1, 1, 1);

    connect(this, SIGNAL(signal_settingsChanged()), this, SLOT(slot_reload()));

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

void SettingsDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    switch (ui->buttonBox->standardButton(button))
    {
    case QDialogButtonBox::Apply:
        this->save();
        break;
    case QDialogButtonBox::Reset:
        this->reset();
        break;
    default:
        break;
    }
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

void SettingsDialog::save()
{
    foreach (Category *cat, categories)
    {
        foreach (OptionsPage *page, cat->pages)
        {
            QFormLayout *layout = (QFormLayout *)page->widget->layout();
            for (int r = 0; r < layout->rowCount(); r++)
            {
                QWidget *wdg = layout->itemAt(r, QFormLayout::FieldRole)->widget();
                QString name = wdg->objectName();


                Attribute at = page->attributes.value(name);
                QString key = cat->name + "_" + page->name + "_" + at.name;
                if (at.type == "int")
                {
                    settings.setValue(key, QVariant::fromValue(((QSpinBox*)wdg)->value()));
                }
                else if (at.type == "color")
                {
                    QColor col = at.value.value<QColor>();
                    settings.setValue(key, QVariant::fromValue(col));
                }
                else if (at.type == "dropdown")
                {
                    if (at.name == "language")
                    {
                        QString oldLang = settings.value(key).toString();
                        QString newLang = at.values.at(((QComboBox*)wdg)->currentIndex());
                        if (oldLang != newLang)
                            QMessageBox::information(this, tr("Changed Language"), tr("The language will change at the next start of the the Application."));
                    }
                    settings.setValue(key, QVariant::fromValue(at.values.at(((QComboBox*)wdg)->currentIndex())));
                }
            }
        }
    }
    emit signal_settingsChanged();
}

void SettingsDialog::reset()
{
    int ret = QMessageBox::critical(this, tr("Reset Settings"), tr("Do you really want to reset all Settings?\nThis action can not be undone!"),
                                    QMessageBox::Cancel, QMessageBox::Ok);
    if (ret != QMessageBox::Ok)
        return;

    foreach (Category *cat, categories)
    {
        foreach (OptionsPage *page, cat->pages)
        {
            foreach (Attribute attr, page->attributes)
            {
                settings.remove(cat->name + "_" + page->name + "_" + attr.name);
            }
        }
    }
    emit signal_settingsChanged();
}

void SettingsDialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    this->showCategory(ui->listWidget->currentRow());
}

void SettingsDialog::slot_reload()
{
    this->loadCategorys();

    int curRow = ui->listWidget->currentRow();
    ui->listWidget->clear();

    foreach (Category *category, categories)
    {
        QListWidgetItem *item = new QListWidgetItem(category->icon, category->displayName);
        ui->listWidget->addItem(item);
    }

    this->showCategory(curRow);
}

void SettingsDialog::loadCategorys()
{
    while (!categories.isEmpty())
        delete categories.takeFirst();
    categories.clear();

    QDomDocument doc;
    QFile file("../../src/settings.xml"); // TODO: move to resource file
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
        cat->icon = QIcon(":/settings/icons/" + category.attributes().namedItem("icon").nodeValue()); // TODO: add icons for categories

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
                else if (at.type == "dropdown")
                {
                    QStringList parts;
                    QStringList split = attr.attributes().namedItem("values").nodeValue().split(",");
                    at.values = split;
                    defaultVal = QVariant::fromValue(dflt);
                    for (int i = 0; i < split.length(); i++)
                        parts.append(tr(split.at(i).toStdString().c_str()));

                    at.displayValues = parts;
                }
                at.value = settings.value(cat->name + "_" + page.attributes().namedItem("name").nodeValue() + "_" + at.name, defaultVal);
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
            QPixmap pxmp = QPixmap(24, 24);
            pxmp.fill(col);
            QPushButton *btn = new QPushButton(QIcon(pxmp), "");
            btn->setObjectName(attr.name);
            btn->setMaximumWidth(34);
            connect(btn, SIGNAL(clicked()), page, SLOT(slot_showColorDialog()));
            layout->addRow(attr.displayName, btn);
        }
        else if (attr.type == "dropdown")
        {
            QComboBox *box = new QComboBox;
            box->setObjectName(attr.name);
            box->setMaximumWidth(150);
            foreach (QString row, attr.displayValues)
                box->addItem(row);


            box->setCurrentIndex(attr.values.indexOf(attr.value.toString()));
            layout->addRow(attr.displayName, box);
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
    Attribute attr = this->attributes.value(btn->objectName());
    QColorDialog colorDialog(attr.value.value<QColor>(), 0);
    colorDialog.setWindowTitle(tr("Select color"));
    colorDialog.setModal(true);
    colorDialog.setOption(QColorDialog::ShowAlphaChannel, true);
    if (colorDialog.exec() == QDialog::Rejected)
        return;

    QPixmap pxmp = QPixmap(24, 24);
    pxmp.fill(colorDialog.currentColor());
    btn->setIcon(QIcon(pxmp));

    attr.value = QVariant::fromValue(colorDialog.currentColor());
    attributes.insert(btn->objectName(), attr);
}
