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

#include "printwidget.h"
#include "ui_printwidget.h"


#include <QMessageBox>

PrintWidget::PrintWidget(QWidget *parent, ItemDB *itemDB) :
    QDockWidget(parent),
    ui(new Ui::PrintWidget)
{
    ui->setupUi(this);
    this->itemDB = itemDB;
    this->printPaperTemplate = new PrintPaperTemplate(this);
    this->glWidget = new GLWidget(this, itemDB);
    QVBoxLayout* layout = new QVBoxLayout(ui->graphicWidget);
    layout->addWidget(this->glWidget);
    this->glWidget->slot_set_cuttingplane_values_changed(100000.0, 100000.0);
}

PrintWidget::~PrintWidget()
{
    delete ui;
//    delete printPaperTemplate;
}

void PrintWidget::on_toolButton_printer_clicked()
{
    QPrintDialog* printDialog = new QPrintDialog(this);
    printDialog->setPalette(this->palette());

    if (printDialog->exec() == QDialog::Rejected)
        return;

    printer = printDialog->printer();
}

void PrintWidget::on_toolButton_print_clicked()
{
    if (printer = NULL)
    {
        QMessageBox::warning(this, tr("Print/Plot"), tr("There is no printer selected..."));
        return;
    }


}

void PrintWidget::on_toolButton_layout_clicked()
{
    this->printPaperTemplate->showMaximized();
}
