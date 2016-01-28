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

#include "logging.h"

PrintPaperTemplate::PrintPaperTemplate(QWidget *parent, GLWidget *glWidget, ItemDB *itemDB) :
    QDialog(parent),
    ui(new Ui::printPaperTemplate),
    m_itemDB(itemDB),
    m_model(itemDB->getPrintscriptTreeModel()),
    glWidget(glWidget),
    m_isRenderingForPreview(false)
{
    this->setStyleSheet(StylesheetProvider::getStylesheet("QTreeView,Button,QLineEdit+QTextEdit"));
    ui->setupUi(this);
    m_printscriptTemplate = ui->plainTextEdit_script->document()->toPlainText();

    ui->treeView_printscripts->setItemDelegate(new TreeViewItemDelegate(this));
    ui->treeView_printscripts->setModel(m_itemDB->getPrintscriptTreeModel());

    m_menuNoItem = new QMenu(this);
    m_menuNoItem->addAction(QIcon(":/ui/printscript/icons/printscript-group-add.png"), tr("New group"), this, SLOT(newGroup()));

    m_menuOnGroup = new QMenu(this);
    m_menuOnGroup->addAction(QIcon(":/ui/printscript/icons/printscript-group-add.png"), tr("New group"), this, SLOT(newGroup()));
    m_menuOnGroup->addAction(QIcon(":/ui/printscript/icons/printscript-add.png"), tr("New printscript"), this, SLOT(newPrintscript()));
    m_menuOnGroup->addSeparator();
    m_menuOnGroup->addAction(tr("Copy"), this, SLOT(copy()));
    m_menuOnGroup->addAction(tr("Cut"), this, SLOT(cut()));
    m_menuOnGroup->addAction(tr("Paste"), this, SLOT(paste()));
    m_menuOnGroup->addSeparator();
    m_menuOnGroup->addAction(tr("Rename"), this, SLOT(rename()));
    m_menuOnGroup->addSeparator();
    m_menuOnGroup->addAction(tr("Remove"), this, SLOT(remove()));

    m_menuOnPrintscript = new QMenu(this);
    m_menuOnPrintscript->addAction(tr("Copy"), this, SLOT(copy()));
    m_menuOnPrintscript->addAction(tr("Cut"), this, SLOT(cut()));
    m_menuOnPrintscript->addSeparator();
    m_menuOnPrintscript->addAction(tr("Rename"), this, SLOT(rename()));
    m_menuOnPrintscript->addSeparator();
    m_menuOnPrintscript->addAction(tr("Remove"), this, SLOT(remove()));

    m_btnLoadTemplate = new QToolButton(ui->plainTextEdit_script);
    m_btnLoadTemplate->setToolTip(tr("Load printscript template"));
    m_btnLoadTemplate->setIcon(QIcon(":/ui/printscript/icons/printscript-load-template.png"));
    m_btnLoadTemplate->setIconSize(QSize(16, 22));
    m_btnLoadTemplate->setCursor(Qt::ArrowCursor);
    m_btnLoadTemplate->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_btnLoadTemplate->setStyleSheet("QToolButton { border: 0px; background: 0; padding: 0px; }");
    m_btnLoadTemplate->setFixedHeight(22);
    m_btnLoadTemplate->setFixedWidth(22);
    m_btnLoadTemplate->move(ui->plainTextEdit_script->rect().width() - 22 - 15, 5);
    connect(m_btnLoadTemplate, &QToolButton::clicked, this, &PrintPaperTemplate::on_btnLoadTemplate_clicked);
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
        // Size and position functions
        else if (command == "papersize")
            this->paintSetPaperSize(arguments);
        else if (painter == NULL)
            continue;
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

        // Direct drawing functions
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

        // Text functions
        else if (command == "fontname")
            this->paintFontName(painter, arguments);
        else if (command == "fontsize")
            this->paintFontSize(painter, arguments);
        else if (command == "textline")
            this->paintTextLine(painter, arguments);
        else if (command == "textbox")
            this->paintTextBox(painter, arguments);

        // Scene drawing functions
        else if (command == "scene")
            this->paintScene(painter, arguments);
    }
}

QString PrintPaperTemplate::getScript()
{
    return this->script;
}

