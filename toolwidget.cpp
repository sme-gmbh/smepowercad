#include "toolwidget.h"
#include "ui_toolwidget.h"

ToolWidget::ToolWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolWidget)
{
    ui->setupUi(this);
    this->setMouseTracking(true);
}

ToolWidget::~ToolWidget()
{
    delete ui;
}

void ToolWidget::setDomain(QString domain)
{
    this->domain = domain;
}

void ToolWidget::mouseMoveEvent()
{

}
