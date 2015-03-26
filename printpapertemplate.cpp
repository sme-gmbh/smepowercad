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

#include "printpapertemplate.h"
#include "ui_printpapertemplate.h"

#include <QTextStream>
#include <QRectF>
#include <QColor>
#include <QRegExp>
#include <QImage>
#include <QPixmap>
#include <qmath.h>
#include <QDebug>

PrintPaperTemplate::PrintPaperTemplate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::printPaperTemplate)
{
    ui->setupUi(this);
}

PrintPaperTemplate::~PrintPaperTemplate()
{
    delete ui;
}


void PrintPaperTemplate::paintSetPaperSize(QString arguments)
{
    if      (arguments == "DIN A0")
        paperSize = QSizeF(1189.0, 841.0);
    else if (arguments == "DIN A1")
        paperSize = QSizeF(841.0, 594.0);
    else if (arguments == "DIN A2")
        paperSize = QSizeF(594.0, 420.0);
    else if (arguments == "DIN A3")
        paperSize = QSizeF(420.0, 297.0);
    else if (arguments == "DIN A4")
        paperSize = QSizeF(297.0, 210.0);
    else if (arguments.contains(QRegExp("mm.+mm$")))
    {
        arguments.remove(QRegExp("[^\\d\\.\\,(mm)]"));
        QStringList coordStrings = arguments.split(',');
        if (coordStrings.size() != 2)
            return;

        qreal size_x = this->text_to_pixel(coordStrings.at(0));
        qreal size_y = this->text_to_pixel(coordStrings.at(1));
        paperSize = QSizeF(size_x, size_y);
    }
}

void PrintPaperTemplate::paintBorder(QPainter* painter)
{
    QRectF paperRect = QRectF(0.0, 0.0, mm_to_pixel(this->paperSize.width()), mm_to_pixel(this->paperSize.height()));

    // Outer border (paper cutting rect)
    painter->drawRect(paperRect);

    // Innter border
    int pixel_5mm = this->mm_to_pixel(5.0);
    painter->drawRect(paperRect.adjusted(mm_to_pixel(20.0), pixel_5mm, -pixel_5mm, -pixel_5mm));
}

void PrintPaperTemplate::paintFoldMarking(QPainter* painter)
{
    QRectF paperRect = QRectF(0.0, 0.0, mm_to_pixel(this->paperSize.width()), mm_to_pixel(this->paperSize.height()));

    int x = paperRect.width() - 1;
    int y = paperRect.height() - 1;

    // Vertical 190 mm
    while (x > mm_to_pixel(210.0 + 2 * 190.0))
    {
        for (int i=0; i<2; i++)
        {
            x -= mm_to_pixel(190.0);
            painter->drawLine(x, paperRect.top(), x, paperRect.top() + mm_to_pixel(5.0));
            painter->drawLine(x, paperRect.bottom(), x, paperRect.bottom() - mm_to_pixel(5.0));
        }
    }

    // Vertical intermediate fold
    x = (x + mm_to_pixel(210.0)) / 2;
    painter->drawLine(x, paperRect.top(), x, paperRect.top() + mm_to_pixel(5.0));
    painter->drawLine(x, paperRect.bottom(), x, paperRect.bottom() - mm_to_pixel(5.0));

    // marking vertical 210 mm from left
    x = mm_to_pixel(210.0);
    painter->drawLine(x, paperRect.top(), x, paperRect.top() + mm_to_pixel(5.0));
    painter->drawLine(x, paperRect.bottom(), x, paperRect.bottom() - mm_to_pixel(5.0));


    // horizontal marking
    painter->drawLine(paperRect.left(), y - mm_to_pixel(297.0), paperRect.left() + mm_to_pixel(5.0), y - mm_to_pixel(297.0));
    while (y >= mm_to_pixel(297.0))
    {
        y -= mm_to_pixel(297.0);
        painter->drawLine(paperRect.right(), y, paperRect.right() - mm_to_pixel(5.0), y);
    }

    // Diagonal marking top left
    QLineF line (paperRect.left() + mm_to_pixel(105), paperRect.top(), paperRect.left(), paperRect.bottom() - mm_to_pixel(297));
    line.setLength(mm_to_pixel(5.0 / qAbs(qSin(line.angle() / 360.0 * 2.0 * 3.1415))));
    painter->drawLine(line);

    // Diagonal marking center left
    line = QLineF (paperRect.left(), paperRect.bottom() - mm_to_pixel(297), paperRect.left() + mm_to_pixel(105), paperRect.top());
    line.setLength(mm_to_pixel(5.0 / qCos(line.angle() / 360.0 * 2.0 * 3.1415)));
    painter->drawLine(line);
}

