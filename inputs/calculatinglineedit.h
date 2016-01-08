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

#ifndef CALCULATINGLINEEDIT_H
#define CALCULATINGLINEEDIT_H

#include <QLineEdit>
#include <QToolButton>
#include "equationsolver.h"
#include "stylesheetprovider.h"

class CalculatingLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CalculatingLineEdit(QWidget *parent = 0);
    ~CalculatingLineEdit();

    QString einheit() const;
    void setEinheit(const QString &einheit);

    float getValue() const;
    void setValue(const float &value);

    void resizeEvent(QResizeEvent *event);

private:
    QString m_einheit;
    QToolButton *m_btnUp;
    QToolButton *m_btnDown;

public slots:
    void setText(const QString &text);

private slots:
    void on_editingFinished();
    void on_buttonUp_clicked();
    void on_buttonDown_clicked();
};

#endif // CALCULATINGLINEEDIT_H
