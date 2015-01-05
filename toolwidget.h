#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

#include <QWidget>

namespace Ui {
class ToolWidget;
}

class ToolWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ToolWidget(QWidget *parent = 0);
    ~ToolWidget();

    void setDomain(QString domain);

protected:
    virtual void mouseMoveEvent();

private:
    Ui::ToolWidget *ui;
    QString domain;
};

#endif // TOOLWIDGET_H
