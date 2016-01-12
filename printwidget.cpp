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
#include "logging.h"

PrintWidget::PrintWidget(QWidget *parent, ItemDB *itemDB) :
    QDockWidget(parent),
    ui(new Ui::PrintWidget)
{
    this->setStyleSheet(StylesheetProvider::getStylesheet("Button"));
    ui->setupUi(this);
    this->printer = NULL;
    this->itemDB = itemDB;
    this->glWidget = new GLWidget(this, itemDB);
    ui->verticalLayout->replaceWidget(ui->placeholderWidget, glWidget);
    this->printPaperTemplate = new PrintPaperTemplate(this, this->glWidget, itemDB);
    connect(this, &PrintWidget::printscriptsUpdateNeeded, printPaperTemplate, &PrintPaperTemplate::onPrintscriptsUpdate);
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
    if (printer == NULL)
    {
        QMessageBox::warning(this, tr("Print/Plot"), tr("There is no printer selected..."));
        return;
    }

    printer->setColorMode(QPrinter::Color);
    printer->setDuplex(QPrinter::DuplexNone);
    printer->setOrientation(QPrinter::Landscape);
    printer->setDoubleSidedPrinting(false);
    printer->setFontEmbeddingEnabled(true);
    printer->setFullPage(true);
    printer->setResolution(600);
    printer->setPaperSize(QPrinter::A3);

    QPainter painter(printer);

    QString script = this->printPaperTemplate->getScriptFromEditor();
    this->printPaperTemplate->setScript(script);
//    this->printPaperTemplate->parseScript(NULL);

    // Production of graphic content
    this->printPaperTemplate->parseScript(&painter);

    painter.end();
}

void PrintWidget::on_toolButton_layout_clicked()
{
    this->printPaperTemplate->showMaximized();
}
