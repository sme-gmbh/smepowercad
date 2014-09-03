#ifndef CADCOMMAND_H
#define CADCOMMAND_H

#include <QObject>
#include <QMap>
#include <QRegExp>
#include <QPointF>
#include <QStringList>

class CADcommand : public QObject
{
    Q_OBJECT

    typedef enum
    {
        NoCommand,
        Line_startpoint,
        Line_endpoint
    } CADcommand_status;

public:
    explicit CADcommand(QObject *parent = 0);
    CADcommand_status getStatus();

private:
    CADcommand_status status;
    QMap<QString, CADcommand_status> commandList;
    QString takeLeftArgument(QString* str);
    QPointF to2Dcoordinate(QString str, bool *ok);

signals:
    void signal_prompt_output(QString str);

    void signal_startLine(QPointF pos);
    void signal_update_line(QPointF newPos);
    void signal_finishLine(QPointF pos);
    void signal_abort();


public slots:
    void slot_prompt_input(QString str);

};

#endif // CADCOMMAND_H
