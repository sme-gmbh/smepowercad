#ifndef WIZARDPARAMS_H
#define WIZARDPARAMS_H

#include <QString>
#include <QVariant>

class WizardParams
{
public:
    WizardParams();
    WizardParams(const WizardParams &params);
    ~WizardParams();

    void insert(QString key, QVariant value);
    void insert(QString key, double value);
    void insert(QString key, int value);
    void insert(QString key, QString value);
    QVariant value(QString key);
    QVariant value(int index);
    QList<QString> keys();
    QList<QVariant> values();
    bool isEmpty();


private:
//    static QList<QString> globalKeys;
//    QList<quint16> keys_indices_double;
    QList<QString> keys_double;
    QList<double> values_double;
//    QList<quint16> keys_indices_int;
    QList<QString> keys_int;
    QList<int> values_int;
//    QList<quint16> keys_indices_string;
    QList<QString> keys_string;
    QList<QString> values_string;

//    quint16 indexOfGlobalKey(QString key);


};

#endif // WIZARDPARAMS_H