QString PrintPaperTemplate::getScriptFromEditor()
{
    return ui->plainTextEdit_script->toPlainText();
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

void PrintPaperTemplate::onFinishedReadingProjectFile()
{
    ui->tableWidget_projectVariables->clearContents();

    QMap<QString,QString> vars = m_itemDB->getGlobalPrintscriptVariables();
    QStringList keys = vars.keys();
    QStringList values = vars.values();
    ui->tableWidget_projectVariables->setRowCount(vars.count());
    for (int i = 0; i < vars.count(); i++) {
        ui->tableWidget_projectVariables->setItem(i, 0, new QTableWidgetItem(keys.at(i)));
        ui->tableWidget_projectVariables->setItem(i, 1, new QTableWidgetItem(values.at(i)));
    }

    ui->treeView_printscripts->reset();
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
    Q_UNUSED(painter)
    Q_UNUSED(arguments)
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

void PrintPaperTemplate::paintScene(QPainter *painter, QString arguments)
{
//    QString text = arguments;
//    text.remove(QRegExp("^(\\S+\\s+){5}"));
//    text.replace(QRegExp("\\\\n"), "\n"); // Convert "\n" to newline
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

    QMatrix4x4 matrix_modelview = this->glWidget->getMatrix_modelview();
    QMatrix4x4 matrix_rotation = this->glWidget->getMatrix_rotation();
    this->glWidget->render_image(painter, x1, y1, w, h, matrix_modelview, matrix_rotation);
}

void PrintPaperTemplate::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

    m_btnLoadTemplate->move(ui->plainTextEdit_script->rect().width() - 22 - 15, 5);
}

int PrintPaperTemplate::mm_to_pixel(double mm)
{
    double dpi = 600.0;

    if (m_isRenderingForPreview) dpi = 300.0;

    // Always 600 dpi printing
    return (dpi * mm / 25.4);
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

Printscript *PrintPaperTemplate::getCurrentPrintscript() const
{
    PrintscriptTreeItem *item = static_cast<PrintscriptTreeItem*>(ui->treeView_printscripts->currentIndex().internalPointer());

    if (item == NULL) return NULL;

    return dynamic_cast<Printscript*>(item);
}

QString PrintPaperTemplate::newPrintscriptVariable(const QTableWidget *wdg)
{
    QString name;
    bool alreadyExists = false;
    bool ok;

    do {
        if (alreadyExists)
            QMessageBox::warning(this, tr("Add variable"), tr("A variable with the name %1 already exists. Choose a different name.").arg(name));

        if (name.isEmpty() || alreadyExists) {
            do {
                name = QInputDialog::getText(this, tr("Add variable"), tr("Variable name"), QLineEdit::Normal, NULL, &ok);

                if (!ok) break;
                if (name.isEmpty())
                    QMessageBox::warning(this, tr("Add variable"), tr("Variable name cannot be empty!"));
            } while (name.isEmpty());
        }

        if (!ok) break;

    } while (wdg->findItems(name, Qt::MatchExactly).count() > 0 && (alreadyExists = true));

    if (ok) return name;

    return QString();
}

void PrintPaperTemplate::on_pushButton_preview_clicked()
{
    m_isRenderingForPreview = true;
    // First dummy-parse the script to get the papersize, so we know how large the image will be
    this->script = ui->plainTextEdit_script->toPlainText();
    this->parseScript(NULL);
    // Now constuct the image and the corresponding painter
    QSize imgSize = QSize(this->mm_to_pixel(this->paperSize.width()), this->mm_to_pixel(this->paperSize.height()));
    QImage image_preview = QImage(imgSize, QImage::Format_ARGB32_Premultiplied);
    image_preview.fill(Qt::white);
    QPainter painter(&image_preview);

    // Production of graphic content
    this->parseScript(&painter);
    painter.end();
    m_isRenderingForPreview = false;

    ui->label_preview->setPixmap(QPixmap::fromImage(image_preview.scaled(ui->label_preview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
}

void PrintPaperTemplate::on_plainTextEdit_script_textChanged()
{
    PrintscriptTreeItem *item = static_cast<PrintscriptTreeItem*>(ui->treeView_printscripts->currentIndex().internalPointer());
    Printscript *ps = dynamic_cast<Printscript*>(item);
    if (ps == NULL) return;
    ps->script = ui->plainTextEdit_script->document()->toPlainText();
}

void PrintPaperTemplate::on_treeView_printscripts_clicked(const QModelIndex &index)
{
    PrintscriptTreeItem *item = static_cast<PrintscriptTreeItem*>(index.internalPointer());
    Printscript *ps = dynamic_cast<Printscript*>(item);
    if (ps == NULL) return;

    ui->plainTextEdit_script->setPlainText(ps->script);

    ui->tableWidget_psVariables->clearContents();
    QMap<QString,QString> vars = ps->getVariables();
    QStringList keys = vars.keys();
    QStringList values = vars.values();
    ui->tableWidget_psVariables->setRowCount(vars.count());
    for (int i = 0; i < vars.count(); i++) {
        ui->tableWidget_psVariables->setItem(i, 0, new QTableWidgetItem(keys.at(i)));
        ui->tableWidget_psVariables->setItem(i, 1, new QTableWidgetItem(values.at(i)));
    }
}



void PrintPaperTemplate::on_treeView_printscripts_doubleClicked(const QModelIndex &index)
{
    on_treeView_printscripts_clicked(index);
    ui->tabWidget->setCurrentIndex(1);
}

void PrintPaperTemplate::on_treeView_printscripts_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->treeView_printscripts->indexAt(pos);
    m_indexAtContextMenuRequest = index;
    m_printscriptItemAtContextMenuRequest = static_cast<PrintscriptTreeItem*>(index.internalPointer());

    Printscript *ps = dynamic_cast<Printscript*>(m_printscriptItemAtContextMenuRequest);

    if (index.isValid()) {
        if (ps != NULL)
            m_menuOnPrintscript->popup(QCursor::pos());
        else
            m_menuOnGroup->popup(QCursor::pos());
    } else {
        m_menuNoItem->popup(QCursor::pos());
    }
}

void PrintPaperTemplate::newGroup()
{
    QString groupName;
    bool alreadyExists = false;
    bool ok;

    do {
        if (alreadyExists)
            QMessageBox::warning(this, tr("New group"), tr("Group name already in use! Try a different name!"));

        if (groupName.isEmpty() || alreadyExists) {
            do {
                groupName = QInputDialog::getText(this, tr("New group"), tr("Group name"), QLineEdit::Normal, NULL, &ok);

                if (!ok) break;
                if (groupName.isEmpty())
                    QMessageBox::warning(this, tr("New group"), tr("Group name cannot be empty"));
            } while(groupName.isEmpty());
        }

        if (!ok) break;
    } while (m_model->findItemByName(groupName) != NULL && (alreadyExists = true));

    if (!ok) return;

    int at = 0;
    QModelIndex parent;
    if (m_printscriptItemAtContextMenuRequest) {
        at = m_printscriptItemAtContextMenuRequest->parentItem()->childCount();
        parent = m_indexAtContextMenuRequest;
    } else {
        parent = m_model->parent(m_indexAtContextMenuRequest);
    }

    m_model->insertGroup(groupName, parent, at);
}

void PrintPaperTemplate::rename()
{
    PrintscriptTreeItem *item = m_printscriptItemAtContextMenuRequest;
    QString itemName;
    bool ok;
    do {
        itemName = QInputDialog::getText(this, tr("Rename item %1").arg(item->name),
                                          tr("Item name"), QLineEdit::Normal, item->name, &ok);

        if (!ok) break;

        if (itemName.isEmpty())
            QMessageBox::warning(this, tr("Rename item"), tr("Item name cannot be empty"));
    } while (itemName.isEmpty());

    if (ok)
        m_printscriptItemAtContextMenuRequest->name = itemName;
}

void PrintPaperTemplate::newPrintscript()
{
    QString psName;
    bool alreadyExists = false;
    bool ok;

    do {
        if (alreadyExists)
            QMessageBox::warning(this, tr("New printscript"), tr("Printscript name already in use! Try a different name!"));

        if (psName.isEmpty() || alreadyExists) {
            do {
                psName = QInputDialog::getText(this, tr("New printscript"), tr("Printscript name"), QLineEdit::Normal, NULL, &ok);

                if (!ok) break;
                if (psName.isEmpty())
                    QMessageBox::warning(this, tr("New printscript"), tr("Printscript name cannot be empty"));
            } while(psName.isEmpty());
        }

        if (!ok) break;
    } while (m_model->findItemByName(psName) != NULL && (alreadyExists = true));

    if (!ok) return;

    int at = 0;
    QModelIndex parent;
    if (m_printscriptItemAtContextMenuRequest) {
        at = m_printscriptItemAtContextMenuRequest->parentItem()->childCount();
        parent = m_indexAtContextMenuRequest;
    } else {
        parent = m_model->parent(m_indexAtContextMenuRequest);
    }

    m_model->insertPrintscript(psName, parent, at);
}

void PrintPaperTemplate:: copy()
{
    if (!m_printscriptItemAtContextMenuRequest) return;

    Printscript *ps = dynamic_cast<Printscript*>(m_printscriptItemAtContextMenuRequest);
    if (ps)
        m_copyItem = new Printscript(ps);
    else
        m_copyItem = new PrintscriptTreeItem(m_printscriptItemAtContextMenuRequest);
}

void PrintPaperTemplate::cut()
{
    if (!m_printscriptItemAtContextMenuRequest) return;

    Printscript *ps = dynamic_cast<Printscript*>(m_printscriptItemAtContextMenuRequest);
    if (ps)
        m_copyItem = new Printscript(ps);
    else
        m_copyItem = new PrintscriptTreeItem(m_printscriptItemAtContextMenuRequest);

    bool success = m_model->removeItemAt(m_model->parent(m_indexAtContextMenuRequest), m_indexAtContextMenuRequest);

    if (!success) m_copyItem = NULL;
}

void PrintPaperTemplate::paste()
{
    if (!m_copyItem) return;

    int at = 0;
    QModelIndex parent;
    if (m_printscriptItemAtContextMenuRequest) {
        at = m_printscriptItemAtContextMenuRequest->parentItem()->childCount();
        parent = m_indexAtContextMenuRequest;
    } else {
        parent = m_model->parent(m_indexAtContextMenuRequest);
    }
    m_model->insertItem(m_copyItem, parent, at);
}

void PrintPaperTemplate::remove()
{
    if (!m_printscriptItemAtContextMenuRequest) return;

    int ret = QMessageBox::question(this, tr("Remove item"), tr("Do you want to remove item %1?").arg(m_printscriptItemAtContextMenuRequest->name), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (ret != QMessageBox::Yes) return;

    bool success = m_model->removeItemAt(m_model->parent(m_indexAtContextMenuRequest), m_indexAtContextMenuRequest);

    if (!success)
        QMessageBox::warning(this, tr("Delete item"), tr("Unable to delete item. Maybe it has child items."));
}

void PrintPaperTemplate::on_btnProjectVarDel_clicked()
{
    int row = ui->tableWidget_projectVariables->currentRow();

    if (row < 0) return;

    m_itemDB->removePrintscriptVariable(ui->tableWidget_projectVariables->itemAt(0, row)->text());
    ui->tableWidget_projectVariables->removeRow(row);
}

void PrintPaperTemplate::on_btnProjectVarAdd_clicked()
{
    QString name = newPrintscriptVariable(ui->tableWidget_projectVariables);
    // TODO: only add if project is openend

    if (!name.isEmpty()) {
        int row = ui->tableWidget_projectVariables->rowCount();
        ui->tableWidget_projectVariables->setRowCount(row +1);
        ui->tableWidget_projectVariables->setItem(row, 0, new QTableWidgetItem(name));
        m_itemDB->insertPrintscriptVariable(name, QString());
    }
}

void PrintPaperTemplate::on_btnPrintscriptVarDel_clicked()
{
    int row = ui->tableWidget_psVariables->currentRow();

    if (row < 0) return;

    Printscript *ps = static_cast<Printscript*>(ui->treeView_printscripts->currentIndex().internalPointer());
    ps->removeVariable(ui->tableWidget_psVariables->itemAt(0, row)->text());
    ui->tableWidget_psVariables->removeRow(row);
}

void PrintPaperTemplate::on_btnPrintscriptVarAdd_clicked()
{
    QString name = newPrintscriptVariable(ui->tableWidget_psVariables);
    Printscript *ps = getCurrentPrintscript();

    if (!name.isEmpty() && ps) {
        int row = ui->tableWidget_psVariables->rowCount();
        ui->tableWidget_psVariables->setRowCount(row +1);
        ui->tableWidget_psVariables->setItem(row, 0, new QTableWidgetItem(name));
        ps->insertVariable(name, QString());
    }
}

void PrintPaperTemplate::on_btnLoadTemplate_clicked()
{
    int ret = QMessageBox::question(this, tr("Load printscript template"), tr("Do you want to load the printscript template and overwrite your current printscript?"));
    if (ret != QMessageBox::Yes) return;

    ui->plainTextEdit_script->setPlainText(m_printscriptTemplate);
}
