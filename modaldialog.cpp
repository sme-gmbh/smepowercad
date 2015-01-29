/**********************************************************************
** smepowercad
** Copyright (C) 2015 Smart Micro Engineering GmbH
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/

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
