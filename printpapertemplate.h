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

#include <QBrush>
#include <QColor>
#include <QDialog>
#include <QFont>
#include <QImage>
#include <QMap>
#include <QMatrix>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QRectF>
#include <QRegExp>
#include <QSizeF>
#include <QTextStream>
#include <QMenu>
#include <QTableWidget>
#include <qmath.h>

#include "glwidget.h"
#include "treeviewitemdelegate.h"

namespace Ui {
class printPaperTemplate;
}

class PrintPaperTemplate : public QDialog
{
    Q_OBJECT

public:
    explicit PrintPaperTemplate(QWidget *parent, GLWidget* glWidget, ItemDB *itemDB);
    ~PrintPaperTemplate();

    void parseScript(QPainter* painter);

    QString getScript();
    QString getScriptFromEditor();
    void setScript(QString script);
    QMap<QString,QString> getDrawingVariables();
    void setDrawingVariables(QMap<QString,QString> drawingVariables);

public slots:
    void onFinishedReadingProjectFile();

private slots:
    void on_pushButton_preview_clicked();
    void on_plainTextEdit_script_textChanged();

    void on_treeView_printscripts_clicked(const QModelIndex &index);
    void on_treeView_printscripts_doubleClicked(const QModelIndex &index);
    void on_treeView_printscripts_customContextMenuRequested(const QPoint &pos);

    void newGroup();
    void newPrintscript();
    void copy();
    void cut();
    void paste();
    void rename();
    void remove();

    void on_btnProjectVarDel_clicked();
    void on_btnProjectVarAdd_clicked();
    void on_btnPrintscriptVarDel_clicked();
    void on_btnPrintscriptVarAdd_clicked();

    void on_btnLoadTemplate_clicked();

private:
    Ui::printPaperTemplate *ui;
    ItemDB *m_itemDB;
    PrintscriptTreeModel *m_model;
    GLWidget* glWidget;
    QSizeF paperSize;
    QString script;
    QPen pen;
    QBrush brush;
    QFont font;
    QMap<QString,QString> drawingVariables;
    QMenu *m_menuOnGroup;
    QMenu *m_menuOnPrintscript;
    QMenu *m_menuNoItem;
    QModelIndex m_indexAtContextMenuRequest;
    PrintscriptTreeItem *m_printscriptItemAtContextMenuRequest;
    bool m_isRenderingForPreview;
    QToolButton *m_btnLoadTemplate;

    PrintscriptTreeItem *m_copyItem;

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
    void paintScene(QPainter* painter, QString arguments);

    void resizeEvent(QResizeEvent *event);

    int mm_to_pixel(double mm);
    qreal text_to_pixel(QString text);

    Printscript *getCurrentPrintscript() const;
    QString newPrintscriptVariable(const QTableWidget *wdg);
};

#endif // PRINTPAPERTEMPLATE_H
