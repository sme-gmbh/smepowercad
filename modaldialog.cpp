#include "modaldialog.h"
#include "ui_modaldialog.h"

ModalDialog::ModalDialog(QString title, QStringList data, QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint),
    ui(new Ui::ModalDialog)
{
    ui->setupUi(this);

    //this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowOpacity(0.8);

    ui->labelTitle->setText(title);

    if (data.length() % 2 != 0) return;
    for (int i = 0; i < data.length(); i = i+2)
    {
        QLabel *labelVal = new QLabel(data.at(i+1));
        ui->formLayout->addRow(data.at(i), labelVal);
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
