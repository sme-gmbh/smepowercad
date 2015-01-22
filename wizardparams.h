#ifndef WIZARDPARAMS_H
#define WIZARDPARAMS_H

#include <QString>
#include <QVariant>

class WizardParams
{
public:
    WizardParams();
    WizardParams(const WizardParams &params);

    void insert(QString key, QVariant value);
    QVariant value(QString key);
    QVariant value(int index);
    QList<QString> keys();
    QList<QVariant> values();
    bool isEmpty();


private:
    QList<QString> keys_double;
    QList<double> values_double;
    QList<QString> keys_int;
    QList<int> values_int;
    QList<QString> keys_string;
    QList<QString> values_string;

};

#endif // WIZARDPARAMS_H
