#include "modaldialog.h"
#include "ui_modaldialog.h"

ModalDialog::ModalDialog(QString title, QMap<QString, QString> data, QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint),
    ui(new Ui::ModalDialog)
{
    ui->setupUi(this);

    //this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowOpacity(0.8);

    ui->labelTitle->setText(title);

    QMap<QString, QString>::iterator i;
    for (i = data.begin(); i != data.end(); i++)
    {
        QLabel *labelKey = new QLabel(i.key());
        QLabel *labelVal = new QLabel(i.value());
        ui->formLayout->addRow(labelKey, labelVal);
    }
    this->adjustSize();
}

ModalDialog::~ModalDialog()
{
    delete ui;
}

void ModalDialog::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    QBitmap bmp(this->size());
    bmp.fill(Qt::color0);
    QPainter painter(&bmp);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::color1);
    painter.drawRoundedRect(this->rect(), 20, 20);
    painter.end();
    setMask(bmp);
}
