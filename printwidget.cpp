#include "printwidget.h"
#include "ui_printwidget.h"

PrintWidget::PrintWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::PrintWidget)
{
    ui->setupUi(this);
}

PrintWidget::~PrintWidget()
{
    delete ui;
}