void PrintPaperTemplate::paintSave(QPainter *painter)
{
    painter->save();
}

void PrintPaperTemplate::paintRestore(QPainter *painter)
{
    painter->restore();
}

void PrintPaperTemplate::paintResetTransform(QPainter *painter)
{
    painter->resetMatrix();
}

void PrintPaperTemplate::paintSetAnchor(QPainter *painter, QString arguments)
{
    qreal dx, dy;

    if (arguments.contains("top"))
        dy = 0.0;
    else if (arguments.contains("bottom"))
        dy = mm_to_pixel(this->paperSize.height());

    if (arguments.contains("left"))
        dx = 0.0;
    else if (arguments.contains("right"))
        dx = mm_to_pixel(this->paperSize.width());

    painter->resetMatrix();
    painter->translate(dx, dy);
}

void PrintPaperTemplate::paintTranslate(QPainter *painter, QString arguments)
{
    arguments.remove(QRegExp("[^\\d\\.\\,(mm)\\-]"));
    QStringList coordStrings = arguments.split(',');
    if (coordStrings.size() != 2)
        return;

    qreal dx = this->text_to_pixel(coordStrings.at(0));
    qreal dy = this->text_to_pixel(coordStrings.at(1));
    painter->translate(dx, dy);
}

void PrintPaperTemplate::paintRotate(QPainter *painter, QString arguments)
{
    painter->rotate(arguments.toDouble());
}

void PrintPaperTemplate::parseScript(QPainter* painter)
{
    this->pen = QPen();
    this->brush = QBrush();
    this->font = QFont();

    QTextStream stream(&this->script);

    while (!stream.atEnd())
    {
        QString line = stream.readLine();

        QString command = line.left(line.indexOf(' '));
        QString arguments = line.right(line.length() - line.indexOf(' ') - 1).trimmed();

        if (command == "")
            continue;
        else if (command == "papersize")
            this->paintSetPaperSize(arguments);
        else if (command == "anchor")
            this->paintSetAnchor(painter, arguments);
        else if (command == "translate")
            this->paintTranslate(painter, arguments);
        else if (command == "rotate")
            this->paintRotate(painter, arguments);
        else if (command == "save")
            this->paintSave(painter);
        else if (command == "restore")
            this->paintRestore(painter);
        else if (command == "reset")
            this->paintResetTransform(painter);

        else if (command == "border")
            this->paintBorder(painter);
        else if (command == "foldMarking")
            this->paintFoldMarking(painter);
        else if (command == "linewidth")
            this->paintSetLineWidth(painter, arguments);
        else if (command == "pencolor")
            this->paintSetPenColor(painter, arguments);
        else if (command == "brushcolor")
            this->paintSetBrushColor(painter, arguments);
        else if (command == "line")
            this->paintLine(painter, arguments);
        else if (command == "circle")
            this->paintCircle(painter, arguments);
        else if (command == "rect")
            this->paintRect(painter, arguments);

        else if (command == "fontname")
            this->paintFontName(painter, arguments);
        else if (command == "fontsize")
            this->paintFontSize(painter, arguments);
        else if (command == "textline")
            this->paintTextLine(painter, arguments);
        else if (command == "textbox")
            this->paintTextBox(painter, arguments);
    }
}

QString PrintPaperTemplate::getScript()
{
    return this->script;
}

void PrintPaperTemplate::setScript(QString script)
{
    this->script = script;
}

QMap<QString, QString> PrintPaperTemplate::getDrawingVariables()
{
    return this->drawingVariables;
}

void PrintPaperTemplate::setDrawingVariables(QMap<QString, QString> drawingVariables)
{
    this->drawingVariables = drawingVariables;
}

void PrintPaperTemplate::paintSetLineWidth(QPainter* painter, QString arguments)
{
    pen.setWidthF(this->text_to_pixel(arguments));
    painter->setPen(pen);
}

void PrintPaperTemplate::paintSetPenColor(QPainter* painter, QString arguments)
{
    QColor color;
    color.setNamedColor(arguments);
    pen.setColor(color);
    painter->setPen(pen);
}

void PrintPaperTemplate::paintSetBrushColor(QPainter* painter, QString arguments)
{
    QColor color;
    color.setNamedColor(arguments);
    brush.setColor(color);
    brush.setStyle(Qt::SolidPattern);
    painter->setBrush(brush);
}

void PrintPaperTemplate::paintLine(QPainter* painter, QString arguments)
{
    arguments.remove(QRegExp("[^\\d\\.\\,(mm)\\-]"));
    QStringList coordStrings = arguments.split(',');
    if (coordStrings.size() != 4)
        return;

    qreal x1 = this->text_to_pixel(coordStrings.at(0));
    qreal y1 = this->text_to_pixel(coordStrings.at(1));
    qreal x2 = this->text_to_pixel(coordStrings.at(2));
    qreal y2 = this->text_to_pixel(coordStrings.at(3));

    painter->drawLine(x1, y1, x2, y2);
}

