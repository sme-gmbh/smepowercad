#ifndef PRINTWIDGET_H
#define PRINTWIDGET_H

#include <QDockWidget>

namespace Ui {
class PrintWidget;
}

class PrintWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit PrintWidget(QWidget *parent = 0);
    ~PrintWidget();

private:
    Ui::PrintWidget *ui;
};

#endif // PRINTWIDGET_H
