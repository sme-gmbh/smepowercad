#include <QCursor>
#include <QDebug>
#include <QToolButton>
#include <QLabel>
#include "toolwidget.h"
#include "ui_toolwidget.h"

int ToolWidget::s_domainIndex = 0;

ToolWidget::ToolWidget(ItemDB *itemDB, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolWidget)
{
    this->itemDB = itemDB;
    ui->setupUi(this);
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    this->setMouseTracking(true);
    this->isOpen = false;

    this->setBackgroundColor(QColor::fromHsv(50 * s_domainIndex, 100, 200));
    s_domainIndex++;
}

ToolWidget::~ToolWidget()
{
    this->deleteWdgs(ui->gridLayout);
    delete ui;
}

void ToolWidget::setDomain(QString domain)
{
    this->domain = domain;
    ui->label->setText(domain);
}

void ToolWidget::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();
}

void ToolWidget::mousePressEvent(QMouseEvent *event)
{
    if (ui->label->rect().adjusted(3, 3, 3, 3).contains(event->pos()))  // BUG: Rect coordinates are not correct, adjust needed (why?)
    {
        if (!this->isOpen)
            this->displayItemButtons();
        else
            this->deleteWdgs(ui->gridLayout);
    }
    event->accept();
}

void ToolWidget::enterEvent(QEvent *event)
{
//    this->displayItemButtons();
    event->accept();
}

void ToolWidget::leaveEvent(QEvent *event)
{
//    this->deleteWdgs(ui->gridLayout);
    event->accept();
}

void ToolWidget::displayItemButtons()
{
    if (isOpen)
        return;

    qDebug() << domain << "displayItemButtons";

    QList<int> items = itemDB->getItemTypesByDomain(this->domain);

//    int columnCount = sqrt(buttonCount);
    int columnCount = 15;
    int column = 0;
    int row = 0;

    foreach(int type, items)
    {
        QIcon icon = itemDB->getIconByItemType((CADitem::ItemType)type, QSize(32, 32));
        QString description = itemDB->getItemDescriptionByItemType((CADitem::ItemType)type);
        QToolButton* button = new QToolButton(this);
        button->setStyleSheet("border = 0;");
        button->setContextMenuPolicy(Qt::CustomContextMenu);
        button->setMinimumSize(42, 42);
        button->setMaximumSize(42, 42);
        button->setFocusPolicy(Qt::NoFocus);
        button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        button->setIconSize(QSize(32, 32));
        button->setIcon(icon);
        button->setToolTip(description);
        button->setProperty("ItemType", QVariant(type));

        connect(button, SIGNAL(clicked()), this, SLOT(slot_button_clicked()));
        connect(button, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slot_button_rightClicked()));


        ui->gridLayout->addWidget(button, row, column);
        column++;
        if (column == columnCount)
        {
            column = 0;
            row++;
        }
    }

    this->isOpen = true;
}

void ToolWidget::deleteWdgs(QLayout *layout)
{
    qDebug() << domain << isOpen;
    if (!this->isOpen)
        return;

    qDebug() << domain << "deleteWdgs" << (layout != NULL);

    QLayoutItem *item;
    while ((item = layout->takeAt(0)))
    {
//        if (item->layout())
//        {
//            deleteWdgs(item->layout());
//            delete item->layout();
//        }
        if (item->widget())
        {
            delete item->widget();
        }
        delete item;
    }

    this->layout()->removeItem(ui->gridLayout);
    ui->gridLayout->deleteLater();
    ui->gridLayout = new QGridLayout();
    ((QVBoxLayout*)this->layout())->insertLayout(1, ui->gridLayout);

    this->isOpen = false;
}

void ToolWidget::setBackgroundColor(QColor color)
{
    QString stylesheet = this->styleSheet();
    stylesheet.prepend(QString().sprintf("QLabel{background-color: rgb(%d, %d, %d);}", color.red(), color.green(), color.green()));
    this->setStyleSheet(stylesheet);
}

void ToolWidget::slot_button_clicked()
{
    QToolButton* button = (QToolButton*)this->sender();
    CADitem::ItemType type = (CADitem::ItemType)button->property("ItemType").toInt();
    emit signal_newItemRequested(type);
}

void ToolWidget::slot_button_rightClicked()
{

}
