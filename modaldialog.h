#ifndef MODALDIALOG_H
#define MODALDIALOG_H

#include <QDialog>
#include <QPainter>
#include <QPaintEvent>

namespace Ui {
class ModalDialog;
}

class ModalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModalDialog(QString title, QStringList data, QWidget *parent = 0);
    ~ModalDialog();
    virtual void paintEvent(QPaintEvent *event);

private:
    Ui::ModalDialog *ui;
};

#endif // MODALDIALOG_H
