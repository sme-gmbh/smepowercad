#ifndef PRINTWIDGET_H
#define PRINTWIDGET_H

#include <QDockWidget>
#include <QPrintDialog>
#include <QPrinter>

namespace Ui {
class PrintWidget;
}

class PrintWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit PrintWidget(QWidget *parent = 0);
    ~PrintWidget();

private slots:
    void on_toolButton_printer_clicked();

private:
    Ui::PrintWidget *ui;
    QPrinter* printer;
};

#endif // PRINTWIDGET_H
