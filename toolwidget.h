#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include "itemdb.h"

namespace Ui {
class ToolWidget;
}

class ToolWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ToolWidget(ItemDB* itemDB, QWidget *parent = 0);
    ~ToolWidget();

    void setDomain(QString domain);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);

private:
    Ui::ToolWidget *ui;
    ItemDB* itemDB;
    QString domain;
    bool isOpen;

    void displayItemButtons();
    void deleteWdgs(QLayout *layout);

private slots:
    void slot_button_clicked();

public slots:

signals:
    void signal_newItemRequested(CADitem::ItemType type);
};

#endif // TOOLWIDGET_H
