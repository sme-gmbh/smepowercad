#include "modaldialog.h"
#include "ui_modaldialog.h"

ModalDialog::ModalDialog(QString title, QStringList data, QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint),
    ui(new Ui::ModalDialog)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_OpaquePaintEvent);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowOpacity(0.8);

    ui->labelTitle->setText(title);

    if (data.length() % 2 != 0) return;
    for (int i = 0; i < data.length(); i = i+2)
    {
        QLabel *labelVal = new QLabel(data.at(i+1), this);
        ui->formLayout->addRow(data.at(i), labelVal);
    }
    this->adjustSize();
}

ModalDialog::~ModalDialog()
{
    delete ui;
}

void ModalDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    QPen pen = QPen(QColor(255, 255, 255, 200));
    pen.setWidth(2);
    painter.setPen(pen);
    painter.setBrush(QColor(0, 0, 0));
    QRectF rect = QRectF(this->rect());
    painter.drawRoundedRect(rect.adjusted(1.0, 1.0, -1.0, -1.0), 20, 20);
    painter.end();
    event->accept();
}
