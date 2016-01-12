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

#ifndef PRINTWIDGET_H
#define PRINTWIDGET_H

#include <QDockWidget>
#include <QLayout>
#include <QMessageBox>
#include <QOpenGLContext>
#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>

#include "printpapertemplate.h"
#include "itemdb.h"
#include "glwidget.h"

namespace Ui {
class PrintWidget;
}

class PrintWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit PrintWidget(QWidget *parent, ItemDB *itemDB);
    ~PrintWidget();

private slots:
    void on_toolButton_printer_clicked();

    void on_toolButton_print_clicked();

    void on_toolButton_layout_clicked();

private:
    Ui::PrintWidget *ui;
    QPrinter* printer;
    PrintPaperTemplate* printPaperTemplate;
    ItemDB* itemDB;
    GLWidget* glWidget;

signals:
    void printscriptsUpdateNeeded();
};

#endif // PRINTWIDGET_H
