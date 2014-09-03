#include "cadcommand.h"

CADcommand::CADcommand(QObject *parent) :
    QObject(parent)
{
    commandList.insert("line", Line_startpoint);
}

CADcommand::CADcommand_status CADcommand::getStatus()
{
    return this->status;
}

QString CADcommand::takeLeftArgument(QString* str)
{
    QString left;
    int indexOfSpace = str->indexOf(" ");
    if (indexOfSpace == -1)
    {
        left = *str;
        str->clear();
        return left;
    }
    else
    {
        left = str->left(indexOfSpace);
        str->remove(0, indexOfSpace);
        *str = str->trimmed();
        return left;
    }
}

QPointF CADcommand::to2Dcoordinate(QString str, bool* ok)
{
    bool localOk1, localOk2;
    *ok = true;
    QStringList coordinateStrLst = str.split(",", QString::SkipEmptyParts);
    if (coordinateStrLst.length() != 2)
    {
        *ok = false;
        if (!*ok)
            emit signal_prompt_output("[" + str + "]: Invalid coordinate format!");
        return QPointF();
    }

    QPointF pos(coordinateStrLst.at(0).toFloat(&localOk1),
                coordinateStrLst.at(1).toFloat(&localOk2));

    *ok = localOk1 && localOk2;

    if (!*ok)
        emit signal_prompt_output("[" + str + "]: Invalid decimal format!");
    return pos;
}

void CADcommand::slot_prompt_input(QString str)
{
    QString arg;
    bool ok;

    str = str.trimmed();
    while (!str.isEmpty())
    {
        switch (status)
        {
        case NoCommand:
        {
            arg = takeLeftArgument(&str);
            arg = arg.toLower();
            status = commandList.value(arg, NoCommand);

            switch (status)
            {
            case Line_startpoint:
                emit signal_prompt_output("[" + arg + "]: Enter start point: ");

                break;
            default:
                break;
            }

            break;
        }
        case Line_startpoint:
        {
            arg = takeLeftArgument(&str);
            QPointF pos = to2Dcoordinate(arg, &ok);
            if (ok)
            {
                emit signal_startLine(pos);
                emit signal_prompt_output(arg);
                emit signal_prompt_output("Enter stop point: ");
                status = Line_endpoint;
            }
            break;
        }
        case Line_endpoint:
        {
            arg = takeLeftArgument(&str);
            QPointF pos = to2Dcoordinate(arg, &ok);
            if (ok)
            {
                emit signal_finishLine(pos);
                emit signal_prompt_output(arg);
                emit signal_prompt_output(">");
                status = NoCommand;
            }
            break;
        }
        default:
        {
            emit signal_prompt_output("[System error]: Invalid CADcommand.status");
        }
        }

        str = str.trimmed();
    }
}
