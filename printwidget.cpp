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

void PrintWidget::on_toolButton_printer_clicked()
{
    QPrintDialog* printDialog = new QPrintDialog(this);

    if (printDialog->exec() == QDialog::Rejected)
        return;

    printer = printDialog->printer();
}