void PrintPaperTemplate::paintCircle(QPainter* painter, QString arguments)
{

}

void PrintPaperTemplate::paintRect(QPainter* painter, QString arguments)
{
    arguments.remove(QRegExp("[^\\d\\.\\,(mm)\\-]"));
    QStringList coordStrings = arguments.split(',');
    if (coordStrings.size() != 4)
        return;

    qreal x1 = this->text_to_pixel(coordStrings.at(0));
    qreal y1 = this->text_to_pixel(coordStrings.at(1));
    qreal w  = this->text_to_pixel(coordStrings.at(2));
    qreal h  = this->text_to_pixel(coordStrings.at(3));

    painter->drawRect(x1, y1, w, h);
}

void PrintPaperTemplate::paintFontName(QPainter *painter, QString arguments)
{
    font.setFamily(arguments);
    painter->setFont(font);
}

void PrintPaperTemplate::paintFontSize(QPainter *painter, QString arguments)
{
    if (arguments.contains("mm"))
        font.setPixelSize(text_to_pixel(arguments));
    else
        font.setPointSizeF(text_to_pixel(arguments));
    painter->setFont(font);
}

void PrintPaperTemplate::paintTextLine(QPainter* painter, QString arguments)
{
    QString text = arguments;
    text.remove(QRegExp("^\\S+\\s+\\S+\\s+"));
//    text.replace(QRegExp("\\\\n"), "\n"); // Newlines do not work in single line text
    QStringList coordStrings = arguments.split(',');
    if (coordStrings.size() < 2)
        return;

    qreal x1 = this->text_to_pixel(coordStrings.at(0));
    qreal y1 = this->text_to_pixel(coordStrings.at(1));

    painter->drawText(x1, y1, text);
}

void PrintPaperTemplate::paintTextBox(QPainter *painter, QString arguments)
{
    QString text = arguments;
    text.remove(QRegExp("^(\\S+\\s+){5}"));
    text.replace(QRegExp("\\\\n"), "\n"); // Convert "\n" to newline
    QStringList coordStrings = arguments.split(',');
    if (coordStrings.size() < 4)
        return;

    qreal x1 = this->text_to_pixel(coordStrings.at(0));
    qreal y1 = this->text_to_pixel(coordStrings.at(1));
    qreal w  = this->text_to_pixel(coordStrings.at(2));
    qreal h  = this->text_to_pixel(coordStrings.at(3));
    if (h < 0.0)
    {
        y1 += h;
        h = -h;
    }
    if (w < 0.0)
    {
        x1 += w;
        w = -w;
    }
    QRectF rect = QRectF(x1, y1, w, h);

    QString options = coordStrings.at(4);
    int flags = 0;
    if (options.contains("AlignLeft"))
        flags |= Qt::AlignLeft;
    if (options.contains("AlignRight"))
        flags |= Qt::AlignRight;
    if (options.contains("AlignHCenter"))
        flags |= Qt::AlignHCenter;
    if (options.contains("AlignTop"))
        flags |= Qt::AlignTop;
    if (options.contains("AlignBottom"))
        flags |= Qt::AlignBottom;
    if (options.contains("AlignVCenter"))
        flags |= Qt::AlignVCenter;
    if (options.contains("AlignCenter"))
        flags |= Qt::AlignCenter;
    if (options.contains("TextWordWrap"))
        flags |= Qt::TextWordWrap;

    painter->drawText(rect, flags, text);
}

int PrintPaperTemplate::mm_to_pixel(double mm)
{
    // Always 600 dpi printing
    return (600.0 * mm / 25.4);
}

qreal PrintPaperTemplate::text_to_pixel(QString text)
{
    text = text.trimmed();
    QString number = text;
    number.remove(QRegExp("[^\\d\\.\\-]"));
    qreal pixel = number.toDouble();
    if (text.endsWith("mm"))
        pixel = mm_to_pixel(pixel);
    return pixel;
}

void PrintPaperTemplate::on_pushButton_preview_clicked()
{
    QImage image_preview = QImage(this->mm_to_pixel(this->paperSize.width()), this->mm_to_pixel(this->paperSize.height()), QImage::Format_ARGB32_Premultiplied);
    image_preview.fill(Qt::white);
    QPainter painter(&image_preview);
    this->script = ui->plainTextEdit_script->toPlainText();
    this->parseScript(&painter);
    painter.end();
    ui->label_preview->setPixmap(QPixmap::fromImage(image_preview.scaled(ui->label_preview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
}
