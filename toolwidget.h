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
    static int s_domainIndex;

    void displayItemButtons();
    void deleteWdgs(QLayout *layout);
    void setBackgroundColor(QColor color);

private slots:
    void slot_button_clicked();
    void slot_button_rightClicked();

public slots:

signals:
    void signal_newItemRequested(CADitemTypes::ItemType type);
};

#endif // TOOLWIDGET_H
