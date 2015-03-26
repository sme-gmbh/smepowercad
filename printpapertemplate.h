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

#ifndef PRINTPAPERTEMPLATE_H
#define PRINTPAPERTEMPLATE_H

#include <QDialog>
#include <QSizeF>
#include <QPainter>
#include <QMap>
#include <QPen>
#include <QBrush>
#include <QFont>

namespace Ui {
class printPaperTemplate;
}

class PrintPaperTemplate : public QDialog
{
    Q_OBJECT

public:
    explicit PrintPaperTemplate(QWidget *parent = 0);
    ~PrintPaperTemplate();

    void parseScript(QPainter* painter);

    QString getScript();
    void setScript(QString script);
    QMap<QString,QString> getDrawingVariables();
    void setDrawingVariables(QMap<QString,QString> drawingVariables);


private slots:
    void on_pushButton_preview_clicked();

private:
    Ui::printPaperTemplate *ui;
    QSizeF paperSize;
    QString script;
    QPen pen;
    QBrush brush;
    QFont font;
    QMap<QString,QString> drawingVariables;

    void paintSetPaperSize(QString arguments);
    void paintBorder(QPainter* painter);
    void paintFoldMarking(QPainter* painter);
    void paintSave(QPainter* painter);
    void paintRestore(QPainter* painter);
    void paintResetTransform(QPainter* painter);
    void paintSetAnchor(QPainter* painter, QString arguments);
    void paintTranslate(QPainter* painter, QString arguments);
    void paintRotate(QPainter* painter, QString arguments);
    void paintSetLineWidth(QPainter* painter, QString arguments);
    void paintSetPenColor(QPainter* painter, QString arguments);
    void paintSetBrushColor(QPainter* painter, QString arguments);
    void paintLine(QPainter* painter, QString arguments);
    void paintCircle(QPainter* painter, QString arguments);
    void paintRect(QPainter* painter, QString arguments);
    void paintFontName(QPainter* painter, QString arguments);
    void paintFontSize(QPainter* painter, QString arguments);
    void paintTextLine(QPainter* painter, QString arguments);
    void paintTextBox(QPainter* painter, QString arguments);

    int mm_to_pixel(double mm);
    qreal text_to_pixel(QString text);
};

#endif // PRINTPAPERTEMPLATE_H
