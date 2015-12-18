/**********************************************************************
** smepowercad
** Copyright (C) 2015 Smart Micro Engineering GmbH
** Parts of this file are taken from the libdxfrw library header files
** and have been written by the libdxfrw team.
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

#ifndef EQUATIONSOLVER_H
#define EQUATIONSOLVER_H

#include <QString>
#include <QVector>
#include <QPair>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(equationsolver)

struct EquationResult {
    float result;
    bool isValid;
};

class EquationSolver
{
public:
    enum CharType { Number, Operation, Dot, Other };

    EquationSolver(QString s);
    EquationResult getResult() const;

private:
    typedef QPair<CharType, QString> Token;
    CharType getType(QChar c);
    bool greaterPriority(Token a, Token b);

    float m_result;
    bool m_resultValid;
};

#endif // EQUATIONSOLVER_H
