#ifndef MODALDIALOG_H
#define MODALDIALOG_H

#include <QDialog>
#include <QBitmap>
#include <QPainter>

namespace Ui {
class ModalDialog;
}

class ModalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModalDialog(QString title, QMap<QString, QString> data, QWidget *parent = 0);
    ~ModalDialog();

    virtual void resizeEvent(QResizeEvent *event);

private:
    Ui::ModalDialog *ui;
};

#endif // MODALDIALOG_H
