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

#include "equationsolver.h"

Q_LOGGING_CATEGORY(equationsolver, "equationsolver")

EquationSolver::EquationSolver(QString s)
    : m_result(0.0),
      m_resultValid(false)
{
    // replace ',' with '.' for string to double conversion
    s.replace(',', '.');

    // add sentinel operation ~
    s.append('~');

    bool foundNumbers = false;
    QVector<Token> tokens = QVector<Token>();
    QString buffer = QString();
    for (int i = 0; i < s.size(); i++) {
        QChar c = s.at(i);
        CharType type = getType(c);
        switch (type) {
        case Number:
            buffer.append(c);
            foundNumbers = true;
            break;
        case Operation:
            if (!foundNumbers) {
                qCWarning(equationsolver) << "Empty expression!";
                return;
            }
            if (buffer.size() > 0) {
                tokens.push_back(Token(EquationSolver::Number, buffer));
                buffer.clear();
            }
            tokens.push_back(Token(EquationSolver::Operation, s.mid(i, 1)));
            break;
        case Dot:
            buffer.append(c);
            break;
        default:
            break;
        }
        int ts = tokens.size();
        if (ts > 1 && tokens.last().first == tokens.at(ts -2).first) {
            qCWarning(equationsolver) << "Invalid expression!";
            return;
        }
    }

    // remove sentinel operation ~
    tokens.pop_back();

    QList<Token> calc, ops;
    QString lastOp;
    for (int i = 0; i < tokens.size(); i++) {
        calc.push_back(tokens.at(i));
        int cs = calc.size();

        switch (tokens.at(i).first) {
        case Number:
            if (lastOp != "") {
                calc.swap(cs -1, cs -2);
                lastOp = "";
            }
            break;
        case Operation:
            lastOp = tokens.at(i).second;
            ops.push_back(tokens.at(i));
            break;
        default:
            break;
        }

        int os = ops.size();

        // handle priority if needed
        if (os > 1 && tokens.at(i).first == Number) {
            if (greaterPriority(ops.at(os -1), ops.at(os -2))) {
                calc.swap(cs -3, cs -2);
                calc.swap(cs -2, cs -1);
                ops.swap(os -1, os -2);
            }
        }
    }

    // now calculate the expression
    Token a, b, op, result = Token(Number, "0");
    int atback = 0;
    while (calc.size() > 1) {
        // optain first three tokens (hopefully A,B,OP)
        a = calc.at(0);
        b = calc.at(1);
        op = calc.at(2);

        // check if a, b, op correspond to expected Type
        if (op.first != Operation || a.first != Number || b.first != Number) {
            calc.push_back(a);
            calc.pop_front();

            // keep track of waiting operands
            atback++;
        } else {
            // ready for calculating result; remove a, b, op from calc
            calc.pop_front(); calc.pop_front(); calc.pop_front();

            if (op.second == "*") {
                result.second = QString::number(a.second.toFloat() * b.second.toFloat());
            } else if (op.second == "+") {
                result.second = QString::number(a.second.toFloat() + b.second.toFloat());
            } else if (op.second == "-") {
                result.second = QString::number(a.second.toFloat() - b.second.toFloat());
            } else if (op.second == "/") {
                // check for division by zero
                if (b.second.toFloat() == 0.0) {
                    qCWarning(equationsolver) << "Division by zero!";
                    return;
                }

                result.second = QString::number(a.second.toFloat() / b.second.toFloat());
            }

            // insert result in calc
            calc.push_front(result);

            // restore waiting operands from back to front
            while (atback-- > 0) {
                calc.push_back(calc.back());
                calc.pop_back();
            }
            atback = 0;
        }
    }

    m_result = calc.front().second.toFloat();
    m_resultValid = true;
}

EquationResult EquationSolver::getResult() const
{
    EquationResult r;
    r.result = m_result;
    r.isValid = m_resultValid;
    return r;
}

EquationSolver::CharType EquationSolver::getType(QChar c)
{
    if (c >= '0' && c <= '9') return EquationSolver::Number;
    if (c == '*' || c == '+' || c == '-' || c == '/') return EquationSolver::Operation;
    if (c == '~') return EquationSolver::Operation;
    if (c == '.') return EquationSolver::Dot;

    return EquationSolver::Other;
}

bool EquationSolver::greaterPriority(Token a, Token b)
{
    return (( a.second == "*" || a.second == "/") && (b.second == "+" || b.second == "-"));
}

